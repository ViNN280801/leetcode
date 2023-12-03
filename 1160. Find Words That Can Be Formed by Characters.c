#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countCharacters(char **words, int wordsSize, char *chars)
{
    int result = 0;

    int charFreq[26] = {0};
    for (int i = 0; chars[i] != '\0'; i++)
        charFreq[chars[i] - 'a']++;

    for (int i = 0; i < wordsSize; i++)
    {
        int wordFreq[26] = {0};
        int canFormWord = 1;

        for (int j = 0; words[i][j] != '\0'; j++)
        {
            wordFreq[words[i][j] - 'a']++;
        }
        for (int j = 0; j < 26; j++)
        {
            if (wordFreq[j] > charFreq[j])
            {
                canFormWord = 0;
                break;
            }
        }

        if (canFormWord)
            result += strlen(words[i]);
    }

    return result;
}

int main()
{
    char *words1[] = {"cat", "bt", "hat", "tree"};
    char chars1[] = "atach";
    printf("%d\n", countCharacters(words1, 4, chars1));

    char *words2[] = {"hello", "world", "leetcode"};
    char chars2[] = "welldonehoneyr";
    printf("%d\n", countCharacters(words2, 3, chars2));

    return 0;
}
