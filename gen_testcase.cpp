#include <iostream>
#include <fstream>
#include <random>

using namespace std;
typedef long long ll;

int main(int argc, char* argv[]) {
    // 產生器和分佈
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<ll> distribution(0, 0x7fffffff);

    // 產生一千萬個隨機數字
    int totalNumbers = 10000000;
    if (argc == 2) {
        totalNumbers = stoll(argv[1]);
    }

    ofstream outFile("random_numbers.bin", ios::binary | ios::out);

    if (outFile.is_open()) {
        for (int i = 0; i < totalNumbers; ++i) {
            ll randomNumber = distribution(generator);
            outFile.write(reinterpret_cast<const char*>(&randomNumber), sizeof(ll));
        }

        outFile.close();
        cout << "成功產生" << totalNumbers << "個隨機數字並以二進制格式寫入檔案。\n";
    } else {
        cerr << "無法開啟檔案進行寫入。\n";
        return 1;
    }

    return 0;
}
