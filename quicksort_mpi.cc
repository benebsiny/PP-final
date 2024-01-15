#define OMPI_SKIP_MPICXX 1 // To eliminate the warning of OpenMPI
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "commons/helper.hpp"

ll n = 0;
int threshold = 8;

int partition(std::vector<ll> &arr, ll low, ll high)
{
    ll pivot = arr[high];
    ll i = low - 1;

    for (ll j = low; j <= high - 1; ++j)
    {
        if (arr[j] < pivot)
        {
            ++i;
            SWAP(arr[i], arr[j]);
        }
    }

    SWAP(arr[i + 1], arr[high]);
    return i + 1;
}

int rank, size, totalDepth;
int k = 0;

void quickSort(std::vector<ll> &arr, ll low, ll high, int depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth == 0)
        {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);
        }
        else
        {
            int target = (1 << (totalDepth - depth)) + rank; // Target rank

            if (target < size)
            { // The `size` may not be the power of 2 If it's not, the `target` might be larger than `size`

                ll info[3] = {pi - low, depth - 1, rank};                                                     // count, depth, rank
                MPI_Send(&info, 3, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD);                             // Send infomation to target rank
                MPI_Send(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD); // Send array to target rank
                quickSort(arr, pi + 1, high, depth - 1);
                MPI_Recv(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Get the calculated part from another rank
            }
            else
            { // No need to `MPI_Send` when there's no more process to run
                quickSort(arr, low, pi - 1, 0);
                quickSort(arr, pi + 1, high, 0);
            }
        }
    }
}

// Load balance
void quickSort2(std::vector<ll> &arr, ll low, ll high, int depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth == 0)
        {
            quickSort2(arr, low, pi - 1, 0);
            quickSort2(arr, pi + 1, high, 0);
        }
        else
        {
            int target = (1 << (totalDepth - depth)) + rank; // Target rank

            if (target < size)
            { // The `size` may not be the power of 2 If it's not, the `target` might be larger than `size`

                if (pi - low <= n / threshold)
                {
                    quickSort2(arr, low, pi - 1, 0);      // Left part
                    quickSort2(arr, pi + 1, high, depth); // Right part
                }
                else if (high - pi <= n / threshold)
                {
                    quickSort2(arr, pi + 1, high, 0);    // Right part
                    quickSort2(arr, low, pi - 1, depth); // Left part
                }
                else
                {
                    ll info[3] = {pi - low, depth - 1, rank};                                                     // count, depth, rank
                    MPI_Send(&info, 3, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD);                             // Send infomation to target rank
                    MPI_Send(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD); // Send array to target rank
                    quickSort2(arr, pi + 1, high, depth - 1);
                    MPI_Recv(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Get the calculated part from another rank
                }
            }
            else
            { // No need to `MPI_Send` when there's no more process to run
                quickSort2(arr, low, pi - 1, 0);
                quickSort2(arr, pi + 1, high, 0);
            }
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    if (argc < 2 || argc > 3)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file> [threshold]\n";
        return 1;
    }

    std::string filename = argv[1];

    bool loadBalance = false;
    if (argc >= 3)
    {
        threshold = atoi(argv[2]);
        loadBalance = true;
    }
    std::cout << "Run " << ((loadBalance) ? "with" : "without") << " load balance.\n";

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    totalDepth = std::ceil(std::log2(size));

    std::vector<ll> arr;

    if (rank == 0)
    {
        if (!read_data(arr, filename))
        {
            std::cerr << "Can't read data\n";
            return 1;
        }
        n = arr.size();
        std::cout << "Load count: " << n << std::endl;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    if (rank == 0)
    {
        if (loadBalance)
        {
            quickSort2(arr, 0, n - 1, totalDepth);
        }
        else
        {
            quickSort(arr, 0, n - 1, totalDepth);
        }
    }
    else
    {
        ll info[3];                                                                                  // count, depth, fromRank
        MPI_Recv(&info, 3, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Get info

        std::vector<ll> localArr((int)info[0]);
        MPI_Recv(localArr.data(), info[0], MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Get data
        if (loadBalance)
        {
            quickSort2(localArr, 0, info[0] - 1, info[1]);
        }
        else
        {
            quickSort(localArr, 0, info[0] - 1, info[1]);
        }
        MPI_Send(localArr.data(), info[0], MPI_LONG_LONG_INT, info[2], 0, MPI_COMM_WORLD); // Send data back
    }

    if (rank == 0)
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

        validate(arr);
    }

    MPI_Finalize();
    return 0;
}
