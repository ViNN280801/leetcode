#include <string>
#include <vector>
#include <string_view>

class Solution final
{
public:
    std::string longestCommonPrefix(std::vector<std::string> const &strs) const
    {
        if (strs.empty())
            return "";
        if (strs.size() == 1ul)
            return strs.front();

        size_t commonLen{strs.front().length()};
        for (std::string_view word : strs)
            commonLen = std::min(word.length(), commonLen);

        for (size_t i{}; i < commonLen; ++i)
        {
            char const commonCh{strs.front()[i]};
            for (std::string_view word : strs)
                if (word[i] != commonCh)
                    return std::string(strs.front().substr(0ul, i));
        }
        return strs.front().substr(0ul, commonLen);
    }
};
