#include <algorithm>
#include <cmath>
#include <vector>

class Solution final
{
private:
    bool _areArraysGood(std::vector<int> const &arr1, std::vector<int> const &arr2)
    {
        auto m{arr1.size()}, n{arr2.size()};

        if ((m == 0 && n != 0) ||
            (m != 0 && n == 0))
            return true;

        if (m > 1000 || n > 1000)
            return false;
        if (m + n < 1 || m + n > 2000)
            return false;

        auto checkNums{[](int num)
                       { return num < std::pow(-10, 6) || num > std::pow(10, 6); }};

        bool goodNumsInArr1{std::any_of(arr1.cbegin(), arr1.cend(), checkNums)},
            goodNumsInArr2{std::any_of(arr2.cbegin(), arr2.cend(), checkNums)};

        return goodNumsInArr1 && goodNumsInArr2;
    }

    bool _isOdd(size_t size) { return size & 1; }

    double _median(std::vector<int> const &nums)
    {
        auto size{nums.size()};
        if (_isOdd(size))
            return static_cast<double>(nums.at(size / 2));
        else
        {
            double a = nums.at(size / 2 - 1),
                   b = nums.at(size / 2);
            return (a + b) / 2;
        }
    }

public:
    double findMedianSortedArrays(std::vector<int> &nums1, std::vector<int> &nums2)
    {
        if (!_areArraysGood(nums1, nums2))
            return 0.0;

        std::vector<int> mergedSorted;
        std::merge(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), std::back_inserter(mergedSorted));

        return _median(mergedSorted);
    }
};
