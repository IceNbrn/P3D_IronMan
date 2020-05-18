#include <iostream>
#include <sstream>
#include <vector>


using namespace std;

namespace Utils
{
    vector<string> Split(const string& s, char delim) {
        vector<string> result;
        stringstream ss(s);
        string item;

        while (getline(ss, item, delim)) {
            result.push_back(item);
        }

        return result;
    }
}
