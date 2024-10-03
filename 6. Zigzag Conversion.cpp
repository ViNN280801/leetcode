#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

class Solution final
{
private:
    /**
     * @brief Checks the input string and number of rows.
     *        Restrictions:
     *          1. 1 <= s.length <= 1000;
     *          2. `s` consists of Eng letters or `,`, or `.`;
     *          3. 1 <= numRows <= 1000. 
     *
     * @param s Input string to check.
     * @param numRows Number of the rows to make zigzag from the string.
     * @return `false` if input is wrong according to restrictions, 
     *         `true` if all restrictions are met.
     */
    constexpr bool _isInputOk(std::string_view s, int numRows)
    {
        //// Time complexity: O(1), because size is an internal variable of the string container.
        if ((s.empty() || s.length() > 1000) ||
            (numRows < 1 || numRows > 1000))
            return false;
        
        //// Time complexity: O(N), because traversing all over the string.
        return std::all_of(s.cbegin(), s.cend(), [](char c){
            return isalpha(c) || c == '.' || c == ',';
        });
    }

public:
    //// Resulting time complexity: O(N).
    //// Resulting space complexity: O(N).
    std::string convert(std::string_view s, int numRows) 
    {
        // 1) Perform checkings.
        if (!_isInputOk(s, numRows))
            return "";
        if (numRows == 1 || (s.length() == 1 || s.length() == 2))
            return std::string(s.data());
        
        // 2) Initializing vars.
        std::vector<std::string> rows(numRows);
        int curRow{};
        bool goingDown{};

        // 3) Iterate all over the string symbols.
        //// Time complexity: O(N).
        for (size_t i{}; i < s.length(); ++i)
        {
            // 3.1) Adding current symbol of the string to current row.
            //// Time complexity (on average for `std::vector`): O(1)
            rows.at(curRow).append(1ul, s.at(i));

            // 3.2) Changing direction if reach first row or last row.
            goingDown = ((curRow == 0) || (curRow == numRows - 1)) ? !goingDown : goingDown; 
        
            // 3.2) If going down, adding 1 to row, if not - decrementing it.
            curRow = (goingDown) ? curRow + 1 : curRow - 1;
        }

        // 4) Assemblying rows to the one string.
        std::string result;
        result.reserve(s.length());
        for (std::string const &row : rows)
            result += row;

        return result;
    }
};
