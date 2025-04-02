#include <algorithm>
#include <vector>

// You are given a 0-indexed integer array nums.
// Return the maximum value over all triplets of indices (i, j, k)
// such that i < j < k. If all such triplets have a negative value, return 0.
// The value of a triplet of indices (i, j, k) is equal to (nums[i] - nums[j]) * nums[k].

class Solution
{
private:
    constexpr long long _triplet_value(long long x_i, long long y_j, long long z_k) { return (x_i - y_j) * z_k; }

public:
    long long maximumTripletValue(std::vector<int> const &nums)
    {
        if (nums.size() < 3 ||
            std::all_of(nums.cbegin(), nums.cend(), [](int x)
                        { return x < 0; }))
            return 0ul;

        long long max{};

        // Brute force O(n^3)
        for (size_t i{}; i < nums.size(); ++i)
        {
            for (size_t j{i + 1}; j < nums.size(); ++j)
            {
                for (size_t k{j + 1}; k < nums.size(); ++k)
                {
                    long long value{_triplet_value(nums.at(i), nums.at(j), nums.at(k))};
                    if (value > max)
                        max = value;
                }
            }
        }
        return max;
    }
};

#include <limits>

class Solution2
{
public:
    long long maximumTripletValue(std::vector<int> const &nums)
    {
        // Edge cases
        if (nums.size() < 3 ||
            std::all_of(nums.cbegin(), nums.cend(), [](int x)
                        { return x < 0; }))
            return 0ul;

        long long max_from_left{nums.at(0)},
            max_diff{std::numeric_limits<long long>::min()},
            max_triplet_value{0};

        // O(n)
        // Traversing the array to find:
        // 1. The maximum value of nums[i] for i in [0, n - 1).
        //    Max value in the left subarray.
        // 2. The maximum value of nums[i] - nums[j] for i in [0, n - 1) and j in (i, n - 1].
        //    Max difference between any two values in the left subarray.
        // 3. The maximum value of (nums[i] - nums[j]) * nums[k]
        // for i in [0, n - 1), j in (i, n - 1], and k in (j, n).
        //    Max product of the difference between any two values in the left subarray
        //    and the value nums[k] in the right subarray.
        for (size_t i{1ul}; i < nums.size() - 1ul; ++i)
        {
            max_from_left = std::max(max_from_left, static_cast<long long>(nums.at(i - 1)));
            max_diff = std::max(max_diff, max_from_left - nums.at(i));
            max_triplet_value = std::max(max_triplet_value, max_diff * nums.at(i + 1));
        }
        return max_triplet_value;
    }
};
