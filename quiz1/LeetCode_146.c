
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

char indexTable[100001];

typedef struct {
  int capacity, count;
  struct list_head dhead, hheads[];
} LRUCache;

typedef struct {
  int key, value;
  struct list_head hlink, dlink;
} LRUNode;

LRUCache *lRUCacheCreate(int capacity) {
  /*
   * LRUCache 須紀錄
   * 1.capicity 以及 當前資料數 (count)
   * 其空間需要 malloc(sizeof(*obj))
   * 2.hash table 的空間
   * malloc(capacity * sizeof(struct list_head))
   */
  for (int i = 0; i <= 100000; i++) {
    indexTable[i] = 0;
  }

  LRUCache *obj = malloc(sizeof(*obj) + capacity * sizeof(struct list_head));
  obj->count = 0;
  obj->capacity = capacity;
  INIT_LIST_HEAD(&obj->dhead);
  for (int i = 0; i < capacity; i++)
    INIT_LIST_HEAD(&obj->hheads[i]);
  return obj;
}

void lRUCacheFree(LRUCache *obj) {
  LRUNode *lru, *n;
  list_for_each_entry_safe(lru, n, &obj->dhead, dlink) {
    list_del(&lru->dlink);
    free(lru);
  }
  free(obj);
}

int lRUCacheGet(LRUCache *obj, int key) {
  LRUNode *lru;

  if (indexTable[key] == 0) {
    return -1;
  }

  /*根據給定的 key 值去計算其應該在 Cache 中的哪個 entry */
  int hash = key % obj->capacity;
  /*
   * 根據計算出的 hash 值去尋找 cache 中 , 此 hash 值的  entry
   * 所以 &obj->hheads[hash] 表示 Cache 中對應的 entry 的 head
   * 而同一個 hash 值的 node 會用 linked list 串接起來 , 用 hlink 串連
   */
  list_for_each_entry(lru, &obj->hheads[hash], hlink) {
    if (lru->key == key) {
      /*
       *  如果有找到相同的 key 值 , 在這個瞬間此 key 是最近被存取到的
       *  把此 key 值放到 cache 的 Least Recently node 的最前頭(最近使用)
       *  dhead 為 cache last reference time 的串列 , 用 dlink 串接
       */
      list_move(&lru->dlink, &obj->dhead);
      return lru->value;
    }
  }
  return -1;
}

void lRUCachePut(LRUCache *obj, int key, int value) {

  LRUNode *lru;

  indexTable[key] = 1;
  /*
   * 根據給定的 key 值去計算其應該在 Cache 中的哪個 entry
   */
  int hash = key % obj->capacity;
  list_for_each_entry(lru, &obj->hheads[hash], hlink) {
    /*
     * 若有相同的 key 值存在於 Cache 當中
     * 則把此 key 值的 node 放到 cache 的 Least Recently node 的最前頭(最近使用)
     * 並修改其 value 為新 put 的 value
     */
    if (lru->key == key) {
      list_move(&lru->dlink, &obj->dhead);
      lru->value = value;
      return;
    }
  }
  /*
   * count 紀錄 Cache 中的資料數量
   * 如果 Cache 的資料數量 < capacity 則向記憶體要求 node 的空間
   * 否則的話 , 找出要取代的 node 並刪除在 linked list 中的 node
   * 要取代的 node 就是 Cache 中的 dhead 的 prev (也就是 tail )
   */
  if (obj->count == obj->capacity) {
    lru = list_last_entry(&obj->dhead, LRUNode, dlink);
    indexTable[lru->key] = 0;
    list_del(&lru->dlink);
    list_del(&lru->hlink);
  } else {
    lru = malloc(sizeof(LRUNode));
    obj->count++;
  }
  lru->key = key;
  list_add(&lru->dlink, &obj->dhead);
  list_add(&lru->hlink, &obj->hheads[hash]);
  lru->value = value;
}
