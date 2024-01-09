#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
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

// Quick Sort 函數
void quickSort(std::vector<ll> &arr, ll low, ll high)
{
    if (low < high)
    {
        // 選擇 pivot 並找到 partition 的位置
        ll pi = partition(arr, low, high);

        // 遞迴地對 pivot 的左右兩邊進行排序
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "[*] Usage: " << argv[0] << " <input file>\n";
        return 1;
    }

    std::string filename = argv[1];

    std::vector<ll> arr;
    if (!read_data(arr, filename))
    {
        std::cerr << "Can't read data\n";
        return 1;
    }
    ll n = arr.size();
    std::cout << "Load count:" << n << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    quickSort(arr, 0, n - 1);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    validate(arr);

    return 0;
}
