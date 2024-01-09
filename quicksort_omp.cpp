#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <cmath>

typedef long long ll;

#define SWAP(x, y) \
    ll tmp = (x);  \
    (x) = (y);     \
    (y) = (tmp);
using namespace std;
using namespace chrono;

int threadCount = 8;
int totalDepth = 3; // log2(8) = 3

// Partition 函數
ll partition(vector<ll> &arr, ll low, ll high)
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

void quickSort(vector<ll> &arr, ll low, ll high, int depth, const int& id)
{
    if (low < high)
    {
        ll pi = partition(arr, low, high);

        if (depth > 0)
        {
            int forkedId = (1 << (totalDepth - depth)) + id;
            if (forkedId < threadCount)  // Create a new thread
            {
#pragma omp task shared(arr)
                quickSort(arr, low, pi - 1, depth - 1, forkedId); // Left part
                quickSort(arr, pi + 1, high, depth - 1, id);// Right part
            }
            else  // It exceeds the number of specified thread count, don't create a new thread
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

bool read_data(vector<ll> &arr)
{
    ifstream inFile("random_numbers.bin", ios::binary | ios::in);

    if (!inFile.is_open())
    {
        return false;
    }

    ll value;
    while (inFile.read(reinterpret_cast<char *>(&value), sizeof(ll)))
    {
        arr.push_back(value);
    }

    inFile.close();

    return true;
}

// If correct, print nothing. Otherwise, print error index and its value
void validate(vector<ll> &arr)
{
    ll n = arr.size();
    for (ll i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            cout << "error at " << i << " " << arr[i] << " " << arr[i + 1] << endl;
        }
    }
}

int main(int argc, char **argv)
{
    int rounds = 1;
    double total_time = 0;
    if (argc == 2)
    {
        rounds = atoi(argv[1]);
    }
    cout << "Run for [" << rounds << "] rounds" << endl;

    threadCount = 8;
    if (argc == 3)
    {
        threadCount = atoi(argv[2]);
    }
    totalDepth = (int)ceil(log2(threadCount));
    cout << "Run for [" << threadCount << "] threads" << endl;

    vector<ll> arr;
    if (!read_data(arr))
    {
        cerr << "Can't read data\n";
        return 1;
    }
    ll n = arr.size();
    cout << "Load count: " << n << endl;

    for (int i = 0; i < rounds; i++)
    {
        vector<ll> temp = vector<ll>(arr);
        auto start_time = high_resolution_clock::now();

#pragma omp parallel
        {
            int rootId = 0;
#pragma omp single
            quickSort(temp, 0, n - 1, totalDepth, rootId);
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        cout << "[Round " << i << "] Execution time: " << duration.count() << " ms" << endl;
        total_time += duration.count();

        validate(temp);
    }

    cout << "Average time: " << total_time / rounds << " ms" << endl;

    return 0;
}