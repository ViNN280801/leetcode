#include <stdlib.h>

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** transpose(int const *const *const matrix, int matrixSize, int* matrixColSize, int* returnSize, int** returnColumnSizes)
{
    *returnSize = *matrixColSize;
    *returnColumnSizes = calloc(*matrixColSize, sizeof(int));

    int **transposed = calloc(*returnSize, sizeof(int *));
    if (!transposed)
    {
        free(returnColumnSizes);
        return transposed;
    }

    for (int i = 0; i < *returnSize; ++i)
    {
        (*returnColumnSizes)[i] = matrixSize;
        transposed[i] = calloc(matrixSize, sizeof(int));
        if (!transposed[i])
        {
            for (int j = 0; j < i; ++j)
                free(transposed[j]);
            free(transposed);
            free(returnColumnSizes);
            return transposed;
        }
    }

    for (int i = 0; i < matrixSize; ++i)
        for (int j = 0; j < *matrixColSize; ++j)
            transposed[j][i] = matrix[i][j];

    return transposed;
}

