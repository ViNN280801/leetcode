/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
    // Checking boundary conditions
    if (!list1 && !list2)
        return NULL;
    if (!list1)
        return list2;
    if (!list2)
        return list1;

    // 2. Merging
    if (list1->val <= list2->val)
    {
        list1->next = mergeTwoLists(list1->next, list2);
        return list1;
    }
    if (list2->val <= list1->val)
    {
        list2->next = mergeTwoLists(list1, list2->next);
        return list2;
    }
    return list1;
}

