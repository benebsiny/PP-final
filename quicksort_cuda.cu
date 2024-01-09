#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctime>

typedef long long ll;
#define SWAP(x, y)  ll tmp = (x); \
                    (x) = (y); \
                    (y) = (tmp);

__device__ void quickSort(ll* arr, ll low, ll high, ll depth);

// CUDA kernel for partition
__device__ ll partition(ll* arr, ll low, ll high) {
    ll pivot = arr[high];
    ll i = low - 1;

    for (ll j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            i++;
            SWAP(arr[i], arr[j]);
        }
    }

    SWAP(arr[i + 1], arr[high]);
    return i + 1;
}

__global__ void tap(ll* arr, ll low1, ll high1, ll low2, ll high2, ll depth) {
    int index = threadIdx.x;
    if (index == 0) {
        // printf("[%d]: low1: %10lld, high1: %10lld, depth: %lld\n", index, low1, high1, depth);
        quickSort(arr, low1, high1, depth);  // Left part
    } else if (index == 1) {
        // printf("[%d]: low2: %10lld, high2: %10lld, depth: %lld\n", index, low2, high2, depth);
        quickSort(arr, low2, high2, depth); // Right part
    } else {
        printf("GG\n");
    }
    cudaDeviceSynchronize();
}

// CUDA kernel for quicksort
__device__ void quickSort(ll* arr, ll low, ll high, ll depth) {
    if (low < high) {
        ll pi = partition(arr, low, high);

        if (depth > 0) {
            // printf("[C]: low1: %10lld, high1: %10lld, low2: %10lld, high2: %10lld, depth: %d\n", low, pi - 1, pi + 1, high, depth - 1);
            tap<<<1,2>>>(arr, low, pi - 1, pi + 1, high, depth - 1);

        } else {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);
        }
    } else {
        // printf("[%d] low >= high: %lld >= %lld\n", depth, low, high);
    }
}

__host__ void read_data(std::vector<ll>& arr) {
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

__host__ void validate(std::vector<ll>& arr) {
    ll n = arr.size();
    for (ll i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) {
            std::cout << "error at " << i << " " << arr[i] << " " << arr[i+1] << std::endl;
        }
    }
}

int main() {

    std::vector<ll> arr;
    read_data(arr);
    ll N = arr.size();
    std::cout << "Load count:" << N << std::endl;

    // Allocate and copy array to device
    ll* d_arr;
    cudaMalloc((void**)&d_arr, N * sizeof(ll));
    cudaMemcpy(d_arr, arr.data(), N * sizeof(ll), cudaMemcpyHostToDevice);

    // Call CUDA kernel to perform quicksort
    tap<<<1, 1>>>(d_arr, 0, N-1, -1, -1, 2);
    cudaDeviceSynchronize();

    // Copy back the sorted array
    cudaMemcpy(arr.data(), d_arr, N * sizeof(ll), cudaMemcpyDeviceToHost);
    validate(arr);

    cudaFree(d_arr);

    return 0;
}
