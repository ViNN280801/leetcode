#include <string>
#include <string_view>

class Solution final {
  private:
    static constexpr size_t _kdefault_min_string_len{2ul};
    static constexpr size_t _kdefault_max_string_len{100000ul};

    constexpr bool _isStringBad(std::string_view s) {
        return s.empty() || s.length() > _kdefault_max_string_len;
    }

  public:
    std::string makeFancyString(std::string_view s) {
        if (_isStringBad(s))
            return "";
        if (s.length() <= _kdefault_min_string_len)
            return std::string(s);

        std::string result;
        for (size_t i{}; i < s.length(); ++i) {
            if (i + 2ul < s.length() && (s.at(i) == s.at(i + 1ul)) &&
                (s.at(i + 1ul) == s.at(i + 2ul))) {
                continue;
            }
            result.push_back(s.at(i));
        }

        return result;
    }
};

#include <iostream>

int main() {
    Solution sln;
    std::cout << sln.makeFancyString("leeetcode") << '\n';
    std::cout << sln.makeFancyString("leeeetcooode") << '\n';
    std::cout << sln.makeFancyString("aaabaaaa") << '\n';
    std::cout << sln.makeFancyString("aab") << '\n';

    return 0;
}
