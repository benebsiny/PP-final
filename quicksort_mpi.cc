#define OMPI_SKIP_MPICXX 1 // To eliminate the warning of OpenMPI
#include <mpi.h>
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include "commons/helper.hpp"

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

void quickSort(std::vector<ll> &arr, ll low, ll high)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void XDEv11_merge(std::vector<ll> &arr, ll divided)
{
	std::vector<ll> res{};

	std::vector<std::pair<ll, int>> pq(divided);
    ll interval = arr.size() / divided;
    for (int i = 0; i < divided; ++i) {
		std::pair<ll, int> x{arr[i * interval], i * interval};
		int j{i - 1};
		for (; j >= 0 && pq[j] > x; --j) pq[j + 1] = pq[j];
		pq[j + 1] = x;
	}

	while (!pq.empty()) {
		auto x = pq[0];
		res.push_back(x.first);
		int i = x.second + 1;
		if (i % interval == 0) {
			for (int j = 1; j < pq.size(); ++j) pq[j - 1] = pq[j];
			pq.pop_back();
		} else {
			x = {arr[i], i};
			int j{1};
			for (; j < pq.size() && pq[j] < x; ++j) pq[j - 1] = pq[j];
			pq[j - 1] = x;
		}
	}

	arr = move(res);
}

void merge(std::vector<ll> &arr, ll divided)
{

    std::vector<ll> left;
    ll interval = arr.size() / divided;

    for (ll d = 0; d < divided; d++)
    {

        std::vector<ll> res(left.size() + interval);
        ll residx = 0;

        size_t i = 0;
        ll j = interval * d;
        while (i < left.size() && j < interval * (d + 1))
        {
            if (left[i] < arr[j])
            {
                res[residx++] = left[i++];
            }
            else
            {
                res[residx++] = arr[j++];
            }
        }
        if (i != left.size())
        {
            for (; i < left.size(); i++)
                res[residx++] = left[i];
        }
        if (j != interval * (d + 1))
        {
            for (; j < interval * (d + 1); j++)
                res[residx++] = arr[j];
        }
        left = res;
    }
    arr = left;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    if (argc != 2)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::string filename = argv[1];

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<ll> arr;

    ll n = 0;

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

    // Send the count of the array to other process
    MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    ll localN = n / size;
    std::vector<ll> localArr(localN);

    auto start_time = std::chrono::high_resolution_clock::now();

    MPI_Scatter(arr.data(), localN, MPI_LONG_LONG_INT, localArr.data(), localN, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
    quickSort(localArr, 0, localN - 1);
    MPI_Gather(localArr.data(), localN, MPI_LONG_LONG_INT, arr.data(), localN, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        XDEv11_merge(arr, size);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

        validate(arr);
    }

    MPI_Finalize();
    return 0;
}
