#include <algorithm>
#include <numeric>
#include <vector>

class Solution final
{
public:
    int findGCD(std::vector<int> const &nums)
    {
        if ((nums.size() < 2 || nums.size() > 1000) ||
            std::any_of(nums.cbegin(), nums.cend(), [](int num)
                        { return num < 1 || num > 1000; }))
            return 0;

        int min{__INT32_MAX__},
            max{-__INT32_MAX__};
        for (int num : nums)
        {
            if (num < min)
                min = num;
            if (num > max)
                max = num;
        }

        return std::gcd(min, max);
    }
};
