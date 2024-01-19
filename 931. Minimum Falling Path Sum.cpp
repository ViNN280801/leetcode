#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

class Solution final
{
public:
    int minFallingPathSum(std::vector<std::vector<int>> &matrix)
    {
        if (matrix.empty())
            return 0;

        size_t n{matrix.size()};
        for (size_t i{1ul}; i < n; ++i)
        {
            for (size_t j{}; j < n; ++j)
            {
                int left{(j > 0) ? matrix.at(i - 1).at(j - 1) : std::numeric_limits<int>::max()},
                    right{(j < n - 1) ? matrix.at(i - 1).at(j + 1) : std::numeric_limits<int>::max()},
                    mid{matrix.at(i - 1).at(j)};

                matrix.at(i).at(j) += std::min({left, mid, right});
            }
        }

        return *std::min_element(matrix.at(n - 1).cbegin(), matrix.at(n - 1).cend());
    }
};

int main()
{
    Solution sln;
    std::vector<std::vector<int>> matrix = {{2, 1, 3}, {6, 5, 4}, {7, 8, 9}};
    std::cout << sln.minFallingPathSum(matrix);

    return 0;
}

