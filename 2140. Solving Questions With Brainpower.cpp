#include <algorithm>
#include <cmath>
#include <vector>

class Solution
{
public:
    long long mostPoints(std::vector<std::vector<int>> const &questions)
    {
        size_t n{questions.size()};
        if (n == 0 || n > std::pow(10, 5))
            return 0;
        for (auto const &subarr : questions)
        {
            if ((subarr.size() != 2) ||
                (subarr.front() < 1 || subarr.front() > std::pow(10, 5)) ||
                (subarr.back() < 1 || subarr.back() > std::pow(10, 5)))
            {
                return 0;
            }
        }

        if (n == 1)
            return questions.front().front();
        
        std::vector<long long> dp(n + 1, 0); // dp[n] == 0
        for (int i{n - 1}; i >= 0; --i)
        {
            int next{i + questions.at(i).back() + 1};
            long long solve{questions.at(i).front() + (next <= n ? dp.at(next) : 0)};
            long long skip{dp.at(i + 1)};
            dp.at(i) = std::max(solve, skip);
        }

        return dp.front();
    }
};

#include <iostream>

int main()
{
    Solution solution;
    std::vector<std::vector<int>> questions{{3, 2}, {4, 3}, {4, 4}, {2, 5}};
    std::cout << solution.mostPoints(questions) << std::endl;

    std::vector<std::vector<int>> questions2{{21, 5}, {92, 3}, {74, 2}, {39, 4}, {58, 2}, {5, 5}, {49, 4}, {65, 3}};
    std::cout << solution.mostPoints(questions2) << std::endl;
}
