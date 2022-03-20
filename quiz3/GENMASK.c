#define GENMASK(h, l)                                                          \
    (((~0UL) >> (63-h)) & ((~0UL) >> l << l)