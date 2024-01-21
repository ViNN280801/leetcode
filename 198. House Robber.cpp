#include <vector>
#include <utility>

class Solution final
{
public:
    int rob(std::vector<int> &nums) 
    {
        // Checking boundary conditions
        if (nums.empty())
            return 0;
        if (nums.size() == 1ul)
            return nums[0ul];
        
        // Reserving space to new vector
        std::vector<int> tmp(nums.size());
        
        // Filling this vector with max values
        tmp[0] = nums[0];
        tmp[1] = std::max(nums[0], nums[1]);

        for (size_t i{2}; i < nums.size(); ++i)
            tmp.at(i) = std::max(nums.at(i) + tmp.at(i - 2), tmp.at(i - 1));
    
        return tmp.back();
    }
};
