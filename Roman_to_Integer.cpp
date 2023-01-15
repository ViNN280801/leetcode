#include <iostream>
#include <string>
#include <map>
#include <ranges>

class Solution
{
private:
    const std::map<char, int> romanNums{
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}};

public:
    int romanToInt(std::string s)
    {
        if ((s.length() < 1) || (s.length() > 15))
            return 0;

        int res{0};
        auto last{std::ranges::cend(s)};
        for (auto it{std::ranges::cbegin(s)}; it != last; ++it)
        {
            if (((*it == 'I') && (*(it + 1) == 'V') && ((it + 1) < last)) ||
                ((*it == 'I') && (*(it + 1) == 'X') && ((it + 1) < last)))
            {
                res += (romanNums.at(*(it + 1)) - 1);
                ++it;
            }
            else if (((*it == 'X') && (*(it + 1) == 'L') && ((it + 1) < last)) ||
                     ((*it == 'X') && (*(it + 1) == 'C') && ((it + 1) < last)))
            {
                res += (romanNums.at(*(it + 1)) - 10);
                ++it;
            }
            else if (((*it == 'C') && (*(it + 1) == 'D') && ((it + 1) < last)) ||
                     ((*it == 'C') && (*(it + 1) == 'M') && ((it + 1) < last)))
            {
                res += (romanNums.at(*(it + 1)) - 100);
                ++it;
            }
            else
                res += romanNums.at(*it);
        }

        return res;
    }
};

int main()
{
    Solution sln;
    std::cout << sln.romanToInt("III") << std::endl;
    std::cout << sln.romanToInt("LVIII") << std::endl;
    std::cout << sln.romanToInt("MCMXCIV") << std::endl;

    return EXIT_SUCCESS;
}
