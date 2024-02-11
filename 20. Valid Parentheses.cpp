#include <stack>
#include <string>
#include <string_view>

class Solution final
{
public:
    bool isValid(std::string_view s)
    {
        if (s.empty())
            return true;

        std::stack<char> parentheses;
        for (char c : s)
        {
            if (c == '{' || c == '[' || c == '(')
                parentheses.push(c);
            else
            {
                if (parentheses.empty())
                    return false;
                else
                {
                    if (parentheses.top() == '{' && c == '}')
                        parentheses.pop();
                    else if (parentheses.top() == '[' && c == ']')
                        parentheses.pop();
                    else if (parentheses.top() == '(' && c == ')')
                        parentheses.pop();
                    else
                        return false;
                }
            }
        }
        if (parentheses.empty())
            return true;
        return false;
    }
};