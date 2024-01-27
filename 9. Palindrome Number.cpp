#include <string>
#include <algorithm>

class Solution final 
{
public:
    bool isPalindrome(int x) const
    {
        std::string xstr(std::to_string(x)), xrev(xstr);
        std::reverse(xrev.begin(), xrev.end());
        return xstr == xrev;
    }
};

