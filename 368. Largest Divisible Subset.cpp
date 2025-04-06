// Given a set of distinct positive integers nums, return the largest subset
// answer such that every pair (answer[i], answer[j]) of elements in this subset satisfies:

//     answer[i] % answer[j] == 0, or
//     answer[j] % answer[i] == 0

// If there are multiple solutions, return any of them.

// Example 1:

// Input: nums = [1,2,3]
// Output: [1,2]
// Explanation: [1,3] is also accepted.

// Example 2:

// Input: nums = [1,2,4,8]
// Output: [1,2,4,8]

// Constraints:

//     1 <= nums.length <= 1000
//     1 <= nums[i] <= 2 * 10^9
//     All the integers in nums are unique.

#include <type_traits>
#include <vector>

namespace _internal
{
    template <typename T>
    constexpr std::remove_reference_t<T> &&_move(T &&a) noexcept { return static_cast<std::remove_reference_t<T> &&>(a); }

    template <typename T>
    void _swap(T &a, T &b)
    {
        static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>,
                      "T must be move constructible or copy constructible");
        T temp{_move(a)};
        a = _move(b);
        b = _move(temp);
    }

    template <typename T>
    void _qsort_helper(std::vector<T> &nums, size_t left, size_t right)
    {
        static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>,
                      "T must be move constructible or copy constructible");

        // Base case: If the interval is empty or invalid
        if (left >= right) // This also implicitly handles right == SIZE_MAX from a potential underflow if we didn't check below
            return;

        // Handle the case where right might be SIZE_MAX if initial call was on an empty/single element vector incorrectly
        // Although the main qsort function handles empty/single element vectors, this adds robustness.
        if (right >= nums.size())
            return; // Avoid accessing out of bounds

        T pivot{nums.at(left)};
        size_t i{left}, j{right};
        while (i < j)
        {
            // Find element on right side smaller than pivot
            while (i < j && nums.at(j) >= pivot)
                j--;
            // Find element on left side greater than pivot
            while (i < j && nums.at(i) <= pivot)
                i++;
            // Swap elements if i < j
            if (i < j) // Check added for clarity, though the outer loop condition handles this
                _swap(nums.at(i), nums.at(j));
        }
        // Place pivot in its final position
        // Now nums[i] is the first element >= pivot (or i == j if all elements were <= pivot)
        // We need to swap the pivot (at left) with the element at index i if pivot is smaller than nums[i]
        // or if i has moved past left. The standard partition places the pivot correctly.
        // The logic should ensure nums[i] is where pivot belongs. Let's rethink the partition end.
        // After the loops, i and j have crossed or met (i >= j).
        // All elements nums[left+1...j] <= pivot
        // All elements nums[j+1...right] >= pivot
        // The correct position for the pivot is j.
        _swap(nums.at(left), nums.at(j)); // Swap pivot (at left) with element at j

        // Recurse on the left partition [left, j-1]
        // Add check to prevent underflow when j is 0
        if (j > left) // equivalent to checking if j > 0 when left is 0
            _qsort_helper(nums, left, j - 1ul);

        // Recurse on the right partition [j+1, right]
        _qsort_helper(nums, j + 1ul, right);
    }
}

namespace sorting
{
    template <typename T>
    void qsort(std::vector<T> &nums)
    {
        static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>,
                      "T must be move constructible or copy constructible");
        if (nums.empty())
            return;
        if (nums.size() == 1ul)
            return;
        if (nums.size() == 2ul)
        {
            if (nums.at(0ul) > nums.at(1ul))
                _internal::_swap(nums.at(0ul), nums.at(1ul));
            return;
        }
        _internal::_qsort_helper(nums, 0ul, nums.size() - 1ul);
    }
}

class Solution
{
public:
    std::vector<int> largestDivisibleSubset(std::vector<int> &nums)
    {
        if (nums.empty())
            return {};
        if (nums.size() == 1)
            return nums;

        // Using dynamic programming to find the largest divisible subset
        // Algorithm:
        // 1. Sort the array
        // 2. Use a dp matrix to store the largest divisible subset for each element
        // 3. Traverse the array from the end to the beginning
        // 4. For each element, we check if it is divisible by the previous element
        //    if it is, then we add the previous element to the current element's subset
        //    if it is not, then we start a new subset with the current element
        //    we keep track of the largest subset for each element
        // 5. Return the largest divisible subset

        // Time complexity:
        // Worst case: O(n^2)
        // Middle case: O(n log n)
        // Best case: O(n)

        // Space complexity: O(n)

        sorting::qsort(nums);

        // 2. Initialize DP arrays
        // dp_size[i] = size of the largest divisible subset ending with nums[i]
        // prev_idx[i] = index of the previous element in the largest divisible subset ending with nums[i]
        std::vector<int> dp_size(nums.size(), 1);
        std::vector<int> prev_idx(nums.size(), -1); // -1 indicates no predecessor

        int max_len{1}, // Stores the maximum size found so far
            max_idx{0}; // Stores the index where the max size subset ends

        // 3. Fill DP table
        // Time complexity: O(n^2), because we have a nested loop
        // Space complexity: O(n), because we are using two arrays of size n
        for (size_t i{}; i < nums.size(); ++i)
        {
            for (size_t j{}; j < i; ++j)
            {
                // If nums[i] is divisible by nums[j] and we can extend the subset ending at j
                if (nums.at(i) % nums.at(j) == 0 && dp_size.at(j) + 1 > dp_size.at(i))
                {
                    dp_size.at(i) = dp_size.at(j) + 1;
                    prev_idx.at(i) = j;
                }
            }
            // Update overall maximum length and index
            if (dp_size.at(i) > max_len)
            {
                max_len = dp_size.at(i);
                max_idx = i;
            }
        }

        // 4. Reconstruct the largest subset by backtracking
        std::vector<int> result;
        int current_idx{max_idx};
        while (current_idx != -1)
        {
            result.push_back(nums.at(current_idx));
            current_idx = prev_idx.at(current_idx);
        }

        return result;
    }
};

#include <gtest/gtest.h>

TEST(Solution, Test1)
{
    Solution solution;
    std::vector<int> nums{1, 2, 3};
    std::vector<int> expected_reversed{2, 1};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected_reversed);
}

TEST(Solution, Test2)
{
    Solution solution;
    std::vector<int> nums{1, 2, 4, 8};
    std::vector<int> expected_reversed{8, 4, 2, 1};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected_reversed);
}

TEST(Solution, Test3)
{
    Solution solution;
    std::vector<int> nums{1, 5, 1, 3, 7, 8, 9};
    std::vector<int> expected_reversed{9, 3, 1, 1};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected_reversed);
}

TEST(Solution, Test4)
{
    Solution solution;
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> expected_reversed{8, 4, 2, 1};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected_reversed);
}

TEST(Solution, Test5)
{
    Solution solution;
    std::vector<int> nums{1};
    std::vector<int> expected{1};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected);
}

TEST(Solution, Test6)
{
    Solution solution;
    std::vector<int> nums{};
    std::vector<int> expected{};
    EXPECT_EQ(solution.largestDivisibleSubset(nums), expected);
}

// Compile and run:
// 
