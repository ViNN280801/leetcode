#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t min_str_len(char const *const *const strs, size_t sz)
{
    if (!strs || sz == 0ul)
        return 0ul;

    for (size_t i = 0ul; i < sz; ++i)
        if (!strs[i])
            return 0ul;
    
    size_t minlen = strlen(strs[0ul]);
    for (size_t i = 1ul; i < sz; ++i)
    {
        size_t len = strlen(strs[i]);
        minlen = (minlen < len) ? minlen : len;
    }
    return minlen;
}

char *longestCommonPrefix(char const *const *const strs, int strsSize)
{
    if (!strs)
        return NULL;
    if (strsSize <= 0)
        return NULL;
    for (int i = 0; i < strsSize; ++i)
        if (!strs[i])
            return NULL;

    if (strsSize == 1)
        return strdup(strs[0]);

    int common_len = min_str_len(strs, strsSize);
    char *res = (char *)calloc(common_len + 1ul, sizeof(char));
    if (!res)
        return res;
    for (int chIdx = 0; chIdx < common_len; ++chIdx)
    {
        char const common_char = strs[0][chIdx];
        for (int wordIdx = 0; wordIdx < strsSize; wordIdx++)
        {
            if (common_char != strs[wordIdx][chIdx])
            {
                res[chIdx] = 0x00;
                res = realloc(res, (chIdx + 1ul) * sizeof(char));
                if (!res)
                    return res;
                return res;
            }
            res[chIdx] = common_char;
        }
    }
    res[common_len] = 0x00;
    return res;
}

int main()
{
    char const *const strs[5] = {"flower", "flow", "flight", "flew", "fly"};
    char *common_prefix = longestCommonPrefix(strs, 5);
    printf("%s\n", common_prefix);
    free(common_prefix);

    char const *const strs2[3] = {"dog", "racecar", "car"};
    common_prefix = longestCommonPrefix(strs2, 3);
    printf("%s\n", common_prefix);
    free(common_prefix);
    
    return 0;
}

