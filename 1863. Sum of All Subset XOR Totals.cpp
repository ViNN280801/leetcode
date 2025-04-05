#include <vector>

class Solution
{
public:
    int subsetXORSum(std::vector<int> const &nums)
    {
        if (nums.empty())
            return 0;
        if (nums.size() == 1)
            return nums[0];
        
        int result{};
        for (int value : nums)
            result |= value;
        return result << (nums.size() - 1);
    }
};
