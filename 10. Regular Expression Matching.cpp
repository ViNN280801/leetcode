/* Given an input string s and a pattern p, implement regular expression matching with support for '.' and '*' where:

    '.' Matches any single character.​​​​
    '*' Matches zero or more of the preceding element.

The matching should cover the entire input string (not partial).

Example 1:

Input: s = "aa", p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".

Example 2:

Input: s = "aa", p = "a*"
Output: true
Explanation: '*' means zero or more of the preceding element, 'a'. Therefore, by repeating 'a' once, it becomes "aa".

Example 3:

Input: s = "ab", p = ".*"
Output: true
Explanation: ".*" means "zero or more (*) of any character (.)".

Constraints:
    1 <= s.length <= 20
    1 <= p.length <= 20
    s contains only lowercase English letters.
    p contains only lowercase English letters, '.', and '*'.
    It is guaranteed for each appearance of the character '*', there will be a previous valid character to match.
*/

#include <array>
#include <string>
#include <string_view>

class Solution
{
public:
    bool isMatch(std::string_view s, std::string_view p)
    {
        // Edge cases
        // .empty() has O(1) complexity because size is cached
        if (s.empty() && p.empty())
            return true;

        // 21 because of task constraints (see constraints above).
        // 21 is the maximum size of the matrix that can be created.
        // 441 cells in the matrix. 21x21. Because if we will have 20x20,
        // we got *** stack smashing detected ***: terminated, because of dp[21][21] access.
        // Space complexity: O(n * m), because we use a matrix to store the results
        // O(21 * 21) = O(1)
        constexpr int n{21}, m{21};
        std::array<std::array<bool, m>, n> dp{};

        // S and P are the sizes of the string and pattern correspondingly.
        int S{static_cast<int>(s.size())},
            P{static_cast<int>(p.size())};

        // Filling matrix from bottom to top and right to left
        dp[S][P] = true; // Empty pattern matches empty string

        // Time complexity: O(S * P), because we iterate through the s.size() and p.size()
        for (int i{S}; i >= 0; --i)
        {
            for (int j{P - 1}; j >= 0; --j)
            {
                bool first_match{(i < S) && (s[i] == p[j] || p[j] == '.')};

                // If the next character is '*', we have two options:
                // 1. Skip the current character and the '*', and check the next character.
                // 2. If the current character matches, we can repeat it as many times as we want.
                if (j + 1 < P && p[j + 1] == '*')
                    dp[i][j] = dp[i][j + 2]                      // 0 repetitions of current character
                               || (first_match && dp[i + 1][j]); // 1+ repetitions of current character
                else
                    dp[i][j] = first_match && dp[i + 1][j + 1];
            }
        }

        // dp[0][0] is the result of the entire string and pattern.
        // Because we filled the matrix from bottom to top and right to left,
        // we know that dp[0][0] is the result of the entire string and pattern.
        return dp[0][0];
    }
};

#include <gtest/gtest.h>

TEST(RegularExpressionMatching, BasicTests)
{
    EXPECT_FALSE(Solution().isMatch("aa", "a"));
    EXPECT_TRUE(Solution().isMatch("aa", "a*"));
    EXPECT_TRUE(Solution().isMatch("ab", ".*"));
    EXPECT_TRUE(Solution().isMatch("aab", "c*a*b"));
    EXPECT_FALSE(Solution().isMatch("mississippi", "mis*is*p*"));
    EXPECT_TRUE(Solution().isMatch("", ""));
    EXPECT_TRUE(Solution().isMatch("", "a*"));
    EXPECT_FALSE(Solution().isMatch("a", ""));
    EXPECT_TRUE(Solution().isMatch("abc", "a.c"));
    EXPECT_FALSE(Solution().isMatch("abc", "a.d"));
    EXPECT_TRUE(Solution().isMatch("aaa", "a*a"));
    EXPECT_TRUE(Solution().isMatch("aaa", "ab*a*c*a"));
    EXPECT_FALSE(Solution().isMatch("aaa", "ab*a"));
    EXPECT_TRUE(Solution().isMatch("", "c*c*"));
    EXPECT_FALSE(Solution().isMatch("a", ".*..a*"));
    EXPECT_FALSE(Solution().isMatch("a", "ab*a"));
    EXPECT_FALSE(Solution().isMatch("a", "ab*a*c*a"));
    EXPECT_TRUE(Solution().isMatch("a", "a*a"));
    EXPECT_FALSE(Solution().isMatch("a", "a*b"));
    EXPECT_TRUE(Solution().isMatch("a", "a*b*"));
    EXPECT_TRUE(Solution().isMatch("aaaaaaaaaa", ".........."));
    EXPECT_TRUE(Solution().isMatch("aaaaaaaaaa", "a*"));
    EXPECT_TRUE(Solution().isMatch("ablkaaac", ".*blka*."));
}

// How to compile and run?
/*
    g++ -Wall -Wpedantic -Wextra 10.\ Regular\ Expression\ Matching.cpp -lgtest -lgtest_main
    ./a.out
*/
