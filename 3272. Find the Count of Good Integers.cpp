/*
You are given two positive integers n and k.

An integer x is called k-palindromic if:
- x is a palindrome.
- x is divisible by k.

An integer is called good if its digits can be rearranged to form a k-palindromic integer.
For example, for k = 2, 2020 can be rearranged to form the k-palindromic
integer 2002, whereas 1010 cannot be rearranged to form a k-palindromic integer.

Return the count of good integers containing n digits.

Note that any integer must not have leading zeros, neither
before nor after rearrangement. For example, 1010 cannot be rearranged to form 101.

Example 1:
Input: n = 3, k = 5
Output: 27
Explanation:
Some of the good integers are:
- 551 because it can be rearranged to form 515.
- 525 because it is already k-palindromic.

Example 2:
Input: n = 1, k = 4
Output: 2
Explanation:
The two good integers are 4 and 8.

Example 3:
Input: n = 5, k = 6
Output: 2468

Constraints:
1) 1 <= n <= 10
2) 1 <= k <= 9
*/

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_set>

class Solution
{
public:
    long long countGoodIntegers(int n, int k)
    {
        // Set to store all unique digit combinations of k-palindromic integers
        std::unordered_set<std::string> validDigitPermutations;

        // Base represents the range [base, base * 10)
        // which generates the left half of the palindromic number
        int base{static_cast<int>(std::pow(10, (n - 1) / 2))},
            skipMiddle{n % 2};

        // Enumerate all palindromic integers of length n
        for (int leftHalf{base}; leftHalf < base * 10; ++leftHalf)
        {
            std::string leftStr(std::to_string(leftHalf)),
                fullStr(leftStr + std::string(leftStr.rbegin() + skipMiddle, leftStr.rend()));

            long long palindromicInt{std::stoll(fullStr)};

            // Check if this palindromic integer is divisible by k
            if (palindromicInt % k == 0)
            {
                // Sort the digits to get a canonical form for hash set
                std::sort(fullStr.begin(), fullStr.end());
                validDigitPermutations.insert(fullStr);
            }
        }

        // Precompute factorials up to n
        std::vector<long long> factorial(n + 1, 1);
        for (int i{1}; i <= n; ++i)
            factorial[i] = factorial[i - 1] * i;

        long long result_count{};

        // For each valid digit set, count the number of unique permutations
        for (const std::string &digits : validDigitPermutations)
        {
            std::vector<int> freq(10, 0);

            // Count occurrences of each digit
            for (char ch : digits)
                ++freq[ch - '0'];

            // Count permutations that don't start with zero
            long long total{(n - freq[0]) * factorial[n - 1]};

            // Divide by factorials of repeating digits
            for (int count : freq)
                total /= factorial[count];

            result_count += total;
        }

        return result_count;
    }
};

#include <gtest/gtest.h>

TEST(Solution, countGoodIntegers)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodIntegers(3, 5), 27);
}

TEST(Solution, countGoodIntegers2)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodIntegers(1, 4), 2);
}

TEST(Solution, countGoodIntegers3)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodIntegers(5, 6), 2468);
}

/*
Compile and run:
g++ '.\3272. Find the Count of Good Integers.cpp' -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out (or .\a.exe on Windows)
*/
