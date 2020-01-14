#include <Util/Util.hpp>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <random>
#include <limits>
using namespace std;

namespace {
std::random_device rngdev;
std::mt19937 rng(rngdev());
}

int randomInt(int mn, int mx) {
	if (mn>mx)
		std::swap(mn,mx);
    std::uniform_int_distribution<std::mt19937::result_type> dist(mn,mx);
	return dist(rng);
}

float randomFloat(float mn, float mx) {
    if (mn > mx)
        std::swap(mn, mx);
    const int mr = std::numeric_limits<int>::max() - 1;
    const float r = randomInt(0, mr);
    const float scaled = r / static_cast<float>(mr) * (mx - mn);
    return mn + scaled;
}

string intToString(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

string doubleToString(double d)
{
    stringstream ss;
    ss << d;
    return ss.str();
}

int stringToInt(const string& s)
{
    return stoi(s.c_str());
}

double stringToDouble(const string& s)
{
    return stod(s.c_str());
}
