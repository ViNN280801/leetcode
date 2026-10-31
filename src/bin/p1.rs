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

struct Solution;

use std::collections::HashMap;

#[derive(Debug, PartialEq, Eq)]
enum FloatComparisonError {
    NonFiniteInput,
    InvalidTolerance,
}

fn are_two_floating_point_near(
    a: f64,
    b: f64,
    abs_epsilon: f64,
    rel_epsilon: f64,
) -> Result<bool, FloatComparisonError> {
    // 1. Проверяем входные значения: NaN и ±∞ недопустимы.
    if !a.is_finite() || !b.is_finite() {
        return Err(FloatComparisonError::NonFiniteInput);
    }

    // 2. Проверяем допуски: отрицательные, NaN, ±∞ недопустимы.
    if !abs_epsilon.is_finite()
        || !rel_epsilon.is_finite()
        || abs_epsilon < 0.0
        || rel_epsilon < 0.0
    {
        return Err(FloatComparisonError::InvalidTolerance);
    }

    // 3. Точное совпадение (включая +0.0 == -0.0).
    if a == b {
        return Ok(true);
    }

    // 4. Комбинированный критерий: abs ИЛИ rel.
    let diff = (a - b).abs();
    let scale = a.abs().max(b.abs());
    let bound = abs_epsilon.max(rel_epsilon * scale);

    Ok(diff <= bound)
}

trait TwoSumLookup<V: Copy> {
    fn remember(&mut self, value: V, index: usize);
    fn find(&self, value: V) -> Option<usize>;
}

trait TwoSumValue: Copy + PartialEq {
    type Lookup: Default + TwoSumLookup<Self>;

    fn checked_sub(self, other: Self) -> Option<Self>;
}

impl TwoSumValue for i32 {
    type Lookup = HashMap<i32, usize>;

    fn checked_sub(self, other: Self) -> Option<Self> {
        i32::checked_sub(self, other)
    }
}

impl TwoSumLookup<i32> for HashMap<i32, usize> {
    fn remember(&mut self, value: i32, index: usize) {
        self.insert(value, index);
    }
    fn find(&self, value: i32) -> Option<usize> {
        self.get(&value).copied()
    }
}

struct FloatLookup {
    entries: Vec<(f64, usize)>,
    abs_epsilon: f64,
    rel_epsilon: f64,
}

impl Default for FloatLookup {
    fn default() -> Self {
        Self {
            entries: Vec::new(),
            abs_epsilon: 1e-10,
            rel_epsilon: 1e-10,
        }
    }
}

impl TwoSumValue for f64 {
    type Lookup = FloatLookup;

    fn checked_sub(self, other: Self) -> Option<Self> {
        let result = self - other;
        if result.is_finite() {
            Some(result)
        } else {
            None
        }
    }
}

impl TwoSumLookup<f64> for FloatLookup {
    fn remember(&mut self, value: f64, index: usize) {
        self.entries.push((value, index));
    }

    fn find(&self, value: f64) -> Option<usize> {
        self.entries.iter().find_map(|&(stored, idx)| {
            are_two_floating_point_near(stored, value, self.abs_epsilon, self.rel_epsilon)
                .unwrap_or(false)
                .then_some(idx)
        })
    }
}

fn two_sum_generic<V: TwoSumValue>(nums: &[V], target: V) -> Option<(usize, usize)> {
    let mut lookup = V::Lookup::default();

    for (i, &value) in nums.iter().enumerate() {
        if let Some(complement) = target.checked_sub(value) {
            if let Some(j) = lookup.find(complement) {
                return Some((j, i));
            }
        }
        lookup.remember(value, i);
    }
    None
}

impl Solution {
    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        let (a, b) =
            two_sum_generic(&nums, target).expect("the problem guarantee exactly one solution.");
        vec![a as i32, b as i32]
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
