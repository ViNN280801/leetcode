/*
Given an integer array nums and an integer k, return the number of good subarrays of nums.
A subarray arr is good if there are at least k pairs of indices (i, j)
such that i < j and arr[i] == arr[j].
A subarray is a contiguous non-empty sequence of elements within an array.

Example 1:
Input: nums = [1,1,1,1,1], k = 10
Output: 1
Explanation: The only good subarray is the array nums itself.

Example 2:
Input: nums = [3,1,4,3,2,2,4], k = 2
Output: 4
Explanation: There are 4 different good subarrays:
- [3,1,4,3,2,2] that has 2 pairs.
- [3,1,4,3,2,2,4] that has 3 pairs.
- [1,4,3,2,2,4] that has 2 pairs.
- [4,3,2,2,4] that has 2 pairs.

Constraints:
1) 1 <= nums.length <= 10^5
2) 1 <= nums[i], k <= 10^9
*/

#include <unordered_map>
#include <vector>

class Solution
{
public:
    long long countGood(std::vector<int> &nums, int k)
    {
        // Get the size of the array
        size_t n{nums.size()};

        /* === Edge cases === */
        // 1. Empty array
        // 2. k is less or equal to 0
        // 3. |arr| < 2
        // Best case: O(1)
        if (n == 0 ||
            k <= 0 ||
            n < 2)
            return 0;
        /* ================== */

        /* === Main logic === */
        // Worst case:
        // Time complexity: O(n)
        // Space complexity: O(n)
        // We can use sliding window to find the number of good subarrays
        long long count{};
        std::unordered_map<int, int> freq;

        // Iterate through the array using left pointer
        for (size_t i{}, left{}; i < n; ++i)
        {
            // Add the current element to the frequency map
            k -= freq[nums.at(i)]++;

            // If the number of pairs is greater than or equal to k, move the left pointer
            while (k <= 0)
                k += --freq[nums.at(left++)];

            // Add the number of good subarrays to the count
            count += left;
        }
        return count;
    }
};

#include <gtest/gtest.h>

TEST(CountGoodSubarrays, EdgeCases)
{
    Solution s;

    // 1. Empty array
    std::vector<int> arr1{};
    EXPECT_EQ(s.countGood(arr1, 1), 0);

    // 2. k is less or equal to 0
    std::vector<int> arr2{1, 2, 3, 4, 5};
    EXPECT_EQ(s.countGood(arr2, 0), 0);

    // 3. |arr| < 2
    std::vector<int> arr3{1};
    EXPECT_EQ(s.countGood(arr3, 1), 0);
}

TEST(CountGoodSubarrays, BasicCases)
{
    Solution s;

    // 1. Basic case 1
    std::vector<int> arr1{1, 1, 1, 1, 1};
    EXPECT_EQ(s.countGood(arr1, 10), 1);

    // 2. Basic case 2
    std::vector<int> arr2{3, 1, 4, 3, 2, 2, 4};
    EXPECT_EQ(s.countGood(arr2, 2), 4);
}

/*
Compile and run:
g++ "2537. Count the Number of Good Subarrays.cpp" -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out (./a.exe on Windows)
*/