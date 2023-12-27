#include <string.h>

int count(char const *s, char prev)
{
    if (!s || s == "")
        return 0;
    size_t len = strlen(s);
    int counter = 0;
    for (size_t i = 0ul; i < len; ++i)
    {
        char cur = s[i];
        if (cur == prev)
        {
            ++count;
            prev = (prev == '0') ? '1' : '0';
        }
        else
            prev = cur;
    }
    return counter;
}

int minOperations(char const *s)
{
    if (!s || s == "")
        return 0;

    char first = s[0];
    int count1 = count(s, first),
        count2 = count(s, (first == '0') ? '1' : '0');
    return (count1 < count2) ? count1 : count2;
}

