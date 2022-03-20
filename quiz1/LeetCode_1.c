#include <stddef.h>
#include <stdlib.h>

struct hlist_node {
  struct hlist_node *next, **pprev;
};
struct hlist_head {
  struct hlist_node *first;
};
typedef struct {
  int bits;
  struct hlist_head *ht;
} map_t;

#define MAP_HASH_SIZE(bits) (1 << bits)

map_t *map_init(int bits) {
  map_t *map = malloc(sizeof(map_t));
  if (!map)
    return NULL;

  map->bits = bits;
  map->ht = malloc(sizeof(struct hlist_head) * MAP_HASH_SIZE(map->bits));
  if (map->ht) {
    for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++)
      /*初始化每一個 hash table entry*/
      (map->ht)[i].first = NULL;
  } else {
    free(map);
    map = NULL;
  }
  return map;
}

struct hash_key {
  int key;
  void *data;
  struct hlist_node node;
};

#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    void *__mptr = (void *)(ptr);                                              \
    ((type *)(__mptr - offsetof(type, member)));                               \
  })

#define GOLDEN_RATIO_32 0x61C88647
static inline unsigned int hash(unsigned int val, unsigned int bits) {
  /* High bits are more random, so use them. */
  return (val * GOLDEN_RATIO_32) >> (32 - bits);
}

static struct hash_key *find_key(map_t *map, int key) {
  /* 去尋找特定 key 值有沒有存在於hash table
   * hash(key, map->bits) 是計算此 key 值應該在哪個 entry
   * 在對應的 entry 中依照 linked list 依序搜尋 key 值有無存在
   * */
  struct hlist_head *head = &(map->ht)[hash(key, map->bits)];
  for (struct hlist_node *p = head->first; p; p = p->next) {
    struct hash_key *kn = container_of(p, struct hash_key, node);
    if (kn->key == key)
      return kn;
  }
  return NULL;
}
void *map_get(map_t *map, int key) {
  struct hash_key *kn = find_key(map, key);
  return kn ? kn->data : NULL;
}

void map_add(map_t *map, int key, void *data) {
  /* 先看此 key 值在不在 hash table中 , 不在的話再加進去*/
  struct hash_key *kn = find_key(map, key);
  if (kn)
    return;
  /* 分配空間以及將資料複製到該節點 */
  kn = malloc(sizeof(struct hash_key));
  kn->key = key, kn->data = data;

  /*
   * h 為 指向 hash table 相對應之 entry 之位址的 poniter
   * n 為 指向上面分配的資料結構空間中的 node 位址的 pointer
   * first 為指向 hash table 相對應之 entry 的首個節點
   * */
  struct hlist_head *h = &map->ht[hash(key, map->bits)];
  struct hlist_node *n = &kn->node, *first = h->first;
  /*
   * n->next = first 把原本相對應之 entry 的首個節點接在新分配的節點後面
   *
   * if (first) first->pprev = &n->next
   * 如果原本相對應之 entry 的首個節點 不為空
   * 則把他的 pprev 改為 指向自己的 adderess 之 pointer
   *
   * h->first = n 改變相對應 entry 串列的頭
   * n->pprev 改為 指向自己的 adderess 之 pointer
   *
   * 其實可以改寫成
   * first->pprev = &first 以及 n->pprev = &n
   * */
  n->next = first;
  if (first)
    first->pprev = &n->next;
  h->first = n;
  n->pprev = &h->first;
}
void map_deinit(map_t *map) {
  if (!map)
    return;

  /* 造訪每一個 hash table entry*/
  for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++) {
    struct hlist_head *head = &map->ht[i];
    /* 造訪每一個 hash table entry 內部連結的節點 */
    for (struct hlist_node *p = head->first; p;) {
      struct hash_key *kn = container_of(p, struct hash_key, node);
      /* n 紀錄目前節點
       * p 則移至下一個節點（主要是用來決定 loop 是否繼續之依據） */
      struct hlist_node *n = p;
      p = p->next;

      /*
       * 從 map_add 我們可以發現 pprev 的意義是指向自己位址的 pointer
       * 所以若指向自己的位址為 null 則跳至 bail */
      if (!n->pprev) /* unhashed */
        goto bail;

      struct hlist_node *next = n->next, **pprev = n->pprev;
      *pprev = next;
      if (next)
        next->pprev = pprev;
      n->next = NULL, n->pprev = NULL;

    bail:
      free(kn->data);
      free(kn);
    }
  }
  free(map);
}
int *twoSum(int *nums, int numsSize, int target, int *returnSize) {
  /* 雜湊表初始化 , 其實 init(數字) 設成 10 以外的數字也可以
   * 雜湊表越大 , 平均尋找時間越短 , 但記憶體消耗更大
   * */
  map_t *map = map_init(10);
  *returnSize = 0;
  int *ret = malloc(sizeof(int) * 2);
  if (!ret)
    goto bail;

  for (int i = 0; i < numsSize; i++) {
    /*尋找此 input 對應之數字 有沒有在雜湊表
     * 例如： 要找相加為9 , input為2 就要找 7 有沒有在雜湊表*/
    int *p = map_get(map, target - nums[i]);
    if (p) { /* found */
      ret[0] = i, ret[1] = *p;
      *returnSize = 2;
      break;
    }
    /*如果對應之數字不在雜湊表 , 就把此 input 加至雜湊表 , 以供後續 input
     * 尋找相對應的數字*/
    p = malloc(sizeof(int));
    *p = i;
    map_add(map, nums[i], p);
  }

bail:
  map_deinit(map);
  return ret;
}