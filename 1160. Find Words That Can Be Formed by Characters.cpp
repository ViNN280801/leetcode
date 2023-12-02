#include <iostream>
#include <vector>
#include <string_view>
#include <string>

class Solution final
{
public:
    int countCharacters(std::vector<std::string> &words, std::string_view chars)
    {
        if (words.empty() || chars.empty())
            return 0;

        int totalLen{};
        for (std::string_view word : words)
        {
            std::string copy(chars);
            size_t len{};
            for (char c : word)
            {
                size_t pos{copy.find(c)};
                if (pos != std::string::npos)
                {
                    copy.erase(pos, 1ul);
                    ++len;
                    if (len == word.length())
                        totalLen += len;
                }
            }
        }
        return totalLen;
    }
};

int main()
{
    std::vector<std::string> v1{"cat", "bt", "hat", "tree"};
    std::vector<std::string> v2{"hello", "world", "leetcode"};

    Solution sln;
    std::cout << sln.countCharacters(v1, "atach") << '\n';
    std::cout << sln.countCharacters(v2, "welldonehoneyr") << '\n';

    return EXIT_SUCCESS;
}
