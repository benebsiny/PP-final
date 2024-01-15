#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "commons/helper.hpp"

typedef long long ll;
#define SWAP(x, y) \
    ll tmp = (x);  \
    (x) = (y);     \
    (y) = (tmp);

__device__ void quickSort(ll *arr, ll low, ll high, ll depth);

// CUDA kernel for partition
__device__ ll partition(ll *arr, ll low, ll high)
{
    ll pivot = arr[high];
    ll i = low - 1;

    for (ll j = low; j <= high - 1; ++j)
    {
        if (arr[j] < pivot)
        {
            i++;
            SWAP(arr[i], arr[j]);
        }
    }

    SWAP(arr[i + 1], arr[high]);
    return i + 1;
}

__global__ void tap(ll *arr, ll low1, ll high1, ll low2, ll high2, ll depth)
{
    int index = threadIdx.x;
    if (index == 0)
    {
        quickSort(arr, low1, high1, depth); // Left part
    }
    else if (index == 1)
    {
        quickSort(arr, low2, high2, depth); // Right part
    }
    else
    {
        printf("GG\n");
    }
    cudaDeviceSynchronize();
}

// CUDA kernel for quicksort
__device__ void quickSort(ll *arr, ll low, ll high, ll depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth > 0)
        {
            tap<<<1, 2>>>(arr, low, pi - 1, pi + 1, high, depth - 1);
        }
        else
        {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);
        }
    }
}

__host__ bool read_data(std::vector<long long> &arr, std::string filename)
{
    std::ifstream inFile(filename, std::ios::binary | std::ios::in);

    if (!inFile.is_open())
    {
        return false;
    }

    long long value;
    while (inFile.read(reinterpret_cast<char *>(&value), sizeof(long long)))
    {
        arr.push_back(value);
    }

    inFile.close();

    return true;
}

__host__ void validate(std::vector<ll> &arr)
{
    ll n = arr.size();
    for (ll i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            std::cout << "error at " << i << " " << arr[i] << " " << arr[i + 1] << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::vector<ll> arr;
    std::string filename = argv[1];
    if (!read_data(arr, filename))
    {
        std::cerr << "Can't read data\n";
        return 1;
    }
    ll N = arr.size();
    std::cout << "Load count:" << N << std::endl;

    // Allocate and copy array to device
    ll *d_arr;
    cudaMalloc((void **)&d_arr, N * sizeof(ll));
    cudaMemcpy(d_arr, arr.data(), N * sizeof(ll), cudaMemcpyHostToDevice);

    // Call CUDA kernel to perform quicksort
    tap<<<1, 1>>>(d_arr, 0, N - 1, -1, -1, 2);
    cudaDeviceSynchronize();

    // Copy back the sorted array
    cudaMemcpy(arr.data(), d_arr, N * sizeof(ll), cudaMemcpyDeviceToHost);
    validate(arr);

    cudaFree(d_arr);

    return 0;
}
