#include <iostream>
#include <vector>

class Solution
{
public:
    std::vector<std::vector<int>> transpose(std::vector<std::vector<int>> const &matrix)
    {
        if (matrix.empty() || matrix[0].empty())
            return {};

        std::vector<std::vector<int>> transposed(matrix[0].size(), std::vector<int>(matrix.size()));

        for (size_t i{}; i < matrix.size(); ++i)
            for (size_t j{}; j < matrix[i].size(); ++j)
                transposed[j][i] = matrix[i][j];

        return transposed;
    }
};

int main()
{
    Solution sln;
    std::vector<std::vector<int>> v{{1, 2, 3}, {4, 5, 6}},
        transposed{sln.transpose(v)};

    for (size_t i{}; i < transposed.size(); ++i)
    {
        for (size_t j{}; j < transposed[i].size(); ++j)
            std::cout << transposed[i][j] << ' ';
        std::endl(std::cout);
    }

    return EXIT_SUCCESS;
}
