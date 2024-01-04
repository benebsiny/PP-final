#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <omp.h>

typedef long long ll;

#define SWAP(x, y)  ll tmp = (x); \
                    (x) = (y); \
                    (y) = (tmp);
using namespace std;
using namespace chrono;


// Partition 函數
ll partition(vector<ll>& arr, ll low, ll high) {
    ll pivot = arr[high];  // 選擇最右邊的元素作為 pivot

    // 小於 pivot 的元素將被放在左邊
    // 大於 pivot 的元素將被放在右邊
    ll i = low - 1;

    for (ll j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            SWAP(arr[i], arr[j]);
        }
    }
    SWAP(arr[i + 1], arr[high]);  // 將 pivot 放在正確的位置
    return i + 1; // 回傳 pivot 的位置
}

#define OMP 1

void quickSort(vector<ll>& arr, ll low, ll high, int depth) {
    if (low < high) {
        ll pi = partition(arr, low, high);

        if (depth > 0) {
            #pragma omp task shared(arr)
            quickSort(arr, low, pi - 1, depth-1);   // Left part
            quickSort(arr, pi + 1, high, depth-1);  // Right part
        } else {
            quickSort(arr, low, pi - 1, 0);
            quickSort(arr, pi + 1, high, 0);
        }       
    }
}

void read_data(vector<ll>& arr) {
    ifstream inFile("random_numbers.bin", ios::binary | ios::in);

    if (inFile.is_open()) {
        ll value;
        while (inFile.read(reinterpret_cast<char*>(&value), sizeof(ll))) {
            arr.push_back(value);
        }

        inFile.close();
        cout << "Number read from binary file successfully\n";
    } else {
        cerr << "Can't open random_numbers.bin\n";
    }
}

// If correct, print nothing. Otherwise, print error index and its value
void validate(vector<ll>& arr) {
    ll n = arr.size();
    for (ll i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) {
            cout << "error at " << i << " " << arr[i] << " " << arr[i+1] << endl;
        }
    }
}

int main() {

    const int depth = 8;  // 2 ** 8 = 256 thread

    vector<ll> arr;
    read_data(arr);
    ll n = arr.size();
    cout << "Load count:" << n << endl;

    auto start_time = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        quickSort(arr, 0, n - 1, depth);
    }
    auto end_time = high_resolution_clock::now();
    
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    
    validate(arr);

    return 0;
}
