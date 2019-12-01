#include "bitvec.h"

#include <assert.h>

struct bitvec * bitvec_create(size_t bits)
{
	assert(sizeof(unsigned) == 4);
	struct bitvec *vec = malloc(sizeof(*vec));
	if(!vec) return NULL;
	vec->nwords = (bits >> bitvec_word_logbits);
	vec->nwords += (bits & (bitvec_word_bits - 1)) ? 1 : 0;
	vec->nbits = bits;
	vec->v = calloc(vec->nwords, sizeof(unsigned));
	if(!vec->v) {
		free(vec);
		return NULL;
	}
	return vec;
}

void bitvec_destroy(struct bitvec *vec)
{
	free(vec->v);
	free(vec);
}

void bitvec_iterate(struct bitvec *vec, int (*cb)(size_t, void *), void *data)
{
	for(size_t w = 0; w < vec->nwords; w++) {
		unsigned word = vec->v[w];
		size_t base = w << bitvec_word_logbits;
		while(word) {
			int i = __builtin_ffs(word) - 1;
			if(0 != cb(base + i, data)) return;
			word ^= 1 << i;
		}
	}
}
