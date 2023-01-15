#include <iostream>

// You are given two non-empty linked lists representing two non-negative integers. The digits are
// stored in reverse order, and each of their nodes contains a single digit. Add the two numbers
// and return the sum as a linked list.

// You may assume the two numbers do not contain any leading zero, except the
// number 0 itself.

// Example 1:
// Input: l1 = [2,4,3], l2 = [5,6,4]
// Output: [7,0,8]
// Explanation: 342 + 465 = 807.

// Example 2:
// Input: l1 = [0], l2 = [0]
// Output: [0]

// Example 3:
// Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
// Output: [8,9,9,9,0,0,0,1]

// Constraints:
// 1) The number of nodes in each linked list is in the range [1, 100].
// 2) 0 <= Node.val <= 9
// 3) It is guaranteed that the list represents a number that does not have leading zeros.

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

void printList(ListNode *ln)
{
    while (ln != nullptr)
    {
        std::cout << ln->val << ' ';
        ln = ln->next;
    }
    endl(std::cout);
}

unsigned int size(ListNode *next)
{
    ListNode *tmp = next;

    unsigned int counter = 1U;

    while (tmp != nullptr)
    {
        counter++;
        tmp = tmp->next;
    }

    return counter;
}

class Solution
{
protected:
    inline bool isDigit(int val) const
    {
        return ((val >= 0) && (val <= 9)) ? true : false;
    }

    inline bool isNumberOfNodes(int val) const
    {
        return ((val >= 1) && (val <= 100)) ? true : false;
    }

    bool isListDataChecked(ListNode *l)
    {
        ListNode *tmp = l;

        while (tmp != nullptr)
        {
            if (!isDigit(tmp->val))
                return false;

            tmp = tmp->next;
        }

        return true;
    }

public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        if (!isNumberOfNodes(size(l1)) && !isNumberOfNodes(size(l2)))
            return nullptr;

        if (!isListDataChecked(l1) || !isListDataChecked(l2))
            return nullptr;

        int add{0};
        ListNode res(0), *cur{&res};

        // While 'l1' or 'l2' is not a "nullptr"
        while (l1 || l2 || add)
        {
            // If 'l1' is not a "nullptr" -> 'sum' += value, that stored in the 'l1'
            // And if 'l2' is not a "nullptr" -> 'sum' += value, that stored in the 'l2'
            // Then, adding up the additional 'add' value, that stored (in the future) 'sum / 10' value
            int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + add;
            add = sum / 10;

            cur->next = new ListNode(sum % 10);
            cur = cur->next;

            // If 'l1' is not a "nullptr" -> move 'l1' to 'l1->next'
            l1 = l1 ? l1->next : l1;
            l2 = l2 ? l2->next : l2;
        }

        return res.next;
    }
};

int main()
{
    ListNode *l1{new ListNode(2, new ListNode(4, new ListNode(3)))},
        *l2{new ListNode(5, new ListNode(6, new ListNode(4)))};

    std::cout << "1st:" << std::endl;
    printList(l1);
    printList(l2);

    Solution sln;
    ListNode *l3{sln.addTwoNumbers(l1, l2)};

    std::cout << "Result:" << std::endl;
    printList(l3);

    delete l3;
    l3 = nullptr;

    delete l1;
    delete l2;
    l1 = nullptr;
    l2 = nullptr;

    ListNode *l4{new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9)))))))))},
        *l5{new ListNode(9, new ListNode(9, new ListNode(9, new ListNode(9))))};

    std::cout << "\n2nd:" << std::endl;
    printList(l4);
    printList(l5);

    ListNode *l6{sln.addTwoNumbers(l4, l5)};

    std::cout << "Result:" << std::endl;
    printList(l6);

    delete l6;
    l6 = nullptr;

    delete l4;
    delete l5;
    l4 = nullptr;
    l5 = nullptr;

    return EXIT_SUCCESS;
}
