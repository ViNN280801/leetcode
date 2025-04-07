"""
Given an integer array nums, return true if you can partition the array into two subsets
such that the sum of the elements in both subsets is equal or false otherwise.

Example 1:

Input: nums = [1,5,11,5]
Output: true
Explanation: The array can be partitioned as [1, 5, 5] and [11].

Example 2:

Input: nums = [1,2,3,5]
Output: false
Explanation: The array cannot be partitioned into equal sum subsets.



Constraints:

    1 <= nums.length <= 200
    1 <= nums[i] <= 100
"""

from typing import List


class Solution(object):
    def canPartition(self, nums: List[int]) -> bool:
        """
        :type nums: List[int]
        :rtype: bool
        """
        n = len(nums)

        # 2 base cases:
        if n == 0 or n == 1:
            return False
        if n == 2:
            return nums[0] == nums[1]

        # Reasoning:
        # Let's take an example: [1,5,11,5]
        # We can partition the array into two subsets (all the combinations):
        # 1. [1], [5,11,5]
        # 2. [1,5], [11,5]
        # 3. [1,5,11], [5]
        # 4. [5], [1,11,5]
        # 5. [5,1], [11,5]
        # 6. [5,1,11], [5]
        # 7. [11], [1,5,5]
        # 8. [11,1], [5,5]
        # 9. [11,1,5], [5]
        # 10. [5], [1,11,5]
        # 11. [5,1], [11,5]
        # 12. [5,1,5], [11]
        # 13. [5,11], [1,5]
        # 14. [5,11,1], [5]
        # etc.
        # So, if we have 4 elements, we can have 4! = 24 combinations.
        # Hence, if we brute force, we will have Time Complexity of O(n!) which is not efficient.
        # But here we should notice that in n! there are many repeating combinations, that is,
        # when we sum the elements from the end, we can get the same result as when we sum the elements from the beginning.
        # For example: [5,1][11,5] and [1,5][11,5]
        # So, we can use dynamic programming to solve this problem.
        # We can use an array dp, to store the sums that we can get.

        # But let's think a little bit more.
        # We can divide the array into two subsets with the same sum,
        # if the sum of all elements is even, and there exists a subset,
        # which is equal to half of the total sum.
        # For example: [1,5,11,5] => 1+5+11+5 = 22, 22/2 = 11,
        # so we can divide the array into two subsets with the same sum: [1,5,5] and [11].
        # So, if the sum of all elements is odd, we can immediately return False.
        # We can consider this as one of the base cases.
        total_sum = sum(nums)
        if total_sum % 2 != 0:
            return False

        # If the sum is even, we need to check if there exists a subset with the sum = totalSum/2
        # Let's say we have a target = totalSum/2
        # We need to check if we can make the sum of the subset equal to target,
        # if yes, then True, otherwise False
        # We use an array dp, where dp[i] = True,
        # if there exists a subset of the array nums,
        # the sum of the elements of which is equal to i, otherwise False
        # If we can calculate all such sums up to target, then the answer will be in dp[target].
        target_sum = total_sum // 2
        dp = [False] * (
            target_sum + 1
        )  # Filling the dp array with size target_sum + 1 with False values
        # Now, dp: [F, F, F, F, F, F, F, F, F, F, F, F]

        dp[0] = True  # An empty array has a sum of 0
        # dp: [T, F, F, F, F, F, F, F, F, F, F, F]
        # idx: 0  1  2  3  4  5  6  7  8  9  10 11

        # Let's iterate from the end of the array to the beginning
        # 1) i = 11, dp[11-1] = dp[10]. dp[10] = False, so dp[11] = False
        # 2) i = 10, dp[10-1] = dp[9]. dp[9] = False, so dp[10] = False
        # 3) i = 9, dp[9-1] = dp[8]. dp[8] = False, so dp[9] = False
        # 4) i = 8, dp[8-1] = dp[7]. dp[7] = False, so dp[8] = False
        # 5) i = 7, dp[7-1] = dp[6]. dp[6] = False, so dp[7] = False
        # 6) i = 6, dp[6-1] = dp[5]. dp[5] = False, so dp[6] = False
        # 7) i = 5, dp[5-1] = dp[4]. dp[4] = False, so dp[5] = False
        # 8) i = 4, dp[4-1] = dp[3]. dp[3] = False, so dp[4] = False
        # 9) i = 3, dp[3-1] = dp[2]. dp[2] = False, so dp[3] = False
        # 10) i = 2, dp[2-1] = dp[1]. dp[1] = False, so dp[2] = False
        # 11) i = 1, dp[1-1] = dp[0]. dp[0] = True, so dp[1] = True
        # dp: [T, T, F, F, F, F, F, F, F, F, F, F]
        # idx: 0  1  2  3  4  5  6  7  8  9  10 11

        # Processing the 2nd element of the array: (dp[1] = 5 in our case)
        # 1) i = 11, dp[11-5] = dp[6]. dp[6] = False, so dp[11] = False
        # 2) i = 10, dp[10-5] = dp[5]. dp[5] = False, so dp[10] = False
        # 3) i = 9, dp[9-5] = dp[4]. dp[4] = False, so dp[9] = False
        # 4) i = 8, dp[8-5] = dp[3]. dp[3] = False, so dp[8] = False
        # 5) i = 7, dp[7-5] = dp[2]. dp[2] = False, so dp[7] = False
        # 6) i = 6, dp[6-5] = dp[1]. dp[1] = True, so dp[6] = True
        # 7) i = 5, dp[5-5] = dp[0]. dp[0] = True, so dp[5] = True
        # 8) i = 4, dp[4-5] = out of bounds, so we skip
        # 9) i = 3, dp[3-5] = out of bounds, so we skip
        # 10) i = 2, dp[2-5] = out of bounds, so we skip
        # 11) i = 1, dp[1-5] = out of bounds, so we skip
        # dp: [T, T, F, F, F, T, T, F, F, F, F, F]
        # idx: 0  1  2  3  4  5  6  7  8  9  10 11

        # Processing the 3rd element of the array: (dp[2] = 11 in our case)
        # 1) i = 11, dp[11-11] = dp[0]. dp[0] = True, so dp[11] = True
        # 2) i = 10, dp[10-11] = out of bounds, so we skip, etc.
        # dp: [T, T, F, F, F, T, T, F, F, F, F, T]
        # idx: 0  1  2  3  4  5  6  7  8  9  10 11

        # Processing the 4th element of the array: (dp[3] = 5 in our case)
        # 1) i = 11, dp[11-5] = dp[6]. dp[6] = True, so dp[11] = True
        # 2) i = 10, dp[10-5] = dp[5]. dp[5] = True, so dp[10] = True
        # 3) i = 9, dp[9-5] = dp[4]. dp[4] = False, so dp[9] = False
        # 4) i = 8, dp[8-5] = dp[3]. dp[3] = False, so dp[8] = False
        # 5) i = 7, dp[7-5] = dp[2]. dp[2] = False, so dp[7] = False
        # 6) i = 6, dp[6-5] = dp[1]. dp[1] = True, so dp[6] = True
        # 7) i = 5, dp[5-5] = dp[0]. dp[0] = True, so dp[5] = True
        # 8) i = 4, dp[4-5] = out of bounds, so we skip, etc.
        # dp: [T, T, F, F, F, T, T, F, F, F, T, T]
        # idx: 0  1  2  3  4  5  6  7  8  9  10 11

        # Now we can see that dp[target_sum] = dp[11] = True
        # So, we can return True, otherwise False

        # Time complexity: O(n * target_sum), because we iterate through the array
        #                                     and for each element we iterate through the dp array
        # Space complexity: O(target_sum + 1), because we use an array dp of size (target_sum + 1)
        # which we can rewrite as O(target_sum)
        for i in range(n):
            for s in range(target_sum, nums[i] - 1, -1):
                if dp[s - nums[i]] == True:
                    dp[s] = True

        # We try to look at whether the sum sum - nums[i] was already reachable.
        # If the sum (sum - nums[i]) was reachable earlier (dp[sum - nums[i]] == True),
        # then the current sum sum is also reachable.
        # Thus, we "move" from smaller sums to larger sums.
        return dp[target_sum]


import unittest


class TestSolution(unittest.TestCase):
    def test_canPartition(self):
        solution = Solution()
        self.assertFalse(solution.canPartition([]))
        self.assertFalse(solution.canPartition([1]))
        self.assertFalse(solution.canPartition([1, 2]))
        self.assertTrue(solution.canPartition([5, 5]))
        self.assertTrue(solution.canPartition([1, 2, 3]))
        self.assertFalse(solution.canPartition([1, 3, 5]))
        self.assertTrue(solution.canPartition([1, 5, 11, 5]))
        self.assertFalse(solution.canPartition([1, 2, 3, 5]))


if __name__ == "__main__":
    unittest.main()
