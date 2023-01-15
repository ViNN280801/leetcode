// Task: Given a signed 32-bit integer x, return x with its digits reversed.
// If reversing x causes the value to go outside the signed 32-bit
// integer range [-2^31, 2^31 - 1], then return 0.
// Assume the environment does not allow you to store 64-bit integers (signed or unsigned).

// Example 1:
// Input: x = 123
// Output: 321

// Example 2:
// Input: x = -123
// Output: -321

// Example 3:
// Input: x = 120
// Output: 21

// Constraints:
// -2^31 <= x <= 2^31 - 1

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <iterator>

class Solution
{
protected:
    int handleStoiFunc(const std::string &str) const
    {
        try
        {
            return std::stoi(str);
        }
        catch (const std::exception &e)
        {
            return 0;
        }
    }

    inline bool isInBounds(const int &val) const noexcept
    {
        return ((val >= pow(-2.0, 31.0)) and (val <= (pow(2.0, 31.0) - 1.0)));
    }

public:
    int reverse(int x) const noexcept
    {
        if (not isInBounds(x))
            return 0;

        std::string str(std::to_string(x));
        std::reverse(std::begin(str), std::end(str));

        if (*(std::cend(str) - 1) == '-')
        {
            str.erase(std::cend(str) - 1);
            str.insert(std::cbegin(str), '-');
        }

        return handleStoiFunc(str);
    }
};

int main()
{
    Solution sln;
    std::cout << sln.reverse(123) << std::endl;
    std::cout << sln.reverse(-123) << std::endl;
    std::cout << sln.reverse(120) << std::endl;
    std::cout << sln.reverse(1534236469) << std::endl;

    return EXIT_SUCCESS;
}
