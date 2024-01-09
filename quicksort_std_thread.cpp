#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include "commons/helper.hpp"

int threadCount = 8;
int totalDepth = 3; // log2(8) = 3

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

void quickSort(std::vector<ll> &arr, ll low, ll high, int depth, const int &id)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth > 0)
        {
            int forkedId = (1 << (totalDepth - depth)) + id;
            if (forkedId < threadCount) // Create a new thread
            {
                std::thread left_thread(quickSort, ref(arr), low, pi - 1, depth - 1, std::ref(forkedId));
                quickSort(arr, pi + 1, high, depth - 1, id); // Right part
                left_thread.join();
            }
            else // It exceeds the number of specified thread count, don't create a new thread
            {
                quickSort(arr, low, pi - 1, 0, id);
                quickSort(arr, pi + 1, high, 0, id);
            }
        }
        else
        {
            quickSort(arr, low, pi - 1, 0, id);
            quickSort(arr, pi + 1, high, 0, id);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file> [number of threads]\n";
        return 1;
    }

    std::string filename = argv[1];

    threadCount = 8;
    if (argc == 3)
    {
        threadCount = atoi(argv[2]);
    }
    totalDepth = (int)ceil(log2(threadCount));
    std::cout << "Run for [" << threadCount << "] threads" << std::endl;

    std::vector<ll> arr;
    if (!read_data(arr, filename))
    {
        std::cerr << "[!] Can't read data\n";
        return 1;
    }
    ll n = arr.size();
    std::cout << "[*] Load count: " << n << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    int rootId = 0;
    quickSort(arr, 0, n - 1, totalDepth, rootId);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "[*] Execution time: " << duration.count() << " ms" << std::endl;

    validate(arr);

    return 0;
}
