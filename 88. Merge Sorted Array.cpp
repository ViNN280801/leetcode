/*
You are given two integer arrays nums1 and nums2, sorted in non-decreasing order,
and two integers m and n, representing the number of elements in nums1 and nums2 respectively.

Merge nums1 and nums2 into a single array sorted in non-decreasing order.

The final sorted array should not be returned by the function,
but instead be stored inside the array nums1. To accommodate this,
nums1 has a length of m + n, where the first m elements denote the
elements that should be merged, and the last n elements are set to 0
and should be ignored. nums2 has a length of n.

Example 1:
Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
Output: [1,2,2,3,5,6]
Explanation: The arrays we are merging are [1,2,3] and [2,5,6].
The result of the merge is [1,2,2,3,5,6] with the underlined elements coming from nums1.

Example 2:
Input: nums1 = [1], m = 1, nums2 = [], n = 0
Output: [1]
Explanation: The arrays we are merging are [1] and [].
The result of the merge is [1].

Example 3:
Input: nums1 = [0], m = 0, nums2 = [1], n = 1
Output: [1]
Explanation: The arrays we are merging are [] and [1].
The result of the merge is [1].
Note that because m = 0, there are no elements in nums1. The 0 is only there to ensure
the merge result can fit in nums1.

Constraints:
1) nums1.length == m + n
2) nums2.length == n
3) 0 <= m, n <= 200
4) 1 <= m + n <= 200
5) -10^9 <= nums1[i], nums2[j] <= 10^9

Follow up: Can you come up with an algorithm that runs in O(m + n) time?
*/

#include <algorithm>
#include <vector>

class Solution
{
public:
    void merge_non_interesting(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
    {
        // Of course, you can simply merge two arrays and sort them, but this is not interesting
        // and this is not the goal of solving the problem, this shows how I can use STL.
        // But, this solution gives the best result on leetcode: 0ms, 12.35Mb.
        for (int i{m}; i < m + n; ++i)
            nums1.at(i) = nums2.at(i - m);
        std::sort(nums1.begin(), nums1.end());
    }

    void merge(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
    {
        // Edge cases:
        // 1. nums1 is empty, nums2 is not empty
        if (nums1.empty() && !nums2.empty())
        {
            // Iterating over nums2 and emplacing its elements into nums1
            nums1.resize(n);
            for (int i{}; i < n; ++i)
                nums1.at(i) = nums2.at(i);
            return;
        }

        // 2. nums1 and nums2 are empty
        // 3. m or n < 0
        // 4. Size of nums1 != m + n
        // 5. Size of nums2 != n
        // 6. nums1 is not empty, nums2 is empty
        if ((nums1.empty() && nums2.empty()) ||
            (m < 0 || n < 0) ||
            (nums1.size() != static_cast<size_t>(m + n)) ||
            (nums2.size() != static_cast<size_t>(n)) ||
            (!nums1.empty() && nums2.empty()))
            return;

        int i{m - 1}, j{n - 1}, k{m + n - 1};
        while (i >= 0 && j >= 0)
        {
            if (nums1.at(i) < nums2.at(j))
            {
                nums1.at(k) = nums2.at(j);
                --j;
                --k;
            }
            else
            {
                nums1.at(k) = nums1.at(i);
                --i;
                --k;
            }
        }
        while (j >= 0)
        {
            nums1.at(k--) = nums2.at(j--);
        }
    }
};

#include <gtest/gtest.h>

TEST(MergeSortedArray, EdgeCase1)
{
    // 1. nums1 and nums2 are empty
    Solution solution;
    std::vector<int> nums1{};
    std::vector<int> nums2{};
    solution.merge(nums1, 0, nums2, 0);
    EXPECT_EQ(nums1, std::vector<int>{});
}

TEST(MergeSortedArray, EdgeCase2)
{
    // 2. m or n < 0
    Solution solution;
    std::vector<int> nums1{};
    std::vector<int> nums2{};
    solution.merge(nums1, -1, nums2, 0);
    EXPECT_EQ(nums1, std::vector<int>{});
}

TEST(MergeSortedArray, EdgeCase3)
{
    // 3. Size of nums1 != m + n
    Solution solution;
    std::vector<int> nums1{1, 2, 3};
    std::vector<int> nums2{4, 5, 6};
    solution.merge(nums1, 3, nums2, 3);
    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, EdgeCase4)
{
    // 4. Size of nums2 != n
    Solution solution;
    std::vector<int> nums1{1, 2, 3};
    std::vector<int> nums2{4, 5, 6, 7, 8, 9};
    solution.merge(nums1, 3, nums2, 4);
    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, EdgeCase5)
{
    // 5. nums1 is not empty, nums2 is empty
    Solution solution;
    std::vector<int> nums1{1, 2, 3};
    std::vector<int> nums2{};
    solution.merge(nums1, 3, nums2, 0);
    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, EdgeCase6)
{
    // 6. nums1 is empty, nums2 is not empty
    Solution solution;
    std::vector<int> nums1{};
    std::vector<int> nums2{1, 2, 3};
    solution.merge(nums1, 0, nums2, 3);
    std::vector<int> expected{1, 2, 3};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, Test1)
{
    // Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
    // Output: [1,2,2,3,5,6]
    Solution solution;
    std::vector<int> nums1{1, 2, 3, 0, 0, 0};
    std::vector<int> nums2{2, 5, 6};
    solution.merge(nums1, 3, nums2, 3);
    std::vector<int> expected{1, 2, 2, 3, 5, 6};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, Test2)
{
    // Input: nums1 = [1], m = 1, nums2 = [], n = 0
    // Output: [1]
    Solution solution;
    std::vector<int> nums1{1};
    std::vector<int> nums2{};
    solution.merge(nums1, 1, nums2, 0);
    std::vector<int> expected{1};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, Test3)
{
    // Input: nums1 = [0], m = 0, nums2 = [1], n = 1
    // Output: [1]
    Solution solution;
    std::vector<int> nums1{0};
    std::vector<int> nums2{1};
    solution.merge(nums1, 0, nums2, 1);
    std::vector<int> expected{1};
    EXPECT_EQ(nums1, expected);
}

TEST(MergeSortedArray, Test4)
{
    // Input: nums1 = [1,2,3,0,0,0], m = 3, nums2 = [-1,2,3], n = 3
    // Output: [-1,1,2,2,3,3]
    Solution solution;
    std::vector<int> nums1{1, 2, 3, 0, 0, 0};
    std::vector<int> nums2{-1, 2, 3};
    solution.merge(nums1, 3, nums2, 3);
    std::vector<int> expected{-1, 1, 2, 2, 3, 3};
    EXPECT_EQ(nums1, expected);
}

/*
g++ '.\88. Merge Sorted Array.cpp' -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out
*/
