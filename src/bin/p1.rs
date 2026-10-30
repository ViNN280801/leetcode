/*
You are given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
You can return the answer in any order.

Example 1:
Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation:
The sum of nums[0] and nums[1] is 2 + 7 = 9, which equals the target. Therefore, the indices [0, 1] are returned.

Example 2:
Input: nums = [3,2,4], target = 6
Output: [1,2]
Explanation:
The sum of nums[1] and nums[2] is 2 + 4 = 6, which equals the target. Therefore, the indices [1, 2] are returned.

Example 3:
Input: nums = [3,3], target = 6
Output: [0,1]
Explanation:
The sum of nums[0] and nums[1] is 3 + 3 = 6, which equals the target. Therefore, the indices [0, 1] are returned.

Constraints:

    2 <= nums.length <= 10^4
    -10^9 <= nums[i] <= 10^9
    -10^9 <= target <= 10^9
    Only one valid answer exists.

Follow-up: Can you come up with an algorithm that is less than O(n2) time complexity?
*/

use std::collections::HashMap;
use std::hash::Hash;
use std::ops::Sub;

struct Solution;

type LeetcodeIndexType = i32;
type LeetcodeTargetValueType = i32;
type LeetcodeReturnType = Vec<LeetcodeIndexType>;

type ProductionIndexType = usize;
// ProductionTargetValueType must: Copy + PartialEq + Add
type ProductionReturnType = (ProductionIndexType, ProductionIndexType);

impl Solution {
    const INDEX_OVERFLOW: &str = "index does not fit into";
    const INDEX_OVERFLOW_PRODUCTION: &str = format!("{} {}", Self::INDEX_OVERFLOW,);
    const INDEX_OVERFLOW_LEETCODE: &str =
        format!("{} {}", Self::INDEX_OVERFLOW, LeetcodeTargetValueType);

    fn two_sum_prod<T>(nums: &[T], target: T) -> Option<ProductionReturnType>
    where
        T: Copy + Eq + Hash + Sub<Output = T>,
    {
        let mut seen: HashMap<T, ProductionIndexType> = HashMap::new();

        for (i, &x) in nums.iter().enumerate() {

        }
    }

    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        
    }
}

fn main() {}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_example_1() {
        assert_eq!(Solution::two_sum(vec![2, 7, 11, 15], 9), [0, 1]);
    }

    #[test]
    fn test_example_2() {
        assert_eq!(Solution::two_sum(vec![3, 2, 4], 6), [1, 2]);
    }

    #[test]
    fn test_example_3() {
        assert_eq!(Solution::two_sum(vec![3, 3], 6), [0, 1]);
    }
}
