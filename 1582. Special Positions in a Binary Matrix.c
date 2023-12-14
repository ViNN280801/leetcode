int hasElem(int const *const *const mat, int rows, int cols, int row, int col)
{
    if (!mat)
        return 0;
    for (int i = 0; i < cols; ++i)
        if (mat[row][i] == 1 && i != col)
            return 1;
    for (int i = 0; i < rows; ++i)
        if (mat[i][col] == 1 && i != row)
            return 1;
    return 0;
}

int numSpecial(int const *const *const mat, int matSize, int const *const matColSize)
{
    if (!mat || !matColSize)
        return 0;
    
    for (int i = 0; i < matSize; ++i)
        if (!mat[i])
            return 0;
    
    int counter = 0;
    for (int i = 0; i < matSize; ++i)
        for (int j = 0; j < *matColSize; ++j)
            if (mat[i][j] == 1 && hasElem(mat, matSize, *matColSize, i, j) == 0)
                ++counter;
    return counter;
}

