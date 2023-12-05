#include <iostream>

class Solution 
{
public:
    int numberOfMatches(int n) 
    {
        int matches{};
        while (n != 1)
        {
            if ((n & 1) == 1)
            {
                n /= 2;
                matches += n;
                ++n;
            }
            else
            {
                n /= 2;
                matches += n;
            }
        }
        return matches;
    }
};

int main()
{
    Solution sln;
    std::cout << sln.numberOfMatches(7) << '\n';
    std::cout << sln.numberOfMatches(6) << '\n';
    std::cout << sln.numberOfMatches(14) << '\n';

    return EXIT_SUCCESS;
}

