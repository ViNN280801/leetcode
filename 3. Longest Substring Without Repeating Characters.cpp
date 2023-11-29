#include <iostream>
#include <string_view>
#include <string>
#include <algorithm>

class Solution final
{
public:
    /**
     * @brief Finds the length of the longest substring without repeating characters.
     * This method employs a sliding window approach, continuously updating the window.
     * @param s The input string_view.
     * @return The length of the longest substring without repeating characters.
     */
    int lengthOfLongestSubstring(std::string_view s)
    {
        /* Checking boundary conditions */
        if (s.empty())
            return 0;
        if (s.length() == 1ul)
            return 1;

        size_t posOfUniqSubstr{}; // Position of the initial character in the window
        int maxLen{};             // Length of the max unique substring
        std::string uniqSubstr;

        for (size_t i{}; i < s.length();)
        {
            // If there is no such a character in the substring - add it
            if (uniqSubstr.find(s[i]) == std::string::npos)
            {
                uniqSubstr += s[i++];

                // If position of the character is equals to the length of the initial string
                // we need to check its length with current max length and assign if it necessary
                if (i == s.length())
                    maxLen = std::max(maxLen, static_cast<int>(uniqSubstr.length()));
            }
            // Otherwise:
            // 1. Compare current max length with the substring length.
            // 2. Update substring.
            // 3. Change window size.
            else
            {
                maxLen = std::max(maxLen, static_cast<int>(uniqSubstr.length()));
                uniqSubstr.clear();
                i = ++posOfUniqSubstr;
            }
        }
        return maxLen;
    }
};

int main()
{
    Solution sln;

    std::cout << sln.lengthOfLongestSubstring("dvdf") << '\n';
    std::cout << sln.lengthOfLongestSubstring("ckilbkd") << '\n';
    std::cout << sln.lengthOfLongestSubstring("au") << '\n';
    std::cout << sln.lengthOfLongestSubstring("abcabcbb") << '\n';
    std::cout << sln.lengthOfLongestSubstring("bbbbb") << '\n';
    std::cout << sln.lengthOfLongestSubstring("pwwkew") << '\n';

    return EXIT_SUCCESS;
}
