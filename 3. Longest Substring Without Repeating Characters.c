#include <stdio.h>
#include <string.h>

#define BUFFSIZE 128

size_t find_ch(char const *s, char to_find)
{
    size_t len = strlen(s);
    for (size_t i = 0ul; i < len; ++i)
        if (s[i] == to_find)
            return i;
    return len;
}

int max(int a, int b) { return a < b ? b : a; }

int lengthOfLongestSubstring(char const *s)
{
    size_t len = strlen(s);
    if (!s || len == 0ul)
        return 0;
    if (len == 1ul)
        return 1;

    int maxLen = 0;
    size_t pos = 0ul;
    char buf[BUFFSIZE];
    memset(buf, 0x00, BUFFSIZE);
    for (size_t i = 0ul; i < len;)
    {
        if (find_ch(buf, s[i]) == strlen(buf))
        {
            strncat(buf, &s[i++], 1ul);
            if (i >= len - 1ul)
                maxLen = max(maxLen, strlen(buf));
        }
        else
        {
            maxLen = max(maxLen, strlen(buf));
            memset(buf, 0x00, BUFFSIZE);
            ++pos;
            i = pos;
        }
    }
    return maxLen;
}

int main()
{
    printf("%d\n", lengthOfLongestSubstring("aab"));
    printf("%d\n", lengthOfLongestSubstring("abcd"));
    printf("%d\n", lengthOfLongestSubstring("au"));
    printf("%d\n", lengthOfLongestSubstring("bbbbb"));
    printf("%d\n", lengthOfLongestSubstring("abcabcbb"));
    printf("%d\n", lengthOfLongestSubstring("pwwkew"));

    return 0;
}
