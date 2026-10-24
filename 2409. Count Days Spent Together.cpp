/*
Alice and Bob are traveling to Rome for separate business meetings.

You are given 4 strings arriveAlice, leaveAlice, arriveBob, and leaveBob. Alice will be in the city from the dates
arriveAlice to leaveAlice (inclusive), while Bob will be in the city from the dates arriveBob to leaveBob (inclusive).
Each will be a 5-character string in the format "MM-DD", corresponding to the month and day of the date.

Return the total number of days that Alice and Bob are in Rome together.

You can assume that all dates occur in the same calendar year, which is not a leap year. Note that the number of days
per month can be represented as: [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31].

Example 1:

Input: arriveAlice = "08-15", leaveAlice = "08-18", arriveBob = "08-16", leaveBob = "08-19"
Output: 3
Explanation: Alice will be in Rome from August 15 to August 18. Bob will be in Rome from August 16 to August 19. They
are both in Rome together on August 16th, 17th, and 18th, so the answer is 3.

Example 2:

Input: arriveAlice = "10-01", leaveAlice = "10-31", arriveBob = "11-01", leaveBob = "12-31"
Output: 0
Explanation: There is no day when Alice and Bob are in Rome together, so we return 0.

Constraints:

    All dates are provided in the format "MM-DD".
    Alice and Bob's arrival dates are earlier than or equal to their leaving dates.
    The given dates are valid dates of a non-leap year.
*/

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace Detail
{
  template <typename... Args>
  concept AtLeastOneElement = (sizeof...(Args) >= 1);

  template <typename StringType>
  concept ConvertibleToStringView = (std::convertible_to<StringType, std::string_view>);

  enum class DateFormatError : std::uint8_t
  {
    EmptyString,
    WrongCountOfSymbols,
    WrongFormat,
    ArrivalAfterLeave,
    Valid
  };
} // namespace Detail

class Solution
{
public:
  // NOLINTBEGIN(cert-err58-cpp)
  inline static constexpr bool kLeapYear                               = false;
  inline static constexpr std::uint8_t const kDateFormatLength         = 5;
  inline static constexpr int const kError                             = -1;

  static constexpr std::array<std::array<int, 13UL>, 2UL> kDaysInMonth = {{
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // non-leap year
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // leap-year
  }};

  // NOLINTEND(cert-err58-cpp)

  int
  countDaysTogether(std::string_view arriveAlice, std::string_view leaveAlice, std::string_view arriveBob,
                    std::string_view leaveBob)
  {
    auto const errs = _isDateFormatValid(kLeapYear, arriveAlice, leaveAlice, arriveBob, leaveBob);
    bool const isFormatValid
      = (std::ranges::all_of(errs, [](Detail::DateFormatError e) { return e == Detail::DateFormatError::Valid; }));

    bool const isOrderingOk = (_isOrdered(arriveAlice, leaveAlice) && (_isOrdered(arriveBob, leaveBob)));

    bool const isValid      = isFormatValid && isOrderingOk;

    if(!isValid) return kError;

    int const aIn  = _toDayOfYear(arriveAlice, kLeapYear);
    int const aOut = _toDayOfYear(leaveAlice, kLeapYear);
    int const bIn  = _toDayOfYear(arriveBob, kLeapYear);
    int const bOut = _toDayOfYear(leaveBob, kLeapYear);

    return _overlap(aIn, aOut, bIn, bOut);
  }

private:
  [[nodiscard]] static bool
  _isOrdered(std::string_view a, std::string_view b)
  {
    return a <= b;
  }

  [[nodiscard]] static Detail::DateFormatError
  _isDateFormatValid(std::string_view s, bool isLeap)
  {
    if(s.empty()) return Detail::DateFormatError::EmptyString;
    if(s.size() != kDateFormatLength) return Detail::DateFormatError::WrongCountOfSymbols;

    auto isDigit = [](char c) { return c >= '0' && c <= '9'; };
    if(s[2] != '-' || !isDigit(s[0]) || !isDigit(s[1]) || !isDigit(s[3]) || !isDigit(s[4]))
      return Detail::DateFormatError::WrongFormat;

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    int month = ((s[0] - '0') * 10) + (s[1] - '0');
    int day   = ((s[3] - '0') * 10) + (s[4] - '0');

    if(month < 1 || month > 12) return Detail::DateFormatError::WrongFormat;
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    if(day < 1 || day > kDaysInMonth.at(static_cast<std::size_t>(isLeap)).at(month))
      return Detail::DateFormatError::WrongFormat;

    return Detail::DateFormatError::Valid;
  }

  template <typename... Args>
    requires(sizeof...(Args) > 1) && (Detail::ConvertibleToStringView<Args> && ...)
  [[nodiscard]] static std::array<Detail::DateFormatError, sizeof...(Args)> _isDateFormatValid(bool isLeap,
                                                                                               Args &&...args)
  {
    return {_isDateFormatValid(std::string_view(std::forward<Args>(args)), isLeap)...};
  }

  [[nodiscard]] static int
  _toDayOfYear(std::string_view s, bool isLeap) noexcept
  {
    int const month = ((s[0] - '0') * 10) + (s[1] - '0');
    int const day   = ((s[3] - '0') * 10) + (s[4] - '0');

    int result      = day;
    for(int m{1}; m < month; ++m) result += kDaysInMonth.at(static_cast<std::size_t>(isLeap)).at(m);

    return result; // 1..365 (or 1..366)
  }

  [[nodiscard]] static int
  _overlap(int aIn, int aOut, int bIn, int bOut) // NOLINT(bugprone-easily-swappable-parameters)
    noexcept
  {
    int const lo = std::max(aIn, bIn);
    int const hi = std::min(aOut, bOut);
    return std::max(0, hi - lo + 1);
  }
};

#include <utility>

#include <gtest/gtest.h>

TEST(CountDaysTogether, Example1_PartialOverlap)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "08-16", "08-19"), 3);
}

TEST(CountDaysTogether, Example2_DisjointMonths)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("10-01", "10-31", "11-01", "12-31"), 0);
}

TEST(CountDaysTogether, NestedInterval)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-01", "08-31", "08-10", "08-12"), 3);
  EXPECT_EQ(s.countDaysTogether("08-10", "08-12", "08-01", "08-31"), 3) // symmetric
    << "containment must be symmetric";
}

TEST(CountDaysTogether, AdjacentNoOverlap)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "08-19", "08-22"), 0);
  EXPECT_EQ(s.countDaysTogether("08-19", "08-22", "08-15", "08-18"), 0);
}

TEST(CountDaysTogether, TouchingAtOneBoundary)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "08-18", "08-22"), 1)
    << "leaveA == arriveB must count that day once, not zero";
}

TEST(CountDaysTogether, SameSingleDay)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-18", "08-18", "08-18", "08-18"), 1);
  EXPECT_EQ(s.countDaysTogether("12-31", "12-31", "12-31", "12-31"), 1);
}

TEST(CountDaysTogether, FullYearBothIntervals)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("01-01", "12-31", "01-01", "12-31"), 365)
    << "catches missing +1 and a wrong cumulative table at once";
}

TEST(CountDaysTogether, CrossMonthOverlap)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-30", "09-02", "09-01", "09-05"), 2);
  EXPECT_EQ(s.countDaysTogether("01-31", "02-02", "02-01", "02-02"), 2);
}

TEST(CountDaysTogether, LeapDayRejected_NonLeap)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("02-29", "02-29", "01-01", "12-31"), -1);
}

TEST(CountDaysTogether, Feb28Accepted)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("02-28", "02-28", "02-28", "02-28"), 1);
}

// ---------- validation gate ----------

TEST(CountDaysTogether, EmptyInput)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("", "08-18", "08-16", "08-19"), -1);
  EXPECT_EQ(s.countDaysTogether("08-15", "", "08-16", "08-19"), -1);
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "", "08-19"), -1);
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "08-16", ""), -1);
}

TEST(CountDaysTogether, WrongLength)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("8-15", "08-18", "08-16", "08-19"), -1) << "no leading zero";
  EXPECT_EQ(s.countDaysTogether("08-155", "08-18", "08-16", "08-19"), -1) << "too long";
  EXPECT_EQ(s.countDaysTogether("08-15 ", "08-18", "08-16", "08-19"), -1) << "trailing space";
  EXPECT_EQ(s.countDaysTogether("0815", "08-18", "08-16", "08-19"), -1) << "no separator";
}

TEST(CountDaysTogether, WrongFormat)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("ab-cd", "08-18", "08-16", "08-19"), -1) << "non-digits";
  EXPECT_EQ(s.countDaysTogether("08/15", "08-18", "08-16", "08-19"), -1) << "wrong separator";
  EXPECT_EQ(s.countDaysTogether("0a-15", "08-18", "08-16", "08-19"), -1) << "digit in month";
  EXPECT_EQ(s.countDaysTogether("08-1b", "08-18", "08-16", "08-19"), -1) << "digit in day";
}

TEST(CountDaysTogether, InvalidMonthOrDay)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("13-01", "08-18", "08-16", "08-19"), -1) << "month 13";
  EXPECT_EQ(s.countDaysTogether("00-15", "08-18", "08-16", "08-19"), -1) << "month 00";
  EXPECT_EQ(s.countDaysTogether("04-31", "08-18", "08-16", "08-19"), -1) << "April has 30";
  EXPECT_EQ(s.countDaysTogether("01-32", "08-18", "08-16", "08-19"), -1) << "day 32";
  EXPECT_EQ(s.countDaysTogether("01-00", "08-18", "08-16", "08-19"), -1) << "day 00";
}

TEST(CountDaysTogether, ArrivalAfterLeave)
{
  Solution s;
  EXPECT_EQ(s.countDaysTogether("08-19", "08-18", "08-16", "08-19"), -1) << "Alice inverted";
  EXPECT_EQ(s.countDaysTogether("08-15", "08-18", "08-20", "08-19"), -1) << "Bob inverted";
  EXPECT_EQ(s.countDaysTogether("08-19", "08-18", "08-20", "08-19"), -1) << "both inverted";
}

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(std::addressof(argc), argv);
  return RUN_ALL_TESTS();
}

// clang-format off
// cl.exe /EHsc /std:c++20 /MD /I "D:\local\gtest-1.18.0\include" "D:\Develop\leetcode\2409. Count Days Spent Together.cpp" /link /LIBPATH:"D:\local\gtest-1.18.0\lib" gtest.lib

// g++ -std=c++20 "D:\Develop\leetcode\2409. Count Days Spent Together.cpp" -o run_tests -lgtest -lpthread
// clang-format on
