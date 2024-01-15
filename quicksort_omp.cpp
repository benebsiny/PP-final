#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <cmath>
#include "commons/helper.hpp"

ll n;
int threshold = 5;

// Partition 函數
ll partition(std::vector<ll> &arr, ll low, ll high)
{
    ll pivot = arr[high]; // 選擇最右邊的元素作為 pivot

    // 小於 pivot 的元素將被放在左邊
    // 大於 pivot 的元素將被放在右邊
    ll i = low - 1;

    for (ll j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            SWAP(arr[i], arr[j]);
        }
    }
    SWAP(arr[i + 1], arr[high]); // 將 pivot 放在正確的位置
    return i + 1;                // 回傳 pivot 的位置
}

#define OMP 1

void quickSort(std::vector<ll> &arr, ll low, ll high, int depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth > 0)
        {
#pragma omp task shared(arr)
            quickSort(arr, low, pi - 1, depth - 1);  // Left part
            quickSort(arr, pi + 1, high, depth - 1); // Right part
        }
        else
        {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);
        }
    }
}

// Load balance
void quickSort2(std::vector<ll> &arr, ll low, ll high, int depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high); // 選擇 pivot 並找到 partition 的位置

        if (depth > 0)
        {
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
#pragma omp task shared(arr)
                quickSort2(arr, pi + 1, high, depth - 1); // Right part
                quickSort2(arr, low, pi - 1, depth - 1);  // Left part
            }
        }
        else
        {
            quickSort2(arr, low, pi - 1, 0);
            quickSort2(arr, pi + 1, high, 0);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file> [number of threads] [threshold]\n";
        return 1;
    }

    std::string filename = argv[1];

    const int depth = 8; // 2 8 = 256 thread

    if (argc >= 3)
    {
        int threads = atoi(argv[2]);
        if (threads != 0) {
            omp_set_dynamic(0);
            omp_set_num_threads(threads);
        }
    }

    bool loadBalance = false;
    if (argc >= 4)
    {
        threshold = atoi(argv[3]);
        loadBalance = true;
    }
    std::cout << "Run " << ((loadBalance) ? "with" : "without") << " load balance.\n";

    std::vector<ll> arr;
    if (!read_data(arr, filename))
    {
        std::cerr << "Can't read data\n";
        return 1;
    }
    n = arr.size();
    std::cout << "Load count: " << n << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    if (loadBalance)
    {
#pragma omp parallel
        {
#pragma omp single
            quickSort2(arr, 0, n - 1, depth);
        }
    }
    else
    {
#pragma omp parallel
        {
#pragma omp single
            quickSort(arr, 0, n - 1, depth);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    validate(arr);

    return 0;
}