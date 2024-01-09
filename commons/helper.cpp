#include "helper.hpp"

bool read_data(std::vector<long long> &arr, std::string filename)
{
    std::ifstream inFile(filename, std::ios::binary | std::ios::in);

    if (!inFile.is_open())
    {
        return false;
    }

    long long value;
    while (inFile.read(reinterpret_cast<char *>(&value), sizeof(long long)))
    {
        arr.push_back(value);
    }

    inFile.close();

    return true;
}

void validate(std::vector<long long> &arr)
{
    long long n = arr.size();
    for (long long i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            std::cout << "error at " << i << " " << arr[i] << " " << arr[i + 1] << std::endl;
        }
    }
}