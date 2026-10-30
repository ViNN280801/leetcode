/*
You are given an array of integers nums and an integer target, return indices of the two numbers such that they add up
to target. You may assume that each input would have exactly one solution, and you may not use the same element twice.
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

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Detail
{
  template <typename VerifiableType, typename... VerifierTypes>
  concept OneOf = (std::is_same_v<VerifiableType, VerifierTypes> || ...);

  template <typename T>
  concept CharType = OneOf<T, char, signed char, unsigned char, char8_t, char16_t, char32_t, wchar_t>;

  template <typename T>
  concept ByteType = OneOf<T, unsigned char, std::byte>;

  template <typename T>
  concept IntegralWithoutByteBoolChar
    = std::is_integral_v<T> && !CharType<T> && !ByteType<T> && !std::is_same_v<T, bool>;

  // target - x, or nullopt when the exact result does not fit into T:
  // then no y of type T satisfies x + y == target, so there is nothing to look up.
  template <std::integral T>
  constexpr auto
  CheckedComplement(T target, T x) -> std::optional<T>
  {
    if constexpr(std::is_unsigned_v<T>)
    {
      if(target < x) return std::nullopt;
    }
    else
    {
      if((x > 0) && (target < std::numeric_limits<T>::min() + x)) return std::nullopt;
      if((x < 0) && (target > std::numeric_limits<T>::max() + x)) return std::nullopt;
    }
    return static_cast<T>(target - x);
  }

  bool
  AreTwoFloatingPointNear(std::floating_point auto a, std::floating_point auto b,
                          std::common_type_t<decltype(a), decltype(b)> scale)
  {
    constexpr auto kMultiplicationFactor = 4; // A few ulps: rounding of the inputs and of target - x.

    if(a == b) return true;
    if(!std::isfinite(a) || !std::isfinite(b)) return false;

    using FloatingPointType             = std::common_type_t<decltype(a), decltype(b)>;
    constexpr FloatingPointType epsilon = std::numeric_limits<FloatingPointType>::epsilon();

    return (std::abs(a - b) <= (epsilon * kMultiplicationFactor * scale));
  };
} // namespace Detail

class Solution
{
public:
  template <typename I> using IndexPair = std::pair<std::iter_difference_t<I>, std::iter_difference_t<I>>;

  template <std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    requires Detail::IntegralWithoutByteBoolChar<std::iter_value_t<Iter>>
  auto
  twoSumProduction(Iter first, Sentinel const &last, std::iter_value_t<Iter> target) -> std::optional<IndexPair<Iter>>
  {
    std::unordered_map<std::iter_value_t<Iter>, std::iter_difference_t<Iter>> seen;
    for(std::iter_difference_t<Iter> index{}; first != last; ++first, ++index)
    {
      auto const &x     = *first;
      auto const needed = Detail::CheckedComplement<std::iter_value_t<Iter>>(target, x);
      if(auto const it = needed ? seen.find(*needed) : seen.cend(); it != seen.cend())
        return IndexPair<Iter>{it->second, index};
      seen.try_emplace(x, index);
    }
    return std::nullopt;
  }

  template <std::ranges::input_range R>
    requires Detail::IntegralWithoutByteBoolChar<std::ranges::range_value_t<R>>
  auto
  twoSumProduction(R &&range, // NOLINT(cppcoreguidelines-missing-std-forward)
                   std::ranges::range_value_t<R> target)
  { return twoSumProduction(std::ranges::begin(range), std::ranges::end(range), target); }

  template <std::input_iterator Iter, std::sentinel_for<Iter> Sentinel>
    requires std::floating_point<std::iter_value_t<Iter>>
  auto
  twoSumProduction(Iter first, Sentinel const &last, std::iter_value_t<Iter> target) -> std::optional<IndexPair<Iter>>
  {
    using Value = std::iter_value_t<Iter>;
    if(std::isnan(target)) return std::nullopt;

    std::map<Value, std::iter_difference_t<Iter>> seen;
    for(std::iter_difference_t<Iter> index{}; first != last; ++first, ++index)
    {
      Value const x = *first;
      if(std::isnan(x)) continue;

      Value const needed   = target - x;
      Value const scale    = std::max(std::abs(target), std::abs(x));

      auto const isPartner = [&](Value candidate)
      {
        return Detail::AreTwoFloatingPointNear(candidate, needed, scale)
               && Detail::AreTwoFloatingPointNear(static_cast<Value>(candidate + x), target, scale);
      };

      auto const it = seen.lower_bound(needed);
      if(it != seen.cend() && isPartner(it->first)) return IndexPair<Iter>{it->second, index};
      if(it != seen.cbegin() && isPartner(std::prev(it)->first)) return IndexPair<Iter>{std::prev(it)->second, index};

      seen.try_emplace(x, index);
    }
    return std::nullopt;
  }

  template <std::ranges::input_range R>
    requires std::floating_point<std::ranges::range_value_t<R>>
  auto
  twoSumProduction(R &&range, // NOLINT(cppcoreguidelines-missing-std-forward)
                   std::ranges::range_value_t<R> target)
  { return twoSumProduction(std::ranges::begin(range), std::ranges::end(range), target); }

  // Leetcode signature
  std::vector<int>
  twoSum(std::vector<int> &nums, int target)
  {
    auto const index_pair = twoSumProduction(nums, target);
    return index_pair.has_value()
             ? std::vector<int>{static_cast<int>(index_pair->first), static_cast<int>(index_pair->second)}
             : std::vector<int>{};
  }
};

/* Testing */
// The code below was generated by AI and reviewd by human.
//
// How to build and run the tests
// ------------------------------
// There is no main() in this file on purpose: GoogleTest ships it in
// src/gtest_main.cc, which calls RUN_ALL_TESTS(). Compile that file together
// with src/gtest-all.cc (the whole library in one translation unit) and this file.
// GTEST below is the "googletest" directory of a GoogleTest checkout
// (the one that contains "include" and "src").
//
// Linux (clang++ or g++), with address and undefined behaviour sanitizers:
//   GTEST=/path/to/gtest
//   clang++ -std=c++23 -O1 -g -fsanitize=address,undefined -pthread \
//     -isystem "$GTEST/include" -I"$GTEST" \
//     "1. Two Sum.cpp" "$GTEST/src/gtest-all.cc" "$GTEST/src/gtest_main.cc" \
//     -o two_sum_tests
//   ./two_sum_tests
//
// Linux with a system GoogleTest package (for example libgtest-dev):
//   clang++ -std=c++23 "1. Two Sum.cpp" -lgtest -lgtest_main -pthread -o two_sum_tests
//
// Windows, MSVC (run inside "x64 Native Tools Command Prompt for VS 2022"):
//   set GTEST=C:\path\to\googletest
//   cl /std:c++latest /EHsc /W4 /permissive- /fsanitize=address /Zi ^
//     /I "%GTEST%\include" /I "%GTEST%" ^
//     "1. Two Sum.cpp" "%GTEST%\src\gtest-all.cc" "%GTEST%\src\gtest_main.cc" ^
//     /Fe:two_sum_tests.exe
//   two_sum_tests.exe
//
// Windows, clang++ from LLVM or MSYS2: the Linux command works as is,
// except that -fsanitize=undefined may be unavailable; keep only address.
//
// Useful runner flags (same on every platform):
//   --gtest_filter=TwoSumLeetCode.*        run one suite only
//   --gtest_also_run_disabled_tests        include DISABLED_ tests
//   --gtest_brief=1                        print failures only

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <complex>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <random>
#include <set>
#include <span>
#include <sstream>
#include <string>

#if defined(__STDCPP_FLOAT16_T__) || defined(__STDCPP_BFLOAT16_T__)
  #include <stdfloat>
#endif

namespace
{
  // ==========================================================================
  // Helpers
  // ==========================================================================

  using IndexPair = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

  // True when twoSumProduction accepts a range of T together with a target of T.
  template <typename T>
  concept Accepted = requires(Solution solution, std::vector<T> const &values, T target) {
    solution.twoSumProduction(values, target);
  };

  // The answer may come in any order, so it is compared as a sorted pair.
  template <typename Index>
  auto
  Normalized(std::optional<std::pair<Index, Index>> const &answer) -> std::optional<IndexPair>
  {
    if(!answer.has_value()) return std::nullopt;
    auto const first  = static_cast<std::ptrdiff_t>(answer->first);
    auto const second = static_cast<std::ptrdiff_t>(answer->second);
    return IndexPair{std::min(first, second), std::max(first, second)};
  }

  auto
  Pair(std::ptrdiff_t first, std::ptrdiff_t second) -> std::optional<IndexPair>
  { return IndexPair{first, second}; }

  auto
  Sorted(std::vector<int> values) -> std::vector<int>
  {
    std::ranges::sort(values);
    return values;
  }

  // Runs the native LeetCode signature on a copy and returns a sorted answer.
  auto
  SolveNative(std::vector<int> nums, int target) -> std::vector<int>
  {
    Solution solution;
    return Sorted(solution.twoSum(nums, target));
  }

  // Multiples of 3 that never pair to 5, followed by 1 and 4: the only pair is the last one.
  auto
  OnlyLastPairSumsToFive(std::size_t size) -> std::vector<int>
  {
    constexpr int kStride = 3;
    std::vector<int> nums;
    nums.reserve(size);
    for(std::size_t i = 0; i + 2 < size; ++i) nums.push_back(static_cast<int>(i) * kStride);
    nums.push_back(1);
    nums.push_back(4);
    return nums;
  }

  // Reference answer: does any pair of distinct positions sum to target?
  // Sums are taken in a wider type, so the oracle itself cannot overflow.
  auto
  HasAnyPair(std::vector<int> const &nums, int target) -> bool
  {
    for(std::size_t i = 0; i < nums.size(); ++i)
      for(std::size_t j = i + 1; j < nums.size(); ++j)
        if(std::int64_t{nums.at(i)} + std::int64_t{nums.at(j)} == std::int64_t{target}) return true;
    return false;
  }

  // Contract check that does not depend on which valid pair is returned.
  void
  ExpectValidAnswer(std::vector<int> const &nums, int target, std::optional<IndexPair> const &answer)
  {
    ASSERT_TRUE(answer.has_value());
    auto const [first, second] = *answer;
    auto const size            = std::ssize(nums);

    ASSERT_NE(first, second) << "the same element must not be used twice";
    ASSERT_TRUE(first >= 0 && first < size) << "index " << first << " is out of range";
    ASSERT_TRUE(second >= 0 && second < size) << "index " << second << " is out of range";
    EXPECT_EQ(std::int64_t{nums.at(static_cast<std::size_t>(first))}
                + std::int64_t{nums.at(static_cast<std::size_t>(second))},
              std::int64_t{target});
  }

  // --------------------------------------------------------------------------
  // Custom types
  // --------------------------------------------------------------------------

  // Input-only generator of 0, 1, 4, 9, ... that ends at a sentinel of its own type.
  class SquaresIterator
  {
  public:
    using iterator_concept = std::input_iterator_tag;
    using value_type       = long long;
    using difference_type  = std::ptrdiff_t;

    auto
    operator*() const -> value_type
    { return root_ * root_; }

    auto
    operator++() -> SquaresIterator &
    {
      ++root_;
      return *this;
    }

    void
    operator++(int)
    { ++root_; }

    [[nodiscard]] auto
    Root() const -> value_type
    { return root_; }

  private:
    value_type root_{};
  };

  class SquaresSentinel
  {
  public:
    SquaresSentinel() = default;

    explicit SquaresSentinel(long long count) : count_{count} {}

    friend auto
    operator==(SquaresIterator const &iterator, SquaresSentinel const &sentinel) -> bool
    { return iterator.Root() >= sentinel.count_; }

  private:
    long long count_{};
  };

  class Squares
  {
  public:
    explicit Squares(long long count) : count_{count} {}

    [[nodiscard]] auto
    begin() const -> SquaresIterator
    { return {}; }

    [[nodiscard]] auto
    end() const -> SquaresSentinel
    { return SquaresSentinel{count_}; }

  private:
    long long count_{};
  };

  static_assert(std::input_iterator<SquaresIterator>);
  static_assert(!std::forward_iterator<SquaresIterator>);
  static_assert(std::sentinel_for<SquaresSentinel, SquaresIterator>);
  static_assert(std::ranges::input_range<Squares>);

  // Number-like class: supports + - and ==, but is not an arithmetic type.
  class StrongInt
  {
  public:
    explicit StrongInt(int value) : value_{value} {}

    friend auto
    operator+(StrongInt lhs, StrongInt rhs) -> StrongInt
    { return StrongInt{lhs.value_ + rhs.value_}; }

    friend auto
    operator-(StrongInt lhs, StrongInt rhs) -> StrongInt
    { return StrongInt{lhs.value_ - rhs.value_}; }

    friend auto operator==(StrongInt lhs, StrongInt rhs) -> bool = default;

  private:
    int value_{};
  };

  // Class that silently converts to int.
  class ConvertibleToInt
  {
  public:
    explicit ConvertibleToInt(int value) : value_{value} {}

    operator int() const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    { return value_; }

  private:
    int value_{};
  };

  // Unscoped on purpose: it implicitly converts to int and must still be rejected.
  enum PlainEnum : int // NOLINT(performance-enum-size,cppcoreguidelines-use-enum-class)
  {
    PlainZero,
  };

  enum class ScopedEnum : int // NOLINT(performance-enum-size)
  {
    Zero,
  };

  // ==========================================================================
  // LeetCode examples through the native signature
  // ==========================================================================

  TEST(TwoSumLeetCode, GivenExample1_WhenTwoSum_ThenReturnsZeroAndOne)
  {
    std::vector<int> const nums{2, 7, 11, 15};
    int const target  = 9;

    auto const answer = SolveNative(nums, target);

    EXPECT_EQ(answer, (std::vector<int>{0, 1}));
  }

  TEST(TwoSumLeetCode, GivenExample2_WhenTwoSum_ThenReturnsOneAndTwo)
  {
    std::vector<int> const nums{3, 2, 4};
    int const target  = 6;

    auto const answer = SolveNative(nums, target);

    EXPECT_EQ(answer, (std::vector<int>{1, 2}));
  }

  TEST(TwoSumLeetCode, GivenExample3_WhenTwoSum_ThenReturnsZeroAndOne)
  {
    std::vector<int> const nums{3, 3};
    int const target  = 6;

    auto const answer = SolveNative(nums, target);

    EXPECT_EQ(answer, (std::vector<int>{0, 1}));
  }

  TEST(TwoSumLeetCode, GivenNoSolution_WhenTwoSum_ThenReturnsEmptyVector)
  {
    std::vector<int> const nums{1, 2, 3};
    int const target  = 100;

    auto const answer = SolveNative(nums, target);

    EXPECT_TRUE(answer.empty());
  }

  TEST(TwoSumLeetCode, GivenInputUnchanged_WhenTwoSum_ThenNumsAreNotModified)
  {
    // The native signature takes a non-const reference, so mutation must be ruled out.
    Solution solution;
    std::vector<int> const before{15, 11, 7, 2};
    int const target = 9;
    auto nums        = before;

    static_cast<void>(solution.twoSum(nums, target));

    EXPECT_EQ(nums, before);
  }

  TEST(TwoSumLeetCode, GivenConstraintExtremes_WhenTwoSum_ThenSumOfBoundsIsFound)
  {
    // -10^9 <= nums[i], target <= 10^9: the sum of both bounds must not misbehave.
    std::vector<int> const nums{1'000'000'000, 5, -1'000'000'000};
    int const target  = 0;

    auto const answer = SolveNative(nums, target);

    EXPECT_EQ(answer, (std::vector<int>{0, 2}));
  }

  TEST(TwoSumLeetCode, GivenMaximumLengthInput_WhenAnswerIsLastPair_ThenItIsFound)
  {
    // 10^4 elements, the only valid pair sits at the very end.
    std::size_t const size = 10'000;
    int const target       = 5;

    auto const answer      = SolveNative(OnlyLastPairSumsToFive(size), target);

    EXPECT_EQ(answer, (std::vector<int>{9'998, 9'999}));
  }

  // ==========================================================================
  // Behaviour for every accepted arithmetic type
  // ==========================================================================

  template <typename T> class TwoSumAllAcceptedTypes : public ::testing::Test
  {};

  using AllAcceptedTypes = ::testing::Types<short, unsigned short, int, unsigned int, long, unsigned long, long long,
                                            unsigned long long, float, double, long double>;
  TYPED_TEST_SUITE(TwoSumAllAcceptedTypes, AllAcceptedTypes);

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenExample1_WhenSolved_ThenReturnsZeroAndOne)
  {
    Solution solution;
    std::vector<TypeParam> const nums{2, 7, 11, 15};

    auto const answer = solution.twoSumProduction(nums, TypeParam{9});

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenExample2_WhenSolved_ThenReturnsOneAndTwo)
  {
    Solution solution;
    std::vector<TypeParam> const nums{3, 2, 4};

    auto const answer = solution.twoSumProduction(nums, TypeParam{6});

    EXPECT_EQ(Normalized(answer), Pair(1, 2));
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenExample3_WhenSolved_ThenReturnsZeroAndOne)
  {
    Solution solution;
    std::vector<TypeParam> const nums{3, 3};

    auto const answer = solution.twoSumProduction(nums, TypeParam{6});

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenEmptyRange_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<TypeParam> const nums;

    auto const answer = solution.twoSumProduction(nums, TypeParam{0});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenSingleElementEqualToHalfTarget_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<TypeParam> const nums{5};

    auto const answer = solution.twoSumProduction(nums, TypeParam{10});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenOnlyElementReuseWouldMatch_WhenSolved_ThenReturnsNullopt)
  {
    // 3 + 3 == 6 only if index 0 is used twice.
    Solution solution;
    std::vector<TypeParam> const nums{3, 1};

    auto const answer = solution.twoSumProduction(nums, TypeParam{6});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenNoPairSums_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<TypeParam> const nums{1, 2, 4, 8, 16};

    auto const answer = solution.twoSumProduction(nums, TypeParam{7});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenLowestAndZero_WhenTargetIsLowest_ThenPairIsFound)
  {
    // target - x lands exactly on the bottom of the type range.
    Solution solution;
    auto const lowest = std::numeric_limits<TypeParam>::lowest();
    std::vector<TypeParam> const nums{lowest, TypeParam{0}};

    auto const answer = solution.twoSumProduction(nums, lowest);

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenMaxAndZero_WhenTargetIsMax_ThenPairIsFound)
  {
    // target - x lands exactly on the top of the type range.
    Solution solution;
    auto const max = std::numeric_limits<TypeParam>::max();
    std::vector<TypeParam> const nums{TypeParam{0}, max};

    auto const answer = solution.twoSumProduction(nums, max);

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumAllAcceptedTypes, GivenIndexType_WhenSolvedOnVector_ThenItIsIteratorDifferenceType)
  {
    using Answer
      = decltype(std::declval<Solution &>().twoSumProduction(std::declval<std::vector<TypeParam> &>(), TypeParam{}));

    EXPECT_TRUE((std::is_same_v<Answer, std::optional<IndexPair>>));
  }

  // ==========================================================================
  // Signed types: negative numbers and zeros
  // ==========================================================================

  template <typename T> class TwoSumSignedTypes : public ::testing::Test
  {};

  using SignedTypes = ::testing::Types<short, int, long, long long, float, double, long double>;
  TYPED_TEST_SUITE(TwoSumSignedTypes, SignedTypes);

  TYPED_TEST(TwoSumSignedTypes, GivenOnlyNegatives_WhenSolved_ThenReturnsTheirIndices)
  {
    Solution solution;
    std::vector<TypeParam> const nums{-1, -2, -3, -4, -5};

    auto const answer = solution.twoSumProduction(nums, TypeParam{-8});

    EXPECT_EQ(Normalized(answer), Pair(2, 4));
  }

  TYPED_TEST(TwoSumSignedTypes, GivenOppositeValues_WhenTargetIsZero_ThenReturnsTheirIndices)
  {
    Solution solution;
    std::vector<TypeParam> const nums{-3, 4, 3, 90};

    auto const answer = solution.twoSumProduction(nums, TypeParam{0});

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
  }

  TYPED_TEST(TwoSumSignedTypes, GivenTwoZerosAmongOthers_WhenTargetIsZero_ThenReturnsBothZeros)
  {
    Solution solution;
    std::vector<TypeParam> const nums{0, 4, 3, 0};

    auto const answer = solution.twoSumProduction(nums, TypeParam{0});

    EXPECT_EQ(Normalized(answer), Pair(0, 3));
  }

  TYPED_TEST(TwoSumSignedTypes, GivenLowestAndMax_WhenTargetIsTheirSum_ThenPairIsFound)
  {
    // lowest + max is -1 for two's complement integers and 0 for floating point.
    Solution solution;
    auto const lowest = std::numeric_limits<TypeParam>::lowest();
    auto const max    = std::numeric_limits<TypeParam>::max();
    std::vector<TypeParam> const nums{lowest, TypeParam{7}, max};

    auto const answer = solution.twoSumProduction(nums, static_cast<TypeParam>(lowest + max));

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
  }

  // ==========================================================================
  // Floating point specifics
  // ==========================================================================

  template <typename T> class TwoSumFloatingTypes : public ::testing::Test
  {};

  using FloatingTypes = ::testing::Types<float, double, long double>;
  TYPED_TEST_SUITE(TwoSumFloatingTypes, FloatingTypes);

  TYPED_TEST(TwoSumFloatingTypes, GivenExactBinaryFractions_WhenSolved_ThenReturnsTheirIndices)
  {
    Solution solution;
    std::vector<TypeParam> const nums{TypeParam{2.5}, TypeParam{7.25}, TypeParam{11}, TypeParam{0.125}};

    auto const answer = solution.twoSumProduction(nums, TypeParam{2.625});

    EXPECT_EQ(Normalized(answer), Pair(0, 3));
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenRoundingError_WhenPointOnePlusPointTwo_ThenPointThreeIsFound)
  {
    // 0.1 + 0.2 != 0.3 bit for bit; the implementation promises a tolerance.
    // Long double literals give every type its own nearest value: double
    // literals would leave long double with double-sized rounding errors.
    Solution solution;
    std::vector<TypeParam> const nums{TypeParam{0.1L}, TypeParam{0.7L}, TypeParam{0.2L}};

    auto const answer = solution.twoSumProduction(nums, TypeParam{0.3L});

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenNegativeAndPositiveZero_WhenTargetIsZero_ThenPairIsFound)
  {
    Solution solution;
    std::vector<TypeParam> const nums{TypeParam{-0.0}, TypeParam{0.0}};

    auto const answer = solution.twoSumProduction(nums, TypeParam{0});

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenOppositeInfinities_WhenTargetIsZero_ThenReturnsNullopt)
  {
    // inf + -inf is NaN, which equals nothing.
    Solution solution;
    auto const inf = std::numeric_limits<TypeParam>::infinity();
    std::vector<TypeParam> const nums{inf, -inf};

    auto const answer = solution.twoSumProduction(nums, TypeParam{0});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenInfinityAndFiniteValue_WhenTargetIsInfinity_ThenPairIsFound)
  {
    // inf + 1 == inf in IEEE 754, so this is a valid pair.
    Solution solution;
    auto const inf = std::numeric_limits<TypeParam>::infinity();
    std::vector<TypeParam> const nums{inf, TypeParam{1}};

    auto const answer = solution.twoSumProduction(nums, inf);

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenNanBeforeValidPair_WhenSolved_ThenNanIsIgnoredAndPairIsFound)
  {
    // NaN must never match, and it must not hide the values stored after it.
    Solution solution;
    std::vector<TypeParam> const nums{std::numeric_limits<TypeParam>::quiet_NaN(), TypeParam{1}, TypeParam{2}};

    auto const answer = solution.twoSumProduction(nums, TypeParam{3});

    EXPECT_EQ(Normalized(answer), Pair(1, 2));
  }

  TYPED_TEST(TwoSumFloatingTypes, GivenNanTarget_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<TypeParam> const nums{TypeParam{1}, TypeParam{2}};

    auto const answer = solution.twoSumProduction(nums, std::numeric_limits<TypeParam>::quiet_NaN());

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumFloatingPoint, GivenLargeMagnitudes_WhenRoundingExceedsAbsoluteEpsilon_ThenPairIsStillFound)
  {
    // At 1e6 one ulp is about 1e-10, far above an absolute tolerance of about 2e-13.
    Solution solution;
    std::vector<double> const nums{1e6 + 0.1, 0.2};

    auto const answer = solution.twoSumProduction(nums, 1e6 + 0.3);

    EXPECT_EQ(Normalized(answer), Pair(0, 1));
  }

  TEST(TwoSumFloatingPoint, GivenTinyMagnitudes_WhenSumIsFarFromTarget_ThenReturnsNullopt)
  {
    // 1e-14 + 2e-14 is 3e-14, not 0; an absolute tolerance must not merge them.
    Solution solution;
    std::vector<double> const nums{1e-14, 2e-14};

    auto const answer = solution.twoSumProduction(nums, 0.0);

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumFloatingPoint, GivenDenormals_WhenSumMatchesTarget_ThenPairIsFound)
  {
    Solution solution;
    auto const denormal = std::numeric_limits<double>::denorm_min();
    std::vector<double> const nums{denormal, 5.0, denormal};

    auto const answer = solution.twoSumProduction(nums, 2 * denormal);

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
  }

  // ==========================================================================
  // Type contract: what must compile and what must be rejected
  // ==========================================================================
  // Checked with EXPECT instead of static_assert so that one broken constraint
  // is reported as a failed test and does not stop the whole suite from building.

  template <typename T> class TwoSumAcceptedTypeContract : public ::testing::Test
  {};

  using AcceptedTypeContract
    = ::testing::Types<short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long,
                       std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t,
                       std::size_t, std::ptrdiff_t, float, double, long double>;
  TYPED_TEST_SUITE(TwoSumAcceptedTypeContract, AcceptedTypeContract);

  TYPED_TEST(TwoSumAcceptedTypeContract, GivenArithmeticNumberType_WhenChecked_ThenItIsAccepted)
  { EXPECT_TRUE(Accepted<TypeParam>); }

  template <typename T> class TwoSumRejectedCharTypes : public ::testing::Test
  {};

  // std::int8_t and std::uint8_t are aliases of signed char and unsigned char.
  using RejectedCharTypes = ::testing::Types<char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;
  TYPED_TEST_SUITE(TwoSumRejectedCharTypes, RejectedCharTypes);

  TYPED_TEST(TwoSumRejectedCharTypes, GivenCharacterType_WhenChecked_ThenItIsRejected)
  { EXPECT_FALSE(Accepted<TypeParam>); }

  template <typename T> class TwoSumRejectedTypes : public ::testing::Test
  {};

  using RejectedTypes
    = ::testing::Types<bool, std::byte, PlainEnum, ScopedEnum, int *, std::nullptr_t, std::complex<double>, std::string,
                       StrongInt, ConvertibleToInt, std::pair<int, int>>;
  TYPED_TEST_SUITE(TwoSumRejectedTypes, RejectedTypes);

  TYPED_TEST(TwoSumRejectedTypes, GivenNonNumberType_WhenChecked_ThenItIsRejected)
  { EXPECT_FALSE(Accepted<TypeParam>); }

  TEST(TwoSumTypeContract, GivenStrongInt_WhenUsedDirectly_ThenItBehavesLikeANumber)
  {
    // Proves that StrongInt is rejected by the constraint, not by missing operators.
    StrongInt const two{2};
    StrongInt const three{3};
    StrongInt const five{5};

    EXPECT_TRUE(two + three == five);
    EXPECT_TRUE(five - three == two);
  }

  // A discarded if constexpr branch needs a template context, hence the helper.
  template <typename Wide>
  void
  ExpectWiderThan64BitsIsSolvedWhenAccepted()
  {
    if constexpr(Accepted<Wide>)
    {
      Solution solution;
      auto const huge = static_cast<Wide>(std::numeric_limits<std::int64_t>::max()) * 4;
      std::vector<Wide> const nums{huge, 1, huge};

      auto const answer = solution.twoSumProduction(nums, static_cast<Wide>(huge * 2));

      EXPECT_EQ(Normalized(answer), Pair(0, 2));
    }
    else
    {
      GTEST_SKIP() << "__int128 is not an integral type in this language mode";
    }
  }

  TEST(TwoSumTypeContract, GivenInt128_WhenAccepted_ThenItIsSolvedCorrectly)
  {
#ifdef __SIZEOF_INT128__
    // __int128 is integral only in GNU mode; if it is accepted, it must also work.
    ExpectWiderThan64BitsIsSolvedWhenAccepted<__int128>();
#else
    GTEST_SKIP() << "the compiler has no __int128";
#endif
  }

  TEST(TwoSumTypeContract, GivenFloat16_WhenSolved_ThenPairIsFound)
  {
#ifdef __STDCPP_FLOAT16_T__
    Solution solution;
    std::vector<std::float16_t> const nums{0.5f16, 3.0f16, 0.25f16};

    auto const answer = solution.twoSumProduction(nums, 0.75f16);

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
#else
    GTEST_SKIP() << "the compiler provides no std::float16_t";
#endif
  }

  TEST(TwoSumTypeContract, GivenBfloat16_WhenSolved_ThenPairIsFound)
  {
#ifdef __STDCPP_BFLOAT16_T__
    Solution solution;
    std::vector<std::bfloat16_t> const nums{0.5bf16, 3.0bf16, 0.25bf16};

    auto const answer = solution.twoSumProduction(nums, 0.75bf16);

    EXPECT_EQ(Normalized(answer), Pair(0, 2));
#else
    GTEST_SKIP() << "the compiler provides no std::bfloat16_t";
#endif
  }

  // ==========================================================================
  // Ranges and iterators of every category
  // ==========================================================================

  TEST(TwoSumRanges, GivenStdArray_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;
    std::array<int, 4> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenCArray_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;
    int const nums[]{11, 15, 2, 7}; // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenDeque_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;
    std::deque<int> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenBidirectionalList_WhenSolved_ThenIndicesArePositions)
  {
    Solution solution;
    std::list<int> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenForwardList_WhenSolved_ThenIndicesArePositions)
  {
    Solution solution;
    std::forward_list<int> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenSortedSet_WhenSolved_ThenIndicesFollowIterationOrder)
  {
    // The set iterates as 2, 7, 11, 15 no matter the insertion order.
    Solution solution;
    std::set<int> const nums{15, 11, 7, 2};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 9)), Pair(0, 1));
  }

  TEST(TwoSumRanges, GivenSpanOverPartOfVector_WhenSolved_ThenIndicesAreRelativeToSpan)
  {
    Solution solution;
    std::vector<int> const storage{2, 7, 11, 15, 4, 5};
    std::span<int const> const tail{storage.begin() + 2, storage.end()};

    EXPECT_EQ(Normalized(solution.twoSumProduction(tail, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenTemporaryVector_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;

    EXPECT_EQ(Normalized(solution.twoSumProduction(std::vector<long long>{11, 15, 2, 7}, 9LL)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenFilterView_WhenSolved_ThenIndicesAreRelativeToView)
  {
    // The view yields 2, 4, 6, 8; 4 + 6 completes 10 first, at view positions 1 and 2.
    Solution solution;
    std::vector<int> const storage{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto even = storage | std::views::filter([](int value) { return value % 2 == 0; });

    EXPECT_EQ(Normalized(solution.twoSumProduction(even, 10)), Pair(1, 2));
  }

  TEST(TwoSumRanges, GivenTransformViewToOtherType_WhenSolved_ThenValueTypeIsTheTransformResult)
  {
    // int values become long long squares: 1, 4, 9, 16.
    Solution solution;
    std::vector<int> const storage{1, 2, 3, 4};
    auto squares = storage | std::views::transform([](int value) { return static_cast<long long>(value) * value; });

    EXPECT_EQ(Normalized(solution.twoSumProduction(squares, 25LL)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenReverseView_WhenSolved_ThenIndicesFollowReversedOrder)
  {
    Solution solution;
    std::vector<int> const storage{2, 7, 11, 15};

    EXPECT_EQ(Normalized(solution.twoSumProduction(storage | std::views::reverse, 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenInfiniteIota_WhenPairExists_ThenStopsAtTheFirstPair)
  {
    // 1, 2, 3, ... never ends; 3 + 4 is the first pair that completes 7.
    Solution solution;

    EXPECT_EQ(Normalized(solution.twoSumProduction(std::views::iota(1), 7)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenSinglePassIstreamView_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;
    std::istringstream stream{"11 15 2 7"};

    EXPECT_EQ(Normalized(solution.twoSumProduction(std::views::istream<int>(stream), 9)), Pair(2, 3));
  }

  TEST(TwoSumRanges, GivenCustomInputRangeWithSentinel_WhenSolved_ThenReturnsIndices)
  {
    // Squares 0, 1, 4, 9, 16, 25: 9 + 16 is found before 0 + 25.
    Solution solution;

    EXPECT_EQ(Normalized(solution.twoSumProduction(Squares{6}, 25LL)), Pair(3, 4));
  }

  TEST(TwoSumIterators, GivenIteratorPair_WhenSolved_ThenMatchesRangeOverload)
  {
    Solution solution;
    std::vector<int> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums.begin(), nums.end(), 9)),
              Normalized(solution.twoSumProduction(nums, 9)));
  }

  TEST(TwoSumIterators, GivenIteratorsFromTheMiddle_WhenSolved_ThenIndicesAreRelativeToFirst)
  {
    Solution solution;
    std::vector<int> const nums{2, 7, 11, 15, 4, 5};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums.begin() + 2, nums.end(), 9)), Pair(2, 3));
  }

  TEST(TwoSumIterators, GivenEqualIterators_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<int> const nums{2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums.begin(), nums.begin(), 9)), std::nullopt);
  }

  TEST(TwoSumIterators, GivenCountedIteratorWithDefaultSentinel_WhenPairIsBeyondCount_ThenReturnsNullopt)
  {
    // Only the first three elements are visible; 2 + 7 needs the fourth.
    Solution solution;
    std::vector<int> const nums{11, 15, 2, 7};

    auto const answer = solution.twoSumProduction(std::counted_iterator{nums.begin(), 3}, std::default_sentinel, 9);

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumIterators, GivenIstreamIterators_WhenSolved_ThenReturnsIndices)
  {
    Solution solution;
    std::istringstream stream{"3 2 4"};

    auto const answer = solution.twoSumProduction(std::istream_iterator<int>{stream}, std::istream_iterator<int>{}, 6);

    EXPECT_EQ(Normalized(answer), Pair(1, 2));
  }

  TEST(TwoSumIterators, GivenUnreachableSentinel_WhenPairExists_ThenStopsAtTheFirstPair)
  {
    Solution solution;
    std::vector<int> const nums{11, 15, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums.begin(), std::unreachable_sentinel, 9)), Pair(2, 3));
  }

  // ==========================================================================
  // Black hat: inputs chosen to break the implementation
  // ==========================================================================

  TEST(TwoSumAdversarial, GivenUnsignedSumThatWrapsAround_WhenSolved_ThenNoFalsePositive)
  {
    // UINT_MAX + 6 is 4294967301, not 5, even though it wraps to 5.
    Solution solution;
    std::vector<unsigned int> const nums{std::numeric_limits<unsigned int>::max(), 6U};

    auto const answer = solution.twoSumProduction(nums, 5U);

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenUnsigned64SumThatWrapsAround_WhenSolved_ThenNoFalsePositive)
  {
    Solution solution;
    std::vector<std::uint64_t> const nums{std::numeric_limits<std::uint64_t>::max(), 2U};

    auto const answer = solution.twoSumProduction(nums, std::uint64_t{1});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenShortValuesWhoseComplementIsTruncated_WhenSolved_ThenNoFalsePositive)
  {
    // -20000 + -15536 is -35536, not 30000; target - x is computed in int and
    // then narrowed back to short when used as a key.
    Solution solution;
    std::vector<std::int16_t> const nums{-20'000, -15'536};

    auto const answer = solution.twoSumProduction(nums, std::int16_t{30'000});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenUnsignedShortValuesWhoseComplementIsTruncated_WhenSolved_ThenNoFalsePositive)
  {
    // 60000 + 6000 is 66000, which only matches 464 after truncation to 16 bits.
    Solution solution;
    std::vector<std::uint16_t> const nums{60'000, 6'000};

    auto const answer = solution.twoSumProduction(nums, std::uint16_t{464});

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenSignedComplementThatOverflows_WhenSolved_ThenNoFalsePositive)
  {
    // INT_MAX + INT_MAX is not -2. target - x does not fit into int here, so it
    // must be skipped: wrapping would give INT_MAX and a false match, and the
    // overflow itself would be undefined behaviour (caught by -fsanitize=undefined).
    Solution solution;
    auto const max = std::numeric_limits<int>::max();
    std::vector<int> const nums{max, max};

    auto const answer = solution.twoSumProduction(nums, -2);

    EXPECT_EQ(Normalized(answer), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenManyDuplicatesWithoutAnswer_WhenSolved_ThenReturnsNullopt)
  {
    Solution solution;
    std::vector<int> const nums(1'000, 1);

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 3)), std::nullopt);
  }

  TEST(TwoSumAdversarial, GivenValueEqualToHalfTargetOnce_WhenSolved_ThenItIsNotPairedWithItself)
  {
    // 4 + 4 would be 8, but 4 occurs only once; the real pair is 1 + 7.
    Solution solution;
    std::vector<int> const nums{4, 1, 2, 7};

    EXPECT_EQ(Normalized(solution.twoSumProduction(nums, 8)), Pair(1, 3));
  }

  TEST(TwoSumAdversarial, GivenAllElementsEqualToHalfTarget_WhenSolved_ThenReturnsTwoDistinctIndices)
  {
    Solution solution;
    std::vector<int> const nums(1'000, 3);

    auto const answer = Normalized(solution.twoSumProduction(nums, 6));

    ASSERT_TRUE(answer.has_value());
    EXPECT_NE(answer->first, answer->second);
  }

  TEST(TwoSumAdversarial, GivenSeveralValidPairs_WhenSolved_ThenAnyReturnedPairIsValid)
  {
    Solution solution;
    std::vector<int> const nums{1, 5, 2, 4, 3, 3};
    int const target = 6;

    ExpectValidAnswer(nums, target, Normalized(solution.twoSumProduction(nums, target)));
  }

  // ==========================================================================
  // Property check against a brute-force oracle (fixed seed)
  // ==========================================================================

  TEST(TwoSumProperty, GivenRandomInputs_WhenSolved_ThenAgreesWithBruteForceOracle)
  {
    constexpr std::uint32_t kSeed    = 20'261'030;
    constexpr int kIterations        = 3'000;
    constexpr std::size_t kMaxLength = 25;
    constexpr int kValueBound        = 20;

    // A fixed seed keeps the test repeatable.
    // NOLINTNEXTLINE(cert-msc32-c,cert-msc51-cpp,bugprone-random-generator-seed)
    std::mt19937 engine{kSeed};
    std::uniform_int_distribution<std::size_t> lengthDistribution{0, kMaxLength};
    std::uniform_int_distribution<int> valueDistribution{-kValueBound, kValueBound};
    std::uniform_int_distribution<int> targetDistribution{-2 * kValueBound, 2 * kValueBound};
    Solution solution;

    for(int iteration = 0; iteration < kIterations; ++iteration)
    {
      std::vector<int> nums(lengthDistribution(engine));
      for(auto &value : nums) value = valueDistribution(engine);
      int const target   = targetDistribution(engine);

      auto const answer  = Normalized(solution.twoSumProduction(nums, target));
      auto const viaPair = Normalized(solution.twoSumProduction(nums.begin(), nums.end(), target));
      SCOPED_TRACE(::testing::Message() << "seed " << kSeed << ", iteration " << iteration);

      EXPECT_EQ(answer, viaPair);
      if(HasAnyPair(nums, target))
        ExpectValidAnswer(nums, target, answer);
      else
        EXPECT_EQ(answer, std::nullopt);
      if(::testing::Test::HasFailure()) break;
    }
  }

  // ==========================================================================
  // Performance smoke: a quadratic algorithm would not finish in reasonable time
  // ==========================================================================

  TEST(TwoSumPerformance, GivenTwoHundredThousandElements_WhenAnswerIsLastPair_ThenItIsFound)
  {
    Solution solution;
    std::size_t const size = 200'000;
    int const target       = 5;
    auto const nums        = OnlyLastPairSumsToFive(size);

    auto const answer      = solution.twoSumProduction(nums, target);

    EXPECT_EQ(Normalized(answer), Pair(199'998, 199'999));
  }
} // namespace
