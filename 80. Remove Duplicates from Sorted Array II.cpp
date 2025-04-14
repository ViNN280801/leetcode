#include <vector>

class Solution
{
public:
    int removeDuplicates(std::vector<int> &nums)
    {
        // Base cases:
        // 1. Empty array
        if (nums.empty())
            return 0;

        // 2. Only 1 element
        if (nums.size() == 1)
            return 1;

        // 3. Two different elements or two same elements
        if (nums.size() == 2)
            return 2;

        // General case: Use two pointers to find unique elements
        // Time complexity: O(n)
        // Space complexity: O(1)

        // This variable will keep track of the current position in the modified nums vector
        size_t i{};
        for (int num : nums)
        {
            // Checking only the first two elements and the elements two positions before
            // the current position
            if (i == 0 || i == 1 || nums.at(i - 2) != num)
                nums.at(i++) = num;
        }
        return i;
    }
};

#include <gtest/gtest.h>

TEST(removeDuplicates, emptyArray)
{
    Solution solution;
    std::vector<int> nums = {};
    EXPECT_EQ(solution.removeDuplicates(nums), 0);
}

TEST(removeDuplicates, singleElement)
{
    Solution solution;
    std::vector<int> nums = {1};
    EXPECT_EQ(solution.removeDuplicates(nums), 1);
}

TEST(removeDuplicates, twoDifferentElements)
{
    Solution solution;
    std::vector<int> nums = {1, 2};
    EXPECT_EQ(solution.removeDuplicates(nums), 2);
}

TEST(removeDuplicates, twoSameElements)
{
    Solution solution;
    std::vector<int> nums = {1, 1};
    EXPECT_EQ(solution.removeDuplicates(nums), 2);
}

TEST(removeDuplicates, fiveElements)
{
    Solution solution;
    std::vector<int> nums = {1, 1, 1, 2, 2, 3};
    EXPECT_EQ(solution.removeDuplicates(nums), 5);
}

TEST(removeDuplicates, sevenElements)
{
    Solution solution;
    std::vector<int> nums = {0, 0, 1, 1, 1, 1, 2, 3, 3};
    EXPECT_EQ(solution.removeDuplicates(nums), 7);
}

/*
Compile and run:
g++ '.\80. Remove Duplicates from Sorted Array II.cpp' -Wall -Wpedantic -Wextra -lgtest -lgtest_main
./a.out (or .\a.exe on Windows)
*/
