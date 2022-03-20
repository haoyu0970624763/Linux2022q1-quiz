unsigned long gcd(unsigned long u, unsigned long v) {

  unsigned long r = u | v;
  /* if a = 0 or b = 0 , then return a | b
   * which means return the number is not zero
   */
  if (!u || !v)
    return r;

  v >>= __ffs(v);
  if (v == 1)
    return r & -r;

  for (;;) {
    /*check whether u is the form of 2^k
     * if so , just return r & -r , like the code above it*/
    u >>= __ffs(u);
    if (u == 1)
      return r & -r;

    /* 如果提出 2的冪次方之後的 u 等於 提出 2的冪次方之後的 v
     * 則表示他們的最大公因數為
     * 提出之後的結果 乘上 兩數分別提出的冪次方中較小的2的冪次方
     * 即 v << __ffs(r)  要寫成 u << __ffs(r) 也行
     * */
    if (u == v)
      return v << __ffs(r);

    /* 確保 u >= v  , 使 u-=b 成立
     * 且運用 gcd(x, y) = gcd(x - y, y)
     * */

    if (u < v)
      swap(u, v);
    u -= v;
  }
}