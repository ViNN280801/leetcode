#include <limits>
#include <type_traits>
#include <vector>

class Solution
{
private:
    bool _are_all_negative(std::vector<int> const &nums) const noexcept
    {
        // Worst case: all elements are negative -> passing the entire vector: O(n)
        // Middle case: half of the elements are negative -> passing half of the vector: O(n/2)
        // Best case: first element is positive -> O(1)
        for (int num : nums)
            if (num >= 0)
                return false;
        return true;
    }

    // O(1), hence we just compare two values
    constexpr long long _max(long long a, long long b) const { return a > b ? a : b; }

    // O(1), because we just multiply two values and return the casted result
    template <typename T, typename U>
    auto _safe_multiply(T a, U b) const -> typename std::common_type_t<T, U, long long>
    {
        using CommonType = typename std::common_type_t<T, U, long long>;
        return static_cast<CommonType>(a) * static_cast<CommonType>(b);
    }

public:
    long long maximumTripletValue(std::vector<int> const &nums) const noexcept
    {
        /* ========== Edge cases ========== */
        // 1. Length of nums is less than 3
        // .size() has O(1) complexity because size of vector is stored in the header
        if (nums.size() < 3ul)
            return 0ll;

        // 2. Length of nums == 3
        if (nums.size() == 3ul)
        {
            long long result{_safe_multiply(nums[0] - nums[1], nums[2])};

            // Edge case of edge case:
            // If the result is negative, we should return 0
            return result < 0ll ? 0ll : result;
        }

        // 3. All are negative
        if (_are_all_negative(nums))
            return 0ll;
        /* ================================= */

        // For example, let's take [12, 6, 1, 2, 7]
        // We can have triplets (i < j < k)
        // 1. (12, 6, 1) -> value = (12 - 6) * 1 = 6
        // 2. (12, 6, 2) -> value = (12 - 6) * 2 = 12
        // 3. (12, 6, 7) -> value = (12 - 6) * 7 = 42
        // 4. (12, 1, 2) -> value = (12 - 1) * 2 = 22
        // 5. (12, 1, 7) -> value = (12 - 1) * 7 = 77
        // 6. (12, 2, 7) -> value = (12 - 2) * 7 = 70
        // 7. (6, 1, 2) -> value = (6 - 1) * 2 = 10
        // 8. (6, 1, 7) -> value = (6 - 1) * 7 = 35
        // 9. (6, 2, 7) -> value = (6 - 2) * 7 = 28
        // 10. (1, 2, 7) -> value = (1 - 2) * 7 = -7

        // So, to have the maximum value, we need to find 3 values:
        // 1. The maximum value in the left subarray
        // 2. The maximum difference between any two values in the left subarray
        // 3. The maximum value of the product of the maximum difference and the value in the right subarray
        long long max_from_left{nums[0]},
            max_diff{std::numeric_limits<long long>::min()},
            max_triplet_value{};

        // O(n)
        for (size_t i{1ul}; i < nums.size() - 1ul; ++i)
        {
            max_from_left = _max(max_from_left, nums[i - 1]);
            max_diff = _max(max_diff, max_from_left - nums[i]);
            max_triplet_value = _max(max_triplet_value, _safe_multiply(max_diff, nums[i + 1]));
        }

        // Here we can have a case where final result is negative,
        // in this case, we should return 0
        return max_triplet_value < 0ll ? 0ull : max_triplet_value;
    }
};

#include <gtest/gtest.h>
#include <iostream>

TEST(MaximumValueOfAnOrderedTripletII, BasicTests)
{
    // Edge cases
    EXPECT_EQ(Solution().maximumTripletValue({}), 0);
    EXPECT_EQ(Solution().maximumTripletValue({1}), 0);
    EXPECT_EQ(Solution().maximumTripletValue({1, 2}), 0);
    EXPECT_EQ(Solution().maximumTripletValue({1, 2, 3}), 0);
    EXPECT_EQ(Solution().maximumTripletValue({5, 4, 3}), 3);
    EXPECT_EQ(Solution().maximumTripletValue({-1, -2, -3}), 0);

    // Basic cases
    EXPECT_EQ(Solution().maximumTripletValue({12, 6, 1, 2, 7}), 77);
    EXPECT_EQ(Solution().maximumTripletValue({5, 4, 8, 2}), 8);
    EXPECT_EQ(Solution().maximumTripletValue({1, 10, 3, 4, 19}), 133);
    EXPECT_EQ(Solution().maximumTripletValue({1, 2, 3, 4, 5}), 0);
    EXPECT_EQ(Solution().maximumTripletValue({100, 5, 7, 4, 5, 6}), 665);
    EXPECT_EQ(Solution().maximumTripletValue({1000000, 1, 1000000}), 999999000000);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(std::addressof(argc), argv);
    return RUN_ALL_TESTS();
}
