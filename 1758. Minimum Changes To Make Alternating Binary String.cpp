#include <string_view>
#include <utility>

class Solution 
{
private:
    int count(std::string_view s, char prev)
    {
        int count{};
        for (size_t i{}; i < s.length(); ++i)
        {
            char cur{s[i]};
            if (cur == prev)
            {
                ++count;
                prev = (prev == '0') ? '1' : '0';
            }
            else
                prev = cur;
        }
        return count;
    }
public:
    int minOperations(std::string_view s)
    {
        if (s.empty())
            return 0;

       char firstCh{s[0]};
       int count_0{count(s, firstCh)},
           count_1{count(s, (firstCh == '0') ? '1' : '0')};
       return std::min(count_0, count_1);
    }
};

