// Given an integer array nums and an integer val, remove all occurrences of val in
// nums in-place. The relative order of the elements may be changed.
// Since it is impossible to change the length of the array in some languages, you
// must instead have the result be placed in the first part of the array nums. More formally,
// if there are k elements after removing the duplicates, then the first k elements of nums
// should hold the final result. It does not matter what you leave beyond the first k elements.
// Return k after placing the final result in the first k slots of nums.
// Do not allocate extra space for another array. You must do this by
// modifying the input array in-place with O(1) extra memory.

// Custom Judge:
// The judge will test your solution with the following code:

// int[] nums = [...]; // Input array
// int val = ...; // Value to remove
// int[] expectedNums = [...]; // The expected answer with correct length.
//                             // It is sorted with no values equaling val.

// int k = removeElement(nums, val); // Calls your implementation

// assert k == expectedNums.length;
// sort(nums, 0, k); // Sort the first k elements of nums
// for (int i = 0; i < actualLength; i++) {
//     assert nums[i] == expectedNums[i];
// }

// If all assertions pass, then your solution will be accepted.

// Example 1:
// Input: nums = [3,2,2,3], val = 3
// Output: 2, nums = [2,2,_,_]
// Explanation: Your function should return k = 2, with the first two elements of nums being 2.
// It does not matter what you leave beyond the returned k (hence they are underscores).

// Example 2:
// Input: nums = [0,1,2,2,3,0,4,2], val = 2
// Output: 5, nums = [0,1,4,0,3,_,_,_]
// Explanation: Your function should return k = 5, with the first five elements of nums containing 0, 0, 1, 3, and 4.
// Note that the five elements can be returned in any order.
// It does not matter what you leave beyond the returned k (hence they are underscores).

// Constraints:
// 0 <= nums.length <= 100
// 0 <= nums[i] <= 50
// 0 <= val <= 100

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

class Solution
{
protected:
    template <typename T>
    constexpr bool isVectorSizeInBounds(const std::vector<T> &nums) const
    {
        return (nums.size() >= 0) and (nums.size() <= 100);
    }

    template <typename T>
    constexpr bool isVectorElementsInBounds(const std::vector<T> &nums) const
    {
        for (auto el : nums)
        {
            if ((el < 0) or (el > 50))
                return false;
        }
        return true;
    }

    template <typename T>
    constexpr bool isValueInBounds(const T &val) const
    {
        return (val >= 0) and (val <= 100);
    }

public:
    constexpr int removeElement(std::vector<int> &nums, int val) const
    {
        if ((not isVectorSizeInBounds(nums)) || (not isVectorElementsInBounds(nums)) || (not isValueInBounds(val)))
            return 0;

        nums.erase(std::remove(std::begin(nums), std::end(nums), val), std::cend(nums));

        for (auto el : nums)
        {
            std::cout << el << ' ';
        }
        std::endl(std::cout);
        nums.shrink_to_fit();
        return nums.size();
    }
};

int main()
{
    std::vector nums{3, 2, 2, 3};

    Solution sln;
    std::cout << sln.removeElement(nums, 3) << std::endl;

    nums = {0, 1, 2, 2, 3, 0, 4, 2};
    std::cout << sln.removeElement(nums, 2) << std::endl;

    return EXIT_SUCCESS;
}
