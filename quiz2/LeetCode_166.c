#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rem_node {
  int key;
  int index;
  struct list_head link;
};

static int find(struct list_head *heads, int size, int key) {
  struct rem_node *node;
  int hash = key % size;
  list_for_each_entry(node, &heads[hash], link) {
    if (key == node->key)
      return node->index;
  }
  return -1;
}

char *fractionToDecimal(int numerator, int denominator) {
  /* It is guaranteed that the length of the answer string is less than 104 for
   * all the given inputs so the size should be 10000+1
   */
  int size = 10001;
  char *result = malloc(size);
  char *p = result;

  /* 如果分母等於 0 , 則字串馬上接終止符號
   * In Leetcode , this case will never happen
   */
  if (denominator == 0) {
    result[0] = '\0';
    return result;
  }
  // 如果分子等於 0 , 字串為0 然後馬上接終止符號
  if (numerator == 0) {
    result[0] = '0';
    result[1] = '\0';
    return result;
  }

  /* using long long type make sure there has no integer overflow */
  long long n = numerator;
  long long d = denominator;

  /* deal with negtive cases */
  if (n < 0)
    n = -n;
  if (d < 0)
    d = -d;

  bool sign = (float)numerator / denominator >= 0;
  /* set negative sign */
  if (!sign)
    *p++ = '-';

  long long remainder = n % d;
  long long division = n / d;

  /* 原本是 sprintf(p, "%ld", division > 0 ? (long)division : (long)-division);
   * 但因為 n 跟 d 我們在上面處理過了有負號的情況
   * 所以這邊不需要再判斷正負號
   * */
  sprintf(p, "%ld", (long)division);
  if (remainder == 0)
    return result;

  /* move the pointer to the right position
   * and set the dot to the string in the right position
   */
  p = result + strlen(result);
  *p++ = '.';

  /* Using a map to record all of reminders and their position.
   * if the reminder appeared before, which means the repeated loop begin,
   */
  char *decimal = malloc(size);
  memset(decimal, 0, size);
  char *q = decimal;
  
  /* size means the hash table entry number
   * But I don't understand why we use 1333 this number
   */
  size = 1333;
  struct list_head *heads = malloc(size * sizeof(*heads));
  for (int i = 0; i < size; i++)
    INIT_LIST_HEAD(&heads[i]);

  for (int i = 0; remainder; i++) {
    int pos = find(heads, size, remainder);
    /* pos > 0 means the remainder has existed in hash table 
     * pos means the index number in hash table
     */
    if (pos >= 0) {
      while (pos-- > 0)
        *p++ = *decimal++;
      *p++ = '(';
      while (*decimal != '\0')
        *p++ = *decimal++;
      *p++ = ')';
      *p = '\0';
      return result;
    }
    struct rem_node *node = malloc(sizeof(*node));
    node->key = remainder;
    node->index = i;
    
    /* put the link to the right position in hash table entry*/
    list_add(&node->link, &heads[remainder % size]);

    *q++ = (remainder * 10) / d + '0';
    remainder = (remainder * 10) % d;
  }

  strcpy(p, decimal);
  return result;
}