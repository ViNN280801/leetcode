#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

inline bool isOpening(char bracket) { return (bracket == '(' || bracket == '[' || bracket == '{'); }
inline bool isParenthesesAreSame(char opening, char closing) { return (opening == '(' && closing == ')') ||
                                                                      (opening == '[' && closing == ']') ||
                                                                      (opening == '{' && closing == '}'); }

bool isValid(char const *const s)
{
    // Checking boundaries
    if (!s)
        return true;

    // O(n) - traverse string
    size_t len = strlen(s);

    // Initializing stack using malloc (not clearing mem to prevent any unnecessary operations)
    char *stack = malloc(len * sizeof(char));
    if (!stack)
        return false;
    int top = -1; // Initially stack is empty

    for (size_t i = 0; i < len; ++i)
    {
        // Storing symbol
        char ch = s[i];

        // Pushing openning brackets
        if (isOpening(ch))
            stack[++top] = ch;
        else
        {
            // If stack is empty or top doesn't match -> string is invalid
            if (top == -1 || !isParenthesesAreSame(stack[top], ch))
            {
                free(stack);
                return false;
            }
            top--; // Pop the matching opening bracket
        }
    }
    free(stack);
    return (top == -1); // All characters were matched, string is valid
}
