#include <string>
#include <vector>

class Solution final
{
public:
    std::string destCity(std::vector<std::vector<std::string>> const &paths) const
    {
        if (paths.empty())
            return "";

        std::string dest(paths.front().back());
        if (paths.size() == 1ul)
            return dest;

        for (size_t i{}; i < paths.size(); ++i)
            if (dest == paths[i].front())
            {
                dest = paths[i].back();
                i = 0;
            }
        return dest;
    }
};