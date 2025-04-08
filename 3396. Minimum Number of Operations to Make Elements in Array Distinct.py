"""
You are given an integer array nums. You need to ensure that the elements in the array are distinct.
To achieve this, you can perform the following operation any number of times:

- Remove 3 elements from the beginning of the array.
If the array has fewer than 3 elements, remove all remaining elements.

Note that an empty array is considered to have distinct elements.
Return the minimum number of operations needed to make the elements in the array distinct.



Example 1:

Input: nums = [1,2,3,4,2,3,3,5,7]
Output: 2
Explanation:
- In the first operation, the first 3 elements are removed, resulting in the array [4, 2, 3, 3, 5, 7].
- In the second operation, the next 3 elements are removed, resulting in the array [3, 5, 7],
which has distinct elements.

Therefore, the answer is 2.



Example 2:
Input: nums = [4,5,6,4,4]
Output: 2
Explanation:
- In the first operation, the first 3 elements are removed, resulting in the array [4, 4].
- In the second operation, all remaining elements are removed, resulting in an empty array.

Therefore, the answer is 2.

Example 3:

Input: nums = [6,7,8,9]
Output: 0
Explanation:
The array already contains distinct elements. Therefore, the answer is 0.

Constraints:

    1 <= nums.length <= 100
    1 <= nums[i] <= 100
"""

from typing import List


class Solution:
    def _is_distinct(self, nums: List[int]) -> bool:
        """
        Time complexity: O(n), because we need to iterate through the array (in func len())
        Space complexity: O(n), because we need to create a set of unique elements
        """
        return len(nums) == len(set(nums))

    def minimumOperations(self, nums: List[int]) -> int:
        """
        Time complexity: O(n^2)
        Space complexity: O(n)
        """
        n = len(nums)
        if n == 0 or n == 1:
            return 0
        if n == 2 and nums[0] != nums[1]:
            return 0
        if n == 2 and nums[0] == nums[1]:
            return 1
        if self._is_distinct(nums):
            return 0

        operations = 0

        # Time complexity: O(n) * O(n) = O(n^2)
        # First O(n) is for the while loop
        # Second O(n) is for the is_distinct check (nested loop)
        # Not the best solution, but it works
        # Space complexity: O(1) + O(n) = O(n)
        # First O(1) is for the operations variable
        # Second O(n) is for the nums set inside the is_distinct function
        while len(nums) > 0:
            nums = nums[3:]
            operations += 1
            if self._is_distinct(nums):
                return operations

        return operations

    def minimumOperations_optimized(self, nums: List[int]) -> int:
        """
        Time complexity: O(n), because we iterate through the array once
        Space complexity: O(n), because we use a set to store the seen elements
        """
        # 1. len(nums):
        # Time complexity: O(1), because Python caches the length of the list in a variable
        # Space complexity: O(1), because we don't create a new variable
        n = len(nums)
        if n == 0 or n == 1:
            return 0
        if n == 2 and nums[0] != nums[1]:
            return 0
        if n == 2 and nums[0] == nums[1]:
            return 1

        # 2. _is_distinct():
        # Time complexity: O(n)
        # Space complexity: O(n)
        if self._is_distinct(nums):
            return 0

        operations = 0

        # 3. copy():
        # Time complexity: O(n), because we iterate through the array once
        # Space complexity: O(n), because we create a new array
        arr = nums.copy()

        # 4. while:
        # Time complexity: O(n/3) = O(n), because we remove 3 elements from the array each time
        # Space complexity: O(1), because we don't create a new container
        while arr:
            # slicing:
            # Time complexity: O(n), because we iterate through the array once
            # Space complexity: O(n), because we create a new array
            arr = arr[3:]
            operations += 1

            # 5. _is_distinct():
            # Time complexity: O(n)
            # Space complexity: O(n)
            if not arr or self._is_distinct(arr):
                return operations

        # Resume:
        # Time complexity: O(n) + O(n) + O(n) + [O(n) * O(n)] = O(3n) + O(n^2) = O(n^2)
        # Space complexity: O(n) + O(n) + O(n) + O(n) = O(4n) = O(n)

        return operations


import unittest


class TestSolution(unittest.TestCase):
    def test_is_distinct(self):
        s = Solution()
        self.assertTrue(s._is_distinct([1, 2, 3, 4, 5]))
        self.assertFalse(s._is_distinct([1, 2, 3, 5, 5, 6]))
        self.assertFalse(s._is_distinct([1, 2, 3, 4, 4]))
        self.assertTrue(s._is_distinct([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]))
        self.assertFalse(s._is_distinct([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10]))
        self.assertTrue(s._is_distinct([]))
        self.assertTrue(s._is_distinct([1]))
        self.assertFalse(s._is_distinct([1, 1]))
        self.assertFalse(s._is_distinct([1, 1, 2]))
        self.assertTrue(
            s._is_distinct([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])
        )
        self.assertFalse(
            s._is_distinct([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15])
        )
        self.assertFalse(s._is_distinct([49, 13, 46, 7, 1, 0, -45, 0, 1, 6, 9]))

    def test_minimumOperations(self):
        s = Solution()
        self.assertEqual(s.minimumOperations([1, 2, 3, 4, 2, 3, 3, 5, 7]), 2)
        self.assertEqual(s.minimumOperations([4, 5, 6, 4, 4]), 2)
        self.assertEqual(s.minimumOperations([6, 7, 8, 9]), 0)
        self.assertEqual(s.minimumOperations([5, 7, 11, 12, 12]), 2)

    def test_minimumOperations_optimized(self):
        s = Solution()
        self.assertEqual(s.minimumOperations_optimized([1, 2, 3, 4, 2, 3, 3, 5, 7]), 2)
        self.assertEqual(s.minimumOperations_optimized([4, 5, 6, 4, 4]), 2)
        self.assertEqual(s.minimumOperations_optimized([6, 7, 8, 9]), 0)
        self.assertEqual(s.minimumOperations_optimized([5, 7, 11, 12, 12]), 2)


if __name__ == "__main__":
    unittest.main()
