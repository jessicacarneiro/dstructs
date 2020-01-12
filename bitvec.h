#pragma once

#include <stdlib.h>

struct bitvec {
	unsigned *v;
	size_t nbits;
	size_t nwords;
};

struct bitvec * bitvec_create(size_t bits);
void bitvec_destroy(struct bitvec *vec);

/* REQUIRES sizeof(unsigned) == 4 */
#define bitvec_word_bits 32
#define bitvec_word_logbits 5
inline void bitvec_set(struct bitvec *vec, size_t i)
{
	size_t w = i >> bitvec_word_logbits;
	unsigned mask = 1 << (i & (bitvec_word_bits - 1));
	vec->v[w] |= mask;
}
inline unsigned bitvec_test(struct bitvec *vec, size_t i)
{
	size_t w = i >> bitvec_word_logbits;
	unsigned mask = 1 << (i & (bitvec_word_bits - 1));
	return vec->v[w] & mask;
}
inline unsigned bitvec_test_set(struct bitvec *vec, size_t i)
{
	size_t w = i >> bitvec_word_logbits;
	unsigned mask = 1 << (i & (bitvec_word_bits - 1));
	unsigned ret = vec->v[w] & mask;
	vec->v[w] |= mask;
	return ret;
}

/* This function calls `cb` for each bit set in `vec`.  It returns
 * when `cb` returns nonzero or when all bits in `vec` have been
 * passed to `cb`.  Pointer `data` is passed as the second parameter
 * to `cb`. */
void bitvec_iterate(struct bitvec *vec, int (*cb)(size_t, void *), void *data);
