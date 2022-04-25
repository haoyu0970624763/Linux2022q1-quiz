#include <stdbool.h>
#include <stddef.h>

struct ListNode {
  int val;
  struct ListNode *next;
};
struct ListNode *deleteDuplicates(struct ListNode *head) {
  if (!head)
    return NULL;

  struct ListNode **ptr= &head;
  struct ListNode *current = head;

  while (current) {
    if (current->next && current->val == current->next->val) {
      current = current->next;
      while (current->next && current->val == current->next->val)
        current = current->next;
      current = current->next;
      *ptr = current;
    } else {
      ptr = &(*ptr)->next;
      current = current->next;
    }
  }
  return head;
}