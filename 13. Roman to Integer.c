#include <stdio.h>
#include <string.h>

int get_roman_int(char roman_literal)
{
    switch (roman_literal)
    {
    case 'I':
        return 1;
    case 'V':
        return 5;
    case 'X':
        return 10;
    case 'L':
        return 50;
    case 'C':
        return 100;
    case 'D':
        return 500;
    case 'M':
        return 1000;
    default:
        return 0;
    }
}

int romanToInt(char const* s)
{
    if (!s || strlen(s) == 0ul || strcmp(s, " ") == 0)
        return 0;

    int result = 0;
    size_t len = strlen(s);
    for (size_t i = 0ul; i < len; ++i)
    {
        if (i + 1 < len && ((s[i] == 'I' && s[i + 1] == 'V') ||
                            (s[i] == 'I' && s[i + 1] == 'X'))) {
            result += get_roman_int(s[i + 1]) - 1;
            ++i;
        }
        else if (i + 1 < len && ((s[i] == 'X' && s[i + 1] == 'L') ||
                                 (s[i] == 'X' && s[i + 1] == 'C'))) {
            result += get_roman_int(s[i + 1]) - 10;
            ++i;
        }
        else if (i + 1 < len && ((s[i] == 'C' && s[i + 1] == 'D') ||
                                 (s[i] == 'C' && s[i + 1] == 'M'))) {
            result += get_roman_int(s[i + 1]) - 100;
            ++i;
        }
        else
            result += get_roman_int(s[i]);
    }
    return (result >= 1 && result <= 3999) ? result : 0;
}

int main()
{
    printf("%d\n", romanToInt(""));
    printf("%d\n", romanToInt(" "));
    printf("%d\n", romanToInt("I"));
    printf("%d\n", romanToInt("II"));
    printf("%d\n", romanToInt("III"));
    printf("%d\n", romanToInt("IIII"));
    printf("%d\n", romanToInt("IV"));
    printf("%d\n", romanToInt("V"));
    printf("%d\n", romanToInt("VII"));
    printf("%d\n", romanToInt("IX"));
    printf("%d\n", romanToInt("MCMXCIV"));

    return 0;
}

