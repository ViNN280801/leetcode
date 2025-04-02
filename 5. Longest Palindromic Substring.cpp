#include <string>

class Solution
{
private:
    int _expandAroundCenter(std::string const &s, int left, int right)
    {
        while (left >= 0 &&
               right < static_cast<int>(s.length()) &&
               s.at(left) == s.at(right))
        {
            --left;
            ++right;
        }
        return right - left - 1;
    }

public:
    std::string longestPalindrome(std::string const &s)
    {
        int n{static_cast<int>(s.length())};
        if (n < 2)
            return s;
        if (n == 2 && s.front() == s.back())
            return s;
        if (n == 2 && s.front() != s.back())
            return s.substr(0, 1);

        int left{}, right{};
        for (int i{}; i < n; ++i)
        {
            int odd{_expandAroundCenter(s, i, i)},
                even{_expandAroundCenter(s, i, i + 1)},
                max_len{odd > even ? odd : even};

            if (max_len > right - left)
            {
                left = i - (max_len - 1) / 2;
                right = i + max_len / 2;
            }
        }
        return s.substr(left, right - left + 1);
    }
};

#include <iostream>

int main()
{
    Solution s;
    std::cout << s.longestPalindrome("ccd") << std::endl;
}
