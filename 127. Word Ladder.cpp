#include <algorithm>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

class Solution
{
private:
    int _bfs(std::string const &beginWord,
             std::string const &endWord,
             std::vector<std::string> const &wordList)
    {
        std::unordered_set<std::string> wordSet{wordList.cbegin(), wordList.cend()};
        if (wordSet.find(endWord) == wordSet.end())
            return 0;

        std::unordered_set<std::string> visited{beginWord};
        std::queue<std::pair<std::string, int>> queue;
        queue.emplace(beginWord, 1);

        while (!queue.empty())
        {
            auto [word, level] = queue.front();
            queue.pop();

            if (word == endWord)
                return level;

            for (size_t i{}; i < word.length(); ++i)
            {
                std::string newWord{word};
                for (char c{'a'}; c <= 'z'; ++c)
                {
                    newWord[i] = c;
                    if (wordSet.find(newWord) != wordSet.end() &&
                        visited.find(newWord) == visited.end())
                    {
                        visited.insert(newWord);
                        queue.emplace(newWord, level + 1);
                    }
                }
            }
        }
        return 0;
    }

public:
    int ladderLength(std::string const &beginWord,
                     std::string const &endWord,
                     std::vector<std::string> &wordList)
    {
        // Edge cases
        if (beginWord.empty() || endWord.empty() || wordList.empty())
            return 0;
        if (std::find(wordList.cbegin(), wordList.cend(), endWord) == wordList.cend())
            return 0;
        if (beginWord == endWord)
            return 0;

        // Lowercasing all the letters
        for (auto &w : wordList)
            std::transform(w.begin(), w.end(), w.begin(), [](char c)
                           { return std::tolower(c); });

        // Using BFS to find the shortest transformation sequence
        return _bfs(beginWord, endWord, wordList);
    }
};
