"""
Given an integer array nums sorted in non-decreasing order,
remove the duplicates in-place such that each unique element
appears only once. The relative order of the elements should be kept the same.
Then return the number of unique elements in nums.

Consider the number of unique elements of nums to be k, to get accepted,
you need to do the following things:

Change the array nums such that the first k elements of nums contain the
unique elements in the order they were present in nums initially.
The remaining elements of nums are not important as well as the size of nums.
Return k.

Custom Judge:
The judge will test your solution with the following code:
int[] nums = [...]; // Input array
int[] expectedNums = [...]; // The expected answer with correct length
int k = removeDuplicates(nums); // Calls your implementation
assert k == expectedNums.length;
for (int i = 0; i < k; i++) {
    assert nums[i] == expectedNums[i];
}

If all assertions pass, then your solution will be accepted.

Example 1:
Input: nums = [1,1,2]
Output: 2, nums = [1,2,_]
Explanation: Your function should return k = 2, with the first two elements
of nums being 1 and 2 respectively.
It does not matter what you leave beyond the returned k (hence they are underscores).

Example 2:
Input: nums = [0,0,1,1,1,2,2,3,3,4]
Output: 5, nums = [0,1,2,3,4,_,_,_,_,_]
Explanation: Your function should return k = 5, with the first five
elements of nums being 0, 1, 2, 3, and 4 respectively.
It does not matter what you leave beyond the returned k (hence they are underscores).

Constraints:
1) 1 <= nums.length <= 3 * 10^4
2) -100 <= nums[i] <= 100
3) nums is sorted in non-decreasing order.
"""

from typing import List


class Solution:
    def removeDuplicates(self, nums: List[int]) -> int:
        # Edge cases:
        # 1. Empty list
        if not nums:
            return 0

        # 2. Single element
        if len(nums) == 1:
            return 1

        # 3. List with all elements are the same
        if len(set(nums)) == 1:
            return 1

        # 4. Two different elements (previous check already handles two or more same elements)
        if len(nums) == 2:
            return 2

        # 5. General case
        nums_set = sorted(list(set(nums)))
        for i, _ in enumerate(nums):
            if i < len(nums_set):
                nums[i] = nums_set[i]

        return len(nums_set)


import unittest


class TestRemoveDuplicates(unittest.TestCase):
    def test_empty_list(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([]), 0)

    def test_single_element(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([1]), 1)

    def test_all_elements_are_the_same(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([1, 1, 1]), 1)

    def test_two_different_elems(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([1, 2]), 2)

    def test_random_case_1(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([1, 1, 2]), 2)

    def test_random_case_2(self):
        solution = Solution()
        self.assertEqual(solution.removeDuplicates([0, 0, 1, 1, 1, 2, 2, 3, 3, 4]), 5)


if __name__ == "__main__":
    unittest.main()
