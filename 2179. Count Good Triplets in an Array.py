"""
You are given two 0-indexed arrays nums1 and nums2 of length n,
both of which are permutations of [0, 1, ..., n - 1].
A good triplet is a set of 3 distinct values which are present
in increasing order by position both in nums1 and nums2.
In other words, if we consider pos1v as the index of the value v in nums1
and pos2v as the index of the value v in nums2, then a good triplet will
be a set (x, y, z) where 0 <= x, y, z <= n - 1,
such that pos1x < pos1y < pos1z and pos2x < pos2y < pos2z.
Return the total number of good triplets.

Example 1:
Input: nums1 = [2,0,1,3], nums2 = [0,1,2,3]
Output: 1
Explanation:
There are 4 triplets (x,y,z) such that pos1x < pos1y < pos1z.
They are (2,0,1), (2,0,3), (2,1,3), and (0,1,3).
Out of those triplets, only the triplet (0,1,3) satisfies pos2x < pos2y < pos2z.
Hence, there is only 1 good triplet.

Example 2:
Input: nums1 = [4,0,1,3,2], nums2 = [4,1,0,2,3]
Output: 4
Explanation: The 4 good triplets are (4,0,3), (4,0,2), (4,1,3), and (4,1,2).

Constraints:
1) n == nums1.length == nums2.length
2) 3 <= n <= 10^5
3) 0 <= nums1[i], nums2[i] <= n - 1
4) nums1 and nums2 are permutations of [0, 1, ..., n - 1].
"""

from typing import List


class Solution:
    def _bs_find_insertion_position(self, arr: List[int], target: int) -> int:
        """
        Description:
        Find the insertion position of target in arr using binary search.
        The insertion position is the position where the target should be inserted
        to keep the array sorted.

        Time complexity: O(log(n)) - standard for binary search
        Space complexity: O(1) - no extra space is used

        Args:
            arr (List[int]): The array to search in.
            target (int): The target value to find the insertion position for.

        Returns:
            int: The insertion position of target in arr.
        """
        low, high = 0, len(arr)

        while low < high:
            mid = (low + high) // 2
            if arr[mid] < target:
                low = mid + 1
            else:
                high = mid

        return low

    def goodTriplets(self, nums1: List[int], nums2: List[int]) -> int:
        """
        Description:
        Count the number of good triplets in nums1 and nums2.

        Time complexity: O(n * log(n)) - iterate over nums2 and for each element,
        use binary search to find the insertion position.
        Space complexity: O(n) - store the sorted indices of nums1

        Args:
            nums1 (List[int]): The first array.
            nums2 (List[int]): The second array.

        Returns:
            int: The number of good triplets.
        """
        n = len(nums1)
        m = len(nums2)

        # Edge cases
        # 1. Both arrays are empty
        # Best case #1 -> both complexities are O(1)
        if n == 0 or m == 0:
            return 0

        # 2. Both arrays have |arr| < 3 elements
        # Best case #2 -> both complexities are O(1)
        if n < 3 or m < 3:
            return 0

        # Main logic
        # Idea is to find the number of elements in nums1 that are less than the current element in nums2
        # and the number of elements in nums1 that are greater than the current element in nums2
        # Then the number of good triplets is the product of the left and right counts
        # Worst case -> O(n * log(n))
        count = 0

        # 1. Map each value in nums1 to its index
        # Time complexity: O(n)
        val_to_idx = {num: i for i, num in enumerate(nums1)}
        ids_sorted = []

        # 2. Iterate over nums2 to find the good triplets
        # Time complexity: O(n)
        for value in nums2:
            idx1 = val_to_idx[value]

            # 2.1. Count elements less than idx1 (left count)
            # Time complexity: O(log(n))
            left_count = self._bs_find_insertion_position(ids_sorted, idx1)

            # 2.2. Calculate right count
            # Time complexity: O(1)
            right_count = (n - 1 - idx1) - (len(ids_sorted) - left_count)

            # 2.3. Update the total number of good triplets
            # Time complexity: O(1)
            count += left_count * right_count

            # 2.4. Insert the index in sorted order
            # Time complexity: O(log(n))
            insert_pos = self._bs_find_insertion_position(ids_sorted, idx1)
            ids_sorted.insert(insert_pos, idx1)

        # Time complexity: O(n) + O(n) * [O(log(n) + O(1) + O(log(n))] =>
        # => O(n) + O(n)*O(1+2log(n)) => O(n) + O(nlog(n)) => O(nlog(n))
        # Space complexity: O(n) - store the sorted indices of nums1

        return count


import unittest


class TestSolution(unittest.TestCase):
    def test_goodTriplets_edgeCase1_both_arrays_are_empty(self):
        nums1 = []
        nums2 = []
        expected = 0
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_edgeCase2_both_arrays_have_less_than_3_elements_opt1(self):
        nums1 = [0, 1]
        nums2 = [0, 1]
        expected = 0
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_edgeCase2_both_arrays_have_less_than_3_elements_opt2(self):
        nums1 = [0]
        nums2 = [0, 1]
        expected = 0
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_edgeCase2_both_arrays_have_less_than_3_elements_opt3(self):
        nums1 = [0, 1]
        nums2 = [0]
        expected = 0
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_edgeCase2_both_arrays_have_less_than_3_elements_opt4(self):
        nums1 = [0]
        nums2 = [0]
        expected = 0
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_example1(self):
        nums1 = [2, 0, 1, 3]
        nums2 = [0, 1, 2, 3]
        expected = 1
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)

    def test_goodTriplets_example2(self):
        nums1 = [4, 0, 1, 3, 2]
        nums2 = [4, 1, 0, 2, 3]
        expected = 4
        self.assertEqual(Solution().goodTriplets(nums1, nums2), expected)


if __name__ == "__main__":
    unittest.main()
