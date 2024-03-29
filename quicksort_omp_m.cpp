#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <cmath>
#include "commons/helper.hpp"

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
    for (int i = 0; i < divided; ++i)
    {
        std::pair<ll, int> x{arr[i * interval], i * interval};
        int j{i - 1};
        for (; j >= 0 && pq[j] > x; --j)
            pq[j + 1] = pq[j];
        pq[j + 1] = x;
    }

    while (!pq.empty())
    {
        auto x = pq[0];
        res.push_back(x.first);
        int i = x.second + 1;
        if (i % interval == 0)
        {
            for (long unsigned int j = 1; j < pq.size(); ++j)
                pq[j - 1] = pq[j];
            pq.pop_back();
        }
        else
        {
            x = {arr[i], i};
            long unsigned int j{1};
            for (; j < pq.size() && pq[j] < x; ++j)
                pq[j - 1] = pq[j];
            pq[j - 1] = x;
        }
    }

    arr = move(res);
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file> [number of threads]\n";
        return 1;
    }

    std::string filename = argv[1];

    // const int depth = 8; // 2 8 = 256 thread
    int threadCount = 8;
    if (argc == 3)
    {
        threadCount = atoi(argv[2]);
        omp_set_dynamic(0);
        omp_set_num_threads(threadCount);
    }

    std::vector<ll> arr;
    if (!read_data(arr, filename))
    {
        std::cerr << "Can't read data\n";
        return 1;
    }
    ll n = arr.size();
    std::cout << "Load count: " << n << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    ll interval = n / threadCount;
#pragma omp parallel for
    for (int i = 0; i < threadCount; i++)
    {
        quickSort(arr, i * interval, (i + 1) * interval - 1);
    }
    XDEv11_merge(arr, threadCount);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    validate(arr);

    return 0;
}