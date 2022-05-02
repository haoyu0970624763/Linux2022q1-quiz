#include "list.h"
#include <stdio.h>
#include <stdlib.h>

struct seq_node {
  int num;
  struct list_head link;
};

struct seq_node {
  int num;
  struct list_head link;
};

static struct seq_node *find(int num, int size, struct list_head *heads) {
  struct seq_node *node;
  int hash = num < 0 ? -num % size : num % size;
  list_for_each_entry(node, &heads[hash], link) {
    if (node->num == num)
      return node;
  }
  return NULL;
}

int longestConsecutive(int *nums, int n_size) {
  int hash, length = 0;
  struct seq_node *node;
  /* heads 為 size 為 n_size 的 hash table*/
  struct list_head *heads = malloc(n_size * sizeof(*heads));

  /* 初始化每一個 hash table entry */
  for (int i = 0; i < n_size; i++)
    INIT_LIST_HEAD(&heads[i]);
  /* 如果 input 的數字不在相對應 hash table 的 entry 中
   * 則把此數字放入 hash table entry 的串列當中
   */
  for (int i = 0; i < n_size; i++) {
    if (!find(nums[i], n_size, heads)) {
      /* 因為 input 不一定為正數 , 所以如果是負數要先加負號再進行 hash */
      hash = nums[i] < 0 ? -nums[i] % n_size : nums[i] % n_size;
      node = malloc(sizeof(*node));
      node->num = nums[i];
      list_add(&node->link, &heads[hash]);
    }
  }

  for (int i = 0; i < n_size; i++) {
    int len = 0;
    int num;

    /* 先找一個 input 存不存在於hash table
     * 第一次跑 loop 時 , 肯定存在
     */
    node = find(nums[i], n_size, heads);
    if (node) {
      /* 若此 input 存在於 hash table中
       * 則 Longest Consecutive Sequence​長度+1
       * 先用 num 紀錄此數字
       * 並在串列中刪除此數 , 用意是避免之後重複的搜尋
       */
      len++;
      num = node->num;
      list_del(&node->link);

      int left = num, right = num;
      /* 找 num 相鄰的數字(負的方向)
       * 找到的話在串列中刪除此數 , 用意是避免之後重複的搜尋
       */
      while ((node = find(--left, n_size, heads))) {
        len++;
        list_del(&node->link);
      }
      /* 找 num 相鄰的數字(正的方向)
       * 找到的話在串列中刪除此數 , 用意是避免之後重複的搜尋
       */
      while ((node = find(++right, n_size, heads))) {
        len++;
        list_del(&node->link);
      }

      /* 判斷目前搜尋的相鄰數字長度有沒有大於當前之最大值 */
      length = len > length ? len : length;
    }
  }

  return length;
}