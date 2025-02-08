#include <set>
#include <stdexcept>
#include <unordered_map>

class NumberContainers {
    std::unordered_map<int, int> index_to_num;
    std::unordered_map<int, std::set<int>> num_to_indices;

  public:
    NumberContainers() = default;

    void change(int index, int number) {
        if (index < 1 || index > 1e9)
            throw std::invalid_argument(
                "Index is invalid. Valid range is: [1, 10^9].");
        if (number < 1 || number > 1e9)
            throw std::invalid_argument(
                "Number is invalid. Valid range is: [1, 10^9].");

        if (auto it = index_to_num.find(index); it != index_to_num.end()) {
            int old_num = it->second;
            num_to_indices[old_num].erase(index);
            if (num_to_indices[old_num].empty()) {
                num_to_indices.erase(old_num);
            }
        }

        index_to_num[index] = number;
        num_to_indices[number].insert(index);
    }

    int find(int number) {
        if (number < 1 || number > 1e9)
            throw std::invalid_argument(
                "Number is invalid. Valid range is: [1, 10^9].");

        if (auto it = num_to_indices.find(number); it != num_to_indices.end()) {
            return *it->second.begin();
        }
        return -1;
    }
};
