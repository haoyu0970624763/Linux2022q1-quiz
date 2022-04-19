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
      /* 初始化每一個雜湊表中的 entry */
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
  /* 去尋找特定 key 值有沒有存在於雜湊表
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
  /* 先看此 key 值是否存在於雜湊表中,不在的話再繼續 */
  struct hash_key *kn = find_key(map, key);
  if (kn)
    return;
  /* 分配空間以及將資料複製到該節點 */
  kn = malloc(sizeof(struct hash_key));
  kn->key = key, kn->data = data;

  /* h 指向雜湊表中相對應 entry 的位址
   * first 指向雜湊表中相對應 entry 的首個節點位址
   * n 指向剛分配的資料結構中的 node 位址
   * */
  struct hlist_head *h = &map->ht[hash(key, map->bits)];
  struct hlist_node *n = &kn->node, *first = h->first;
  /* n->next = first
   * 把原本雜湊表中相對應 entry 的首個節點接在新分配的節點後面
   *
   * if (first) first->pprev = &n->next
   * 如果原本雜湊表中相對應 entry 的首個節點存在的話
   * 將他的 pprev 指向 指向新分配的節點的 next 位址
   *
   * h->first = n
   * 改變雜湊表中相對應 entry 所指向的首個節點
   *
   * n->pprev = &h->first
   * 將新分配的節點的 pprev 指向 指向雜湊表中相對應 entry 的位址
   * 可搭配題目上方的圖示可以更好的理解此部份
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

  /* 造訪雜湊表中的每一個 entry*/
  for (int i = 0; i < MAP_HASH_SIZE(map->bits); i++) {
    struct hlist_head *head = &map->ht[i];
    /* 造訪雜湊表中的每一個 entry 內部連結的節點 */
    for (struct hlist_node *p = head->first; p;) {
      struct hash_key *kn = container_of(p, struct hash_key, node);
      /* n 紀錄目前節點
       * p 則移至下一個節點（主要是用來決定 loop 是否繼續之依據） */
      struct hlist_node *n = p;
      p = p->next;

      /* 如果當前的節點的 pprev 為空(非正常行為)
       * 跳至 bail 標籤 , 否則的話處理以下行為
       * */
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
  /* 雜湊表的建立 , 設置雜湊表的 entry 總數為 1024
   * (要設置除了 1024 以外的數字也可以)
   * 通常來說雜湊表 entry 數越多 , 平均尋找時間越短 ,但記憶體消耗更大
   * */
  map_t *map = map_init(10);
  *returnSize = 0;
  int *ret = malloc(sizeof(int) * 2);
  /* if mallocing the memory of return address fail , direct to the end of code
   */
  if (!ret)
    goto bail;

  for (int i = 0; i < numsSize; i++) {
    /* 尋找當前 input 對應 target 之數字是否存在於雜湊表中
     * 例如：要找兩數相加為9 , 當前 input 為 2 , 就要找 7
     * 這個數字是否存在於雜湊表
     * */
    int *p = map_get(map, target - nums[i]);
    if (p) { /* found */
      ret[0] = i, ret[1] = *p;
      *returnSize = 2;
      break;
    }
    /* 如果當前 input 對應 target 之數字不存在於雜湊表中
     * 就把此 input 加至雜湊表裡面
     * */
    p = malloc(sizeof(int));
    *p = i;
    map_add(map, nums[i], p);
  }

bail:
  map_deinit(map);
  return ret;
}