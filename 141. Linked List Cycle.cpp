class Solution 
{
public:
    bool hasCycle(ListNode *head) 
    {
        if (!head || !head->next)
            return false;

        ListNode *pa = head,
                 *pb = head->next;

        while (pb && pb->next)
        {
            if (pa == pb)
                return true;
            pa = pa->next;
            pb = pb->next->next;
        }
        return false;
    }
};

