/*
You are given two 0-indexed strings source and target, both of length n and
consisting of lowercase English letters. You are also given two 0-indexed
character arrays original and changed, and an integer array cost, where cost[i]
represents the cost of changing the character original[i] to the character
changed[i].

You start with the string source. In one operation, you can pick a character x
from the string and change it to the character y at a cost of z if there exists
any index j such that cost[j] == z, original[j] == x, and changed[j] == y.

Return the minimum cost to convert the string source to the target using
any number of operations. If it is impossible to convert source to target,
return -1.

Note that there may exist indices i, j such that original[j] == original[i] and
changed[j] == changed[i].

Constraints:
    1 <= source.length == target.length <= 10^5
    source, target consist of lowercase English letters.
    1 <= cost.length == original.length == changed.length <= 2000
    original[i], changed[i] are lowercase English letters.
    1 <= cost[i] <= 10^6
    original[i] != changed[i]
*/

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#if (defined(__x86_64__) || defined(_M_X64)) && defined(__AVX2__)
  #include <immintrin.h>
#endif

namespace
{
  // ============= Concepts (universal algorithm) =============

  template <typename T>
  concept CharacterType = std::integral<T> && sizeof(T) <= sizeof(char32_t);

  template <typename T>
  concept CostType = std::integral<T> && std::is_signed_v<T>;

  // ============= Constants =============

  constexpr std::size_t kAlphabetSize                 = 26U;

  template <CostType CostT> constexpr CostT kInfinity = static_cast<CostT>(std::numeric_limits<CostT>::max() / 2);

  // ============= Character index (a-z -> 0-25) =============

  template <CharacterType CharT>
  constexpr std::size_t
  CharToIndex(CharT chr) noexcept
  {
    auto const idx = static_cast<std::size_t>(static_cast<unsigned char>(chr));
    return idx - static_cast<std::size_t>('a');
  }

  // ============= Conversion graph: 26x26 min-cost matrix =============

  template <CostType CostT> using CostMatrix = std::array<std::array<CostT, kAlphabetSize>, kAlphabetSize>;

  template <CharacterType CharT, CostType CostT, typename CostInputT>
    requires std::convertible_to<CostInputT, CostT>
  void
  BuildConversionGraph(std::span<CharT const> original, // NOLINT(bugprone-easily-swappable-parameters)
                       std::span<CharT const> changed, std::span<CostInputT const> cost,
                       CostMatrix<CostT> &out) noexcept
  {
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
    for(std::size_t i{}; i < kAlphabetSize; ++i)
      for(std::size_t j{}; j < kAlphabetSize; ++j) out[i][j] = (i == j) ? static_cast<CostT>(0) : kInfinity<CostT>;

    for(std::size_t i{}; i < cost.size(); ++i)
    {
      std::size_t const idxU = CharToIndex(original[i]);
      std::size_t const idxV = CharToIndex(changed[i]);
      auto const weight      = static_cast<CostT>(cost[i]);
      if(weight < out[idxU][idxV]) out[idxU][idxV] = weight;
    }
    // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  // ============= All-pairs shortest path: Floyd-Warshall =============

  template <CostType CostT>
  void
  FloydWarshall(CostMatrix<CostT> &dist) noexcept
  {
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
    for(std::size_t k{}; k < kAlphabetSize; ++k)
    {
      for(std::size_t i{}; i < kAlphabetSize; ++i)
      {
        if(dist[i][k] == kInfinity<CostT>) continue;
        for(std::size_t j{}; j < kAlphabetSize; ++j)
        {
          if(dist[k][j] == kInfinity<CostT>) continue;
          CostT const via = dist[i][k] + dist[k][j];
          if(via < dist[i][j]) dist[i][j] = via;
        }
      }
    }
    // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
  }

  // ============= SIMD sum for int64 (AVX2), fallback scalar =============

#if (defined(__x86_64__) || defined(_M_X64)) && defined(__AVX2__)
  [[nodiscard]] std::int64_t
  SimdSum64(std::span<std::int64_t const> values) noexcept
  {
    __m256i sumVec = _mm256_setzero_si256();
    std::size_t i  = 0u;
    for(; i + 4u <= values.size(); i += 4u)
    {
      __m256i const chunk = _mm256_loadu_si256(reinterpret_cast<__m256i const *>(values.data() + i));
      sumVec              = _mm256_add_epi64(sumVec, chunk);
    }
    alignas(32) std::int64_t tail[4];
    _mm256_store_si256(reinterpret_cast<__m256i *>(tail), sumVec);
    std::int64_t sum = tail[0] + tail[1] + tail[2] + tail[3];
    for(; i < values.size(); ++i) sum += values[i];
    return sum;
  }
#else
  [[nodiscard]] inline std::int64_t
  SimdSum64(std::span<std::int64_t const> values) noexcept
  {
    std::int64_t sum = 0;
    std::ranges::for_each(values, [&sum](auto val) { sum += val; });
    return sum;
  }
#endif

  // ============= Minimum conversion cost (universal core) =============
  // Thread-safe: no shared mutable state.
  // Exception-safety: nothrow when using fixed matrix and spans.

  template <CharacterType CharT, CostType CostT, typename CostInputT>
    requires std::convertible_to<CostInputT, CostT>
  [[nodiscard]] CostT
  MinimumConversionCostImpl(std::basic_string_view<CharT> source, std::basic_string_view<CharT> target,
                            std::span<CharT const> original, std::span<CharT const> changed,
                            std::span<CostInputT const> cost) noexcept
  {
    if(source.size() != target.size() || cost.empty() || original.size() != changed.size()
       || original.size() != cost.size())
    {
      return static_cast<CostT>(-1);
    }

    CostMatrix<CostT> dist{};
    BuildConversionGraph<CharT, CostT, CostInputT>(original, changed, cost, dist);
    FloydWarshall(dist);

    // View: indices -> (source[i], target[i]) -> filter(s != t) -> cost lookup
    auto const Nsize = source.size();
    auto indices     = std::views::iota(std::size_t{0}, Nsize);
    auto pairs = indices | std::views::transform([&](std::size_t idx) { return std::pair(source[idx], target[idx]); });
    auto differing = pairs | std::views::filter([](auto const &pair) { return pair.first != pair.second; });
    auto costView  = differing
                    | std::views::transform(
                      [&dist](auto const &pair)
                      {
                        return dist[ // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
                          CharToIndex(pair.first)][CharToIndex(pair.second)];
                      });

    // Check for unreachable (infinity) and collect costs for sum
    std::vector<std::int64_t> costsToSum;
    costsToSum.reserve(Nsize);
    for(auto const &chr : costView)
    {
      if(chr == kInfinity<CostT>) return static_cast<CostT>(-1);
      costsToSum.push_back(static_cast<std::int64_t>(chr));
    }

    std::int64_t const total = SimdSum64(costsToSum);
    if(total > static_cast<std::int64_t>(std::numeric_limits<CostT>::max())) return static_cast<CostT>(-1);
    return static_cast<CostT>(total);
  }

} // anonymous namespace

// ============= LeetCode entry point =============

class Solution
{
public:
  /**
   * @brief Minimum cost to convert source string to target using given
   * character conversions.
   * @param source Initial string.
   * @param target Target string (same length as source).
   * @param original Origin character for each conversion edge.
   * @param changed Target character for each conversion edge.
   * @param cost Cost of each conversion edge.
   * @return Minimum total cost, or -1 if conversion is impossible.
   * @note Thread-safe: stateless, pure function of inputs.
   * @note Exception-safety: strong guarantee (may throw on vector copy;
   * internal core is noexcept).
   */
  // NOLINTNEXTLINE(readability-identifier-naming) LeetCode API
  long long
  minimumCost(std::string const &source, std::string const &target,
              std::vector<char> &original, // NOLINT(bugprone-easily-swappable-parameters)
              std::vector<char> &changed, std::vector<int> &cost)
  {
    return MinimumConversionCostImpl<char, std::int64_t, int>(
      std::string_view(source), std::string_view(target), std::span<char const>(original.data(), original.size()),
      std::span<char const>(changed.data(), changed.size()), std::span<int const>(cost.data(), cost.size()));
  }
};
