#include <limits.h>

int minFromThree(int a, int b, int c)
{
    int min = (a < b) ? a : b;
    return (min < c) ? min : c;
}

int minInLastRow(int **matrix, int n)
{
    if (!matrix)
        return INT_MAX;
    for (int i = 0; i < n; ++i)
        if (!matrix[i])
            return INT_MAX;

    int min = INT_MAX;
    for (int col = 0; col < n; ++col)
        if (min > matrix[n - 1][col])
            min = matrix[n - 1][col];

    return min;
}

int minFallingPathSum(int** matrix, int matrixSize, int* matrixColSize) 
{
    if (!matrix || !matrixColSize || matrixSize == 0 || matrixSize != *matrixColSize)
        return 0;
    for (int i = 0; i < matrixSize; ++i)
        if(!matrix[i])
            return 0;

    for (int i = 1; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            int left = (j > 0) ? matrix[i - 1][j - 1] : INT_MAX,
                right = (j < matrixSize - 1) ? matrix[i - 1][j + 1] : INT_MAX,
                mid = matrix[i - 1][j];
                
            matrix[i][j] += minFromThree(left, mid, right);
        }
    }

    return minInLastRow(matrix, matrixSize);
}
