typedef struct tnode *tree;
struct tnode {
  int data;
  tnode *left;
  tnode *right;
  tnode(int d) {
    data = d;
    left = right = 0;
  }
};

void remove_data(tree &t, int d) {
  tnode **p = &t;
  while (*p != 0 && (*p)->data != d) {
    if (d < (*p)->data)
      p = &(*p)->left;
    else
      p = &(*p)->right;
  }
  tnode *q = *p;
  if (!q)
    return;

  if (!q->left)
    *p = q->right;
  else if (!q->right)
    *p = q->left;
  else {
    tnode **r = &q->right;
    while ((*r)->left)
      r = &(*r)->left;
    q->data = (*r)->data;
    q = *r;
    *r = q->right;
  }
  delete q;
}

