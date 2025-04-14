/*
Given an array of integers arr, and three integers a, b and c. You need to find the number of good triplets.
A triplet (arr[i], arr[j], arr[k]) is good if the following conditions are true:
1) 0 <= i < j < k < arr.length
2) |arr[i] - arr[j]| <= a
3) |arr[j] - arr[k]| <= b
4) |arr[i] - arr[k]| <= c

Where |x| denotes the absolute value of x.

Return the number of good triplets.

Example 1:
Input: arr = [3,0,1,1,9,7], a = 7, b = 2, c = 3
Output: 4
Explanation: There are 4 good triplets: [(3,0,1), (3,0,1), (3,1,1), (0,1,1)].

Example 2:
Input: arr = [1,1,2,2,3], a = 0, b = 0, c = 1
Output: 0
Explanation: No triplet satisfies all conditions.

Constraints:
1) 3 <= arr.length <= 100
2) 0 <= arr[i] <= 1000
3) 0 <= a, b, c <= 1000
*/

#include <type_traits>
#include <vector>

class Solution
{
private:
    template <typename T>
    inline constexpr T _min(T a, T b) noexcept
    {
        static_assert(std::is_arithmetic_v<T>,
                      "Type T must be numeric type");
        return a < b ? a : b;
    }

    template <typename T>
    inline constexpr T _max(T a, T b) noexcept
    {
        static_assert(std::is_arithmetic_v<T>,
                      "Type T must be numeric type");
        return a > b ? a : b;
    }

    template <typename T>
    inline constexpr T _abs(T a) noexcept
    {
        static_assert(std::is_arithmetic_v<T>,
                      "Type T must be numeric type");
        return a < 0 ? -a : a;
    }

public:
    // Time complexity: O(n^3)
    // Space complexity: O(1)
    int countGoodTriplets_naive(std::vector<int> &arr, int a, int b, int c)
    {
        /* === Edge cases: === */
        // 1. Array is empty
        // 2. Array has less than 3 elements
        size_t n{arr.size()};
        if (arr.empty() || n < 3)
            return 0;
        /* =================== */

        // Here we can to solve this problem naive way - brute force.
        // Just to use 3 for loops, this is not good, because
        // time complexity will be O(n^3):
        int count{};
        for (size_t i{}; i < n - 2; ++i)
        {
            for (size_t j{i + 1}; j < n - 1; ++j)
            {
                for (size_t k{j + 1}; k < n; ++k)
                {
                    if (_abs(arr[i] - arr[j]) <= a &&
                        _abs(arr[j] - arr[k]) <= b &&
                        _abs(arr[i] - arr[k]) <= c)
                        ++count;
                }
            }
        }
        return count;
    }

    // O(n^3) in real life is very bad approach, so we need to optimize it.
    // Time complexity: O(n^2)
    // Space complexity: O(1)
    int countGoodTriplets(std::vector<int> &arr, int a, int b, int c)
    {
        /* === Edge cases: === */
        // 1. Array is empty
        // 2. Array has less than 3 elements
        size_t n{arr.size()};
        if (arr.empty() || n < 3)
            return 0;
        /* =================== */

        // Main idea: remove 1st loop
        std::vector<int> prefix_count(1001, 0); // Initialize prefix count array
        int count{};

        // Fill prefix count array
        // Time complexity for this loop is O(n)*O(n), because we have 2 nested loops
        for (size_t i{}; i < n - 1; ++i)
        {
            // Time complexity for this loop is O(n)
            for (size_t j{i + 1}; j < n; ++j)
            {
                if (_abs(arr.at(i) - arr.at(j)) <= b)
                {
                    int right{_min(arr.at(i) + a, arr.at(j) + c)},
                        left{_max(arr.at(i) - a, arr.at(j) - c)};

                    left = _max(left, 0);
                    right = _min(right, 1000);

                    if (left <= right)
                        count += prefix_count.at(right) - (left == 0 ? 0 : prefix_count.at(left - 1));
                }
            }
            // Time complexity for this loop is O(1000) -> O(1) because we always iterate from 0 to 1000
            for (int k{arr.at(i)}; k <= 1000; ++k)
                ++prefix_count.at(k);
        }

        return count;
    }
};

#include <gtest/gtest.h>

TEST(CountGoodTriplets, EdgeCase1)
{
    Solution s;
    std::vector<int> arr{};
    int a{7}, b{2}, c{3};
    int expected{0};
    EXPECT_EQ(s.countGoodTriplets_naive(arr, a, b, c), expected);
}

TEST(CountGoodTriplets, EdgeCase2_opt1)
{
    Solution s;
    std::vector<int> arr{1};
    int a{0}, b{0}, c{1};
    int expected{0};
    EXPECT_EQ(s.countGoodTriplets_naive(arr, a, b, c), expected);
}

TEST(CountGoodTriplets, EdgeCase2_opt2)
{
    Solution s;
    std::vector<int> arr{1, 2};
    int a{0}, b{0}, c{1};
    int expected{0};
    EXPECT_EQ(s.countGoodTriplets_naive(arr, a, b, c), expected);
}

TEST(CountGoodTriplets, BaseCase)
{
    Solution s;
    std::vector<int> arr{3, 0, 1, 1, 9, 7};
    int a{7}, b{2}, c{3};
    int expected{4};
    EXPECT_EQ(s.countGoodTriplets_naive(arr, a, b, c), expected);
}

/*
Compile and run:
g++ 1534.\ Count\ Good\ Triplets.cpp  -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out (or ./a.exe on Windows)
*/
