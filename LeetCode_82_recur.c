#include <stddef.h>

struct ListNode {
  int val;
  struct ListNode *next;
};

struct ListNode *deleteDuplicates(struct ListNode *head) {
  /* node is empty*/
  if (!head)
    return NULL;

  /* node is not empty*/
  /* this node->val is equal to node->next->val */
  if (head->next && head->val == head->next->val) {

    /* check there are more than one repeated number equal to head node */
    while (head->next && head->val == head->next->val) {
      struct ListNode *tmp = head;
      head = head->next;
      /* free the memory allocated to the duplicate  node */
      free(tmp);
    }
    struct ListNode *tmp = head;
    head = head->next;
    free(tmp);
    // return the node which val is not equal to head node
    return deleteDuplicates(head);
  }
  /*
      this node->val is not equal to node->next->val
      directly return the next node
  */
  head->next = deleteDuplicates(head->next);
  return head;
}