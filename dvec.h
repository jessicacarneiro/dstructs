#ifndef __DVEC_H__
#define __DVEC_H__

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "log.h"

/**
 * A dynamically allocated vector data structure.
 *
 * dvec_struct(int) vec;
 * dvec_init(vec, 16);
 * dvec_append(vec, 1);
 * for(int i = 0; i < vec.sz; i++) {
 *   printf("vec.data[%d] = %d\n", i, vec.data[i]);
 * }
 **/

#define dvec_struct(type) struct { size_t sz; size_t max; type *data; }

#define dvec_init(vector, initsz) { \
	assert(initsz > 0); \
	vector.sz = 0; vector.max = initsz; \
	vector.data = malloc(vector.max * sizeof(vector.data[0])); \
	if(!vector.data) logea(__FILE__, __LINE__, NULL); \
}

#define dvec_append(vector, value) { \
	vector.data[vector.sz] = value; \
	vector.sz++; \
	dvec_check_expand(vector); \
}

#define dvec_check_expand(vector) { \
	if(vector.sz == vector.max) { \
		vector.max << 1; \
		vector.data = realloc(vector.data, \
				vector.max * sizeof(vector.data[0])); \
		if(!vector.data) logea(__FILE__, __LINE__, NULL); \
	} \
}

#endif
