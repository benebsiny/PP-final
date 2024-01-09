#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cmath>


typedef long long ll;
#define SWAP(x, y)  ll tmp = (x); \
                    (x) = (y); \
                    (y) = (tmp);

int partition(std::vector<ll>& arr, ll low, ll high) {
    ll pivot = arr[high];
    ll i = low - 1;

    for (ll j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            SWAP(arr[i], arr[j]);
        }
    }

    SWAP(arr[i + 1], arr[high]);
    return i + 1;
}

int rank, size, totalDepth;
int k = 0;

void quickSort(std::vector<ll>& arr, ll low, ll high, int depth) {
    if (low < high) {
        ll pi = partition(arr, low, high);

        if (depth == 0) {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);

        } else {            
            int target = (1 << (totalDepth - depth)) + rank;  // Target rank

            if (target < size) {  // The `size` may not be the power of 2 If it's not, the `target` might be larger than `size`

                ll info[3] = {pi - low, depth - 1, rank}; // count, depth, rank
                MPI_Send(&info, 3, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD);                               // Send infomation to target rank
                MPI_Send(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD);   // Send array to target rank
                quickSort(arr, pi + 1, high, depth - 1);
                MPI_Recv(&(*arr.begin()) + (int)low, pi - low, MPI_LONG_LONG_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Get the calculated part from another rank

            } else {  // No need to `MPI_Send` when there's no more process to run
                quickSort(arr, low, pi - 1, 0);
                quickSort(arr, pi + 1, high, 0);
            }
        }
    }
}

void read_data(std::vector<ll>& arr) {
    std::ifstream inFile("random_numbers.bin", std::ios::binary | std::ios::in);

    if (inFile.is_open()) {
        ll value;
        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(ll))) {
            arr.push_back(value);
        }

        inFile.close();
        std::cout << "Number read from binary file successfully\n";
    } else {
        std::cerr << "Can't open random_numbers.bin\n";
    }
}

// If correct, print nothing. Otherwise, print error index and its value
void validate(std::vector<ll>& arr) {
    ll n = arr.size();
    for (ll i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) {
            std::cout << "error at " << i << " " << arr[i] << " " << arr[i+1] << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    totalDepth = std::ceil(std::log2(size));

    std::vector<ll> arr;
    ll n = 0;

    if (rank == 0) {
        read_data(arr);
        n = arr.size();
        std::cout << "Load count:" << n << std::endl;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    if (rank == 0) {
        quickSort(arr, 0, n-1, totalDepth);
    } else {
        ll info[3];  // count, depth, fromRank
        MPI_Recv(&info, 3, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Get info

        std::vector<ll> localArr((int)info[0]);
        MPI_Recv(localArr.data(), info[0], MPI_LONG_LONG_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Get data
        quickSort(localArr, 0, info[0] - 1, info[1]);
        MPI_Send(localArr.data(), info[0], MPI_LONG_LONG_INT, info[2], 0, MPI_COMM_WORLD);  // Send data back
    }

    if (rank == 0) {
        validate(arr);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
