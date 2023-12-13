#include <iostream>
#include <vector>

class Solution final
{
private:
    bool hasElementInRowsCols(std::vector<std::vector<int>> const &mat, size_t row, size_t col) const
    {
        for (size_t i{}; i < mat.size(); ++i)
            if (mat[i][col] == 1 && i != row)
                return false;
        for (size_t j{}; j < mat[row].size(); ++j)
            if (mat[row][j] == 1 && j != col)
                return false;

        return true;
    }

public:
    int numSpecial(std::vector<std::vector<int>> const &mat) const
    {
        if (mat.empty())
            return 0;

        int counter{};
        for (size_t row{}; row < mat.size(); ++row)
            for (size_t col{}; col < mat[row].size(); ++col)
                if (mat[row][col] == 1 && hasElementInRowsCols(mat, row, col))
                    ++counter;
        return counter;
    }
};

int main()
{
    Solution sln;

    std::vector<std::vector<int>> v{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    std::cout << sln.numSpecial(v) << '\n';
    
    v = {{1, 0, 0}, {0, 0, 1}, {1, 0, 0}};
    std::cout << sln.numSpecial(v) << '\n';

    return 0;
}
