// Given an array of integers nums and an integer target, return
// indices of the two numbers such that they add up to target.

// You may assume that each input would have exactly one solution, and you may not use the same element twice.

// You can return the answer in any order.

// Example 1:
// Input: nums = [2,7,11,15], target = 9
// Output: [0,1]
// Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

// Example 2:
// Input: nums = [3,2,4], target = 6
// Output: [1,2]

// Example 3:
// Input: nums = [3,3], target = 6
// Output: [0,1]

// Constraints:
// 2 <= nums.length <= 10^4
// -10^9 <= nums[i] <= 10^9
// -10^9 <= target <= 10^9
// Only one valid answer exists.

#include <iostream>
#include <vector>
#include <cmath>

class Solution
{
protected:
    inline bool is109Checked(int val) const
    {
        return ((val >= -pow(10, 9)) && (val <= pow(10, 9))) ? true : false;
    }

    inline bool is2_104Checked(int val) const
    {
        return ((val >= 2) && (val <= pow(10, 4))) ? true : false;
    }

public:
    std::vector<int> twoSum(std::vector<int> &nums, int target)
    {
        if (!is2_104Checked(nums.size()) && !is109Checked(target))
            return {0};

        std::vector<int> indeces;
        for (size_t i{0UL}; i < nums.size(); i++)
        {
            if (((i + 1UL) < nums.size()) && (is109Checked(nums.at(i))))
            {
                for (size_t j{i + 1UL}; j < nums.size(); j++)
                {
                    if ((target == (nums.at(i) + nums.at(j))))
                    {
                        indeces.push_back(static_cast<int>(i));
                        indeces.push_back(static_cast<int>(j));
                    }
                }
            }
        }

        std::cout << indeces.at(0UL) << ' ' << indeces.at(1UL) << std::endl;

        return indeces;
    }
};

int main()
{
    std::vector<int> vec{2, 7, 11, 15};

    Solution sln;
    sln.twoSum(vec, 26);

    vec = {2222222, 2222222};
    sln.twoSum(vec, 4444444);

    return EXIT_SUCCESS;
}
