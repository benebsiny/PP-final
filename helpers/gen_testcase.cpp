#include <iostream>
#include <fstream>
#include <random>

typedef long long ll;

int main(int argc, char *argv[])
{
    // 產生器和分佈
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<ll> distribution(0, 0x7fffffff);

    // 產生一千萬個隨機數字
    int totalNumbers = 10000000;
    if (argc == 2)
    {
        totalNumbers = std::stoll(argv[1]);
    }

    std::string filename = "random_numbers_" + std::to_string(totalNumbers) + ".bin";
    std::ofstream outFile(filename, std::ios::binary | std::ios::out);

    if (!outFile.is_open())
    {
        std::cerr << "無法開啟檔案進行寫入。\n";
        return 1;
    }

    for (int i = 0; i < totalNumbers; ++i)
    {
        ll randomNumber = distribution(generator);
        outFile.write(reinterpret_cast<const char *>(&randomNumber), sizeof(ll));
    }

    outFile.close();
    std::cout << "Generated file: " + filename + "\n";

    return 0;
}
