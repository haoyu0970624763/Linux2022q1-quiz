#include <stddef.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *deleteDuplicates(struct ListNode *head)
{

    if (!head)
    {
        return NULL;
    }

    /* Deal with the duplicate case appear in head */
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
            /* if no duplicate case appear in head , then out of the while loop*/
            break;
        }
    }

    if (!head)
    {
        return NULL;
    }
    /* use current to traverse the list */
    struct ListNode *current = head;

    /* In here , first node will definitely not equal to second node , if there has second node */
    /* if list have just one node , we just return it , otherwise we deal with it */
    while (current->next)
    {
        /*
            Use variable prev to record now we traverse
            and simply check two nodes behind it are duplicate or not

            if two nodes behind it are duplicate , deal with it
            otherwise just traverse next node
        */
        if (current->next->next && current->next->val == current->next->next->val)
        {
            struct ListNode *prev = current;
            int same_value = current->next->val;
            current = current->next;
            while (current && current->val == same_value)
            {
                struct ListNode *tmp = current;
                current = current->next;
                /* free the memory allocated to the duplicate  node */
                free(tmp);
            }
            prev->next = current;
            current = prev;
        }
        else
        {
            current = current->next;
        }
    }
    return head;
}
