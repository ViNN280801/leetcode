#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

class Solution
{
public:
    std::vector<int> sortedSquares(std::vector<int> &nums)
    {
        std::vector<int> squares;
        std::ranges::transform(nums, std::back_inserter(squares), [](int value)
                               { return value * value; });
        std::ranges::sort(squares);
        return squares;
    }
};

int main()
{
    Solution sln;
    std::vector<int> v({9, 3, 0, -4, -1});
    auto res{sln.sortedSquares(v)};
    for (int val : res)
        std::cout << val << ' ';
    std::endl(std::cout);

    return EXIT_SUCCESS;
}
