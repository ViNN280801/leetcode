#include <stdbool.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

bool hasCycle(struct ListNode *head)
{
    // If there is no elems or only 1 elem
    if (!head || !head->next)
        return false;

    // Init ptrs observers
    struct ListNode *first = head,
                    *second = head->next;
    
    // While there is any not null data in ptr `second` and next
    while (second && second->next)
    {
        // If ptrs turn be eq -> this is cycle
        if (first == second)
            return true;
        
        // Moving ptrs step by step
        first = first->next;
        second = second->next->next;
    }
    return false;
}

