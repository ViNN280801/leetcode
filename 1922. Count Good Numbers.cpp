/*
A digit string is good if the digits (0-indexed) at even indices
are even and the digits at odd indices are prime (2, 3, 5, or 7).

For example, "2582" is good because the digits (2 and 8) at
even positions are even and the digits (5 and 2) at odd positions are prime.
However, "3245" is not good because 3 is at an even index but is not even.

Given an integer n, return the total number of good digit strings of length n.
Since the answer may be large, return it modulo 10^9 + 7.

A digit string is a string consisting of digits 0 through 9 that may contain leading zeros.

Example 1:
Input: n = 1
Output: 5
Explanation: The good numbers of length 1 are "0", "2", "4", "6", "8".

Example 2:
Input: n = 4
Output: 400

Example 3:
Input: n = 50
Output: 564908303

Constraints:
1) 1 <= n <= 10^15
*/

class Solution
{
private:
    // Binary exponentiation: https://cp-algorithms.com/algebra/binary-exp.html
    long long _mod_pow(long long base, long long exp, long long mod) noexcept
    {
        // Time complexity: O(log(exp)), because we divide exp by 2 in each iteration
        // Space complexity: O(1), because we don't use any extra space
        long long result{1};
        while (exp > 0)
        {
            if (exp & 1) // If exp is odd
                result = (result * base) % mod;

            base = (base * base) % mod;
            exp >>= 1; // Divide exp by 2
        }
        return result;
    }

public:
    inline static constexpr long long MOD{1'000'000'007ll};

    int countGoodNumbers(long long n) noexcept
    {
        /* ==== Edge cases: ==== */
        // 1) n < 0
        // 2) n = 0
        if (n < 0 || n == 0)
            return 0;

        // 3) n = 1
        if (n == 1)
            return 5; // Because there are 5 even digits (0, 2, 4, 6, 8)
        /* ===================== */

        /* ==== Main logic: ==== */
        // Let k be the number of even positions, i.e. positions with indices 0, 2, 4, 6, 8 => 5
        // Let m be the number of odd positions, i.e. positions with indices 1, 3, 5, 7, 9
        // On each even position can be one of 5 digits (0, 2, 4, 6, 8)
        // On each odd position can be one of 4 digits (2, 3, 5, 7)
        // Then the total number of permutations is 5^k * 4^m

        // Even positions: k = (n + 1) / 2
        // Odd positions: m = n / 2
        // For example n=5: available positions 0, 1, 2, 3, 4
        //               even: 0, 2, 4
        //               odd: 1, 3
        //               k = (5 + 1) / 2 = 3
        //               m = 5 / 2 = 2
        // Total number of permutations: 5^3 * 4^2 = 125 * 16 = 2000
        // We can use binary exponentiation to calculate 5^k and 4^m modulo MOD
        // Because MOD is prime, we can use Fermat's little theorem (https://en.wikipedia.org/wiki/Fermat%27s_little_theorem)
        // to calculate modular inverses
        // a^(p-1) = 1 (mod p) => a^(p-2) = a^(-1) (mod p)
        // So, 5^(-1) = 5^(MOD-2) (mod MOD) and 4^(-1) = 4^(MOD-2) (mod MOD)
        // We can use binary exponentiation to calculate 5^(MOD-2) and 4^(MOD-2) modulo MOD
        /* ===================== */

        long long k{(n + 1) / 2},
            m{n / 2};

        // Time complexity: O(log(k) + log(m)), because we use binary exponentiation
        // Space complexity: O(1), because we don't use any extra space
        return (_mod_pow(5, k, MOD) * _mod_pow(4, m, MOD)) % MOD;
    }
};

#include <gtest/gtest.h>

TEST(countGoodNumbers, EdgeCase_n_is_negative)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodNumbers(-1), 0);
}

TEST(countGoodNumbers, EdgeCase_n_is_0)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodNumbers(0), 0);
}

TEST(countGoodNumbers, EdgeCase_n_is_1)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodNumbers(1), 5);
}

TEST(countGoodNumbers, Example_n_is_4)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodNumbers(4), 400);
}

TEST(countGoodNumbers, Example_n_is_50)
{
    Solution solution;
    EXPECT_EQ(solution.countGoodNumbers(50), 564908303ll);
}

/*
g++ '.\1922. Count Good Numbers.cpp' -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out (or ./a.exe on Windows)
*/
