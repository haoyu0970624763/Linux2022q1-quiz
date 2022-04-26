#include <stdbool.h>
#include <stddef.h>

struct ListNode {
  int val;
  struct ListNode *next;
  struct ListNode *prev;
};
struct ListNode *deleteDuplicates(struct ListNode *head) {
  if (!head)
    return NULL;

  struct ListNode **ptr = &head;
  struct ListNode *current = head;

  while (current) {
    if (current->next && current->val == current->next->val) {
      struct ListNode *tmp = current->prev;
      while (current->next && current->val == current->next->val)
        current = current->next;

      tmp->next = current->next;
      current = current->next;
      current->prev = tmp;

      *ptr = current;
    } else {
      ptr = &(*ptr)->next;

      if (current->next) {
        current = current->next;
      } else {
        current->next = head;
      }
    }
  }
  return head;
}