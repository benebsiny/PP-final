#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstring>
#include "commons/helper.hpp"

int threadCount = 8;
int totalDepth = 3; // log2(8) = 3
ll n = 0;

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

// Load balance
void quickSort2(std::vector<ll> &arr, ll low, ll high, int depth)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high); // 選擇 pivot 並找到 partition 的位置

        if (depth > 0)
        {
            if (pi - low <= n / 5)
            {
                // std::thread left_thread(quickSort2, ref(arr), low, pi - 1, 0); // Left part
                quickSort2(arr, low, pi-1, 0);
                quickSort2(arr, pi + 1, high, depth);                          // Right part
                // left_thread.join();
            }
            else if (high - pi <= n / 5)
            {
                // std::thread right_thread(quickSort2, ref(arr), pi + 1, high, 0); // Right part
                quickSort2(arr, pi + 1, high, 0);
                
                quickSort2(arr, low, pi - 1, depth);                             // Left part
                // right_thread.join();
            }
            else
            {
                std::thread right_thread(quickSort2, ref(arr), pi + 1, high, depth - 1); // Right part
                quickSort2(arr, low, pi - 1, depth - 1);                                 // Left part
                right_thread.join();
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
        std::cerr << "[*] Usage: " << argv[0] << " <input file> [number of threads] -b\n";
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

    bool loadBalance = false;
    if (argc == 4 && !strcmp(argv[3], "-b"))
    {
        loadBalance = true;
    }
    std::cout << "Run " << ((loadBalance) ? "with" : "without") << " load balance\n";

    std::vector<ll> arr;
    if (!read_data(arr, filename))
    {
        std::cerr << "[!] Can't read data\n";
        return 1;
    }
    n = arr.size();
    std::cout << "[*] Load count: " << n << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    int rootId = 0;
    if (loadBalance)
    {
        quickSort2(arr, 0, n - 1, totalDepth);
    }
    else
    {
        quickSort(arr, 0, n - 1, totalDepth, rootId);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "[*] Execution time: " << duration.count() << " ms" << std::endl;

    validate(arr);

    return 0;
}
