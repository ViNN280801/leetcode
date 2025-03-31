#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

class Solution
{
public:
    long long putMarbles(std::vector<int> const &weights, int k)
    {
        if (weights.empty())
            throw std::invalid_argument("Input vector is empty");
        if (k == 0)
            throw std::invalid_argument("k is 0");
        if (k > weights.size())
            throw std::invalid_argument("k is greater than the size of the input vector");

        if (k == 1)
            return 0;

        std::vector<int> splits;
        for (size_t i{}; i < weights.size() && (i + 1) < weights.size(); ++i)
            splits.emplace_back(weights.at(i) + weights.at(i + 1));

        std::sort(splits.begin(), splits.end());

        return std::accumulate(splits.end() - k + 1, splits.end(), 0LL) -
               std::accumulate(splits.begin(), splits.begin() + k - 1, 0LL);
    }
};

#include <iostream>

int main()
{
    Solution solution;
    std::vector<int> weights{1, 3, 5, 1};
    std::cout << solution.putMarbles(weights, 2) << std::endl;
}