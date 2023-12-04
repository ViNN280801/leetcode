#include <iostream>
#include <string>
#include <string_view>
#include <cctype>

class Solution
{
private:
    constexpr bool onlyDigits(std::string_view num)
    {
        for (char c : num)
            if (!isdigit(c))
                return false;
        return true;
    }

public:
    std::string largestGoodInteger(std::string_view num)
    {
        if (num.empty() || num.length() < 3 || num.length() > 1000 || not onlyDigits(num))
            return "";

        char c{};
        for (size_t i{}; i < num.length(); ++i)
            if (i + 2 < num.length() && i + 1 < num.length() && num[i] == num[i + 1] && num[i] == num[i + 2])
                c = (c < num[i]) ? num[i] : c;
        if (c == 0x00)
            return "";
        std::string result;
        result += c;
        result += c;
        result += c;
        return result;
    }
};

int main()
{
    Solution sln;
    std::cout << sln.largestGoodInteger("6777133339") << std::endl;
    std::cout << sln.largestGoodInteger("2300019") << std::endl;
    std::cout << sln.largestGoodInteger("42352338") << std::endl;

    return EXIT_SUCCESS;
}
