#include <stddef.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *deleteDuplicates(struct ListNode *head)
{
    

    while (head)
    {
        if (head->next && head->val == head->next->val)
        {
            while (head->next && head->val == head->next->val)
            {
                struct ListNode *tmp = head;
                head = head->next;
                /* free the memory allocated to the duplicate  node */
                free(tmp);
            }
            struct ListNode *tmp = head;
            head = head->next;
            free(tmp);
        }
        else
        {
            break;
        }
    }

    struct ListNode *tmp_head = head;
    

    while (head)
    {
        if (head->next && head->next->next && head->next->val == head->next->next->val)
        {
            struct ListNode *tmp = head;
            
            int same_value = head->next->val;
            head=head->next;
            while ( head && head->val == same_value)
            {
                head=head->next;
                /* free the memory allocated to the duplicate  node */
            }
            tmp-> next  = head ; 
            head=tmp;
            
        }
        else
        {
            head=head->next;
        }
    }

    return tmp_head;
}