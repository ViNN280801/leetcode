#include <string>
#include <string_view>
#include <limits>
#include <cctype>

class Solution final
{
public:
    int myAtoi(std::string_view s) 
    {
        if (s.empty())
            return 0;
        
        // Skipping whitespaces
        size_t idx{};
        while (idx < s.length() && s[idx] == ' ') ++idx;

        int sign{1};
        if (s[idx] == '-')
        {
            sign = -1;
            ++idx;
        }
        else if (s[idx] == '+')
            ++idx;

        // Converting chars to digits
        long long result{};
        while (idx < s.length() && std::isdigit(s[idx]))
        {
            result = result * 10 + (s[idx] - '0');

            // Checking of the underflow and overflow
            if (sign == -1 && -result < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
            else if (sign == 1 && result > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();

            ++idx;
        }

        return static_cast<int>(result * sign);
    }
};
