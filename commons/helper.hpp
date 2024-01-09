#include <vector>
#include <string>
#include <fstream>
#include <iostream>

typedef long long ll;

#define SWAP(x, y) \
    ll tmp = (x);  \
    (x) = (y);     \
    (y) = (tmp);

bool read_data(std::vector<long long> &arr, std::string filename);

// If correct, print nothing. Otherwise, print error index and its value
void validate(std::vector<long long> &arr);