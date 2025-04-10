/*
You are given three integers start, finish, and limit. You are also given a 0-indexed string s
representing a positive integer.

A positive integer x is called powerful if it ends with s (in other words, s is a suffix of x)
and each digit in x is at most limit.

Return the total number of powerful integers in the range [start..finish].

A string x is a suffix of a string y if and only if x is a substring of y that starts from some
index (including 0) in y and extends to the index y.length - 1. For example, 25 is a suffix of 5125 whereas 512 is not.

Constraints:
1) 1 <= start <= finish <= 1015
2) 1 <= limit <= 9
3) 1 <= s.length <= floor(log10(finish)) + 1
4) s only consists of numeric digits which are at most limit.
5) s does not have leading zeros.
*/

#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <string_view>

class Solution
{
public:
    inline static constexpr short kdefault_max_digits{20};
    inline static constexpr short kdefault_dim{2};

    // Time complexity: O(S + log(finish))
    // - O(S) for input validation checks involving the suffix string `numStr` (e.g., std::all_of).
    // - O(log(finish)) for std::to_string conversions (number of digits).
    // - O(log(finish)) for each call to _count_valid (see below).
    // Space complexity: O(S + log(finish))
    // - O(S) to store m_suffix.
    // - O(log(finish)) for the DP table and recursion stack depth (see below).
    // - O(log(finish)) to store string representations of finish and start-1.
    long long numberOfPowerfulInt(long long start, long long finish, int limit, std::string_view numStr) noexcept
    {
        // Base cases:
        // Time complexity: O(S), where S = numStr.length(), due to std::all_of.
        // Space complexity: O(1)
        if ((start > finish) ||                                         // 1. start > finish
            (limit < 0) ||                                              // 2. limit is negative
            (numStr.empty()) ||                                         // 3. 's' is empty
            ((numStr.length() > std::floor(std::log10(finish)) + 1)) || // 4. 's' is too long
            (numStr.length() > 1 && numStr[0] == '0') ||                // 5. 's' has leading zeros
            (!std::all_of(numStr.cbegin(), numStr.cend(), [](char c)
                          { return std::isdigit(c); })) ||            // 6. 's' has non-digits
            (!std::all_of(numStr.cbegin(), numStr.cend(), [&](char c) // 7. digits in suffix are not within limit
                          { return (c - '0') <= limit; })))
        {
            return 0;
        }

        m_suffix = numStr;
        m_limit = limit;

        // We can use DP (dynamic programming) to solve this problem

        // Processing digits one by one
        // - If the current position is within the suffix zone, force the digit to match.
        // - Else, choose any digit from '0' to 'limit' (while respecting tight bounds).
        // - Use memoization to store the results of subproblems to avoid recalculating them.

        // To count within the range [start..finish]:
        // 1. Count valid numbers from 1 to finish.
        // 2. Count valid numbers from 1 to start - 1.
        // 3. Subtract the two to get the count in [start, finish].

        long long suffix_ll{std::stoll(m_suffix)};
        if (suffix_ll > finish)
            return 0;

        long long countToStart{(start > suffix_ll) ? _count_valid(std::to_string(start - 1)) : 0},
            countToFinish{_count_valid(std::to_string(finish))};

        return countToFinish - countToStart;
    }

private:
    std::array<std::array<long long, kdefault_dim>, kdefault_max_digits> m_dp;
    std::string m_suffix;
    int m_limit;

    // Time complexity: O(...)
    // Space complexity: O(...)
    long long _dfs(std::string_view numStr, size_t pos, bool tight) noexcept
    {
        if (pos == numStr.length())
            return 1;

        if (m_dp.at(pos).at(tight) != -1)
            return m_dp.at(pos).at(tight);

        long long res{};
        short maxDigit{static_cast<short>(tight ? numStr.at(pos) - '0' : 9)};
        size_t suffixStart{numStr.length() - m_suffix.size()};

        if (pos >= suffixStart)
        {
            size_t idx{pos - suffixStart};
            short digit{static_cast<short>(m_suffix.at(idx) - '0')};

            if (digit <= maxDigit && digit <= m_limit)
            {
                res += _dfs(numStr, pos + 1, tight && (digit == maxDigit));
            }
        }
        else
        {
            for (short d{}; d <= maxDigit && d <= m_limit; ++d)
            {
                res += _dfs(numStr, pos + 1, tight && (d == maxDigit));
            }
        }

        return m_dp.at(pos).at(tight) = res;
    }

    long long _count_valid(std::string_view numStr) noexcept
    {
        for (auto &row : m_dp)
            row.fill(-1);

        return _dfs(numStr, 0, true);
    }
};

#include <gtest/gtest.h>

TEST(Solution, Test1)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 6000, 4, "124"), 5);
}

TEST(Solution, Test2)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(15, 215, 6, "10"), 2);
}

TEST(Solution, Test3)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1000, 2000, 4, "3000"), 0);
}

TEST(Solution, BaseCase1)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(100, 1, 1, "1"), 0);
}

TEST(Solution, BaseCase2)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 10, -23091, "1"), 0);
}

TEST(Solution, BaseCase3)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 10, 1, ""), 0);
}

TEST(Solution, BaseCase4)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 10, 1, "3457893405728903483450394853035375875348954353490583"), 0);
}

TEST(Solution, BaseCase5)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 1015, 3, "0125"), 0);
}

TEST(Solution, BaseCase6)
{
    Solution solution;
    ASSERT_EQ(solution.numberOfPowerfulInt(1, 1015, 3, "01a25"), 0);
}

/*
Compile and run:
g++ -Wall -Wpedantic -Wextra 2999.\ Count\ the\ Number\ of\ Powerful\ Integers.cpp -lgtest -lgtest_main
./a.out
*/
