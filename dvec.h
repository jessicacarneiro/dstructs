#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

#define dvec_declare(name, type) struct name { size_t sz; size_t max; type *data; }

dvec_declare(i32vec, int32_t);
dvec_declare(u32vec, uint32_t);

#define dvec_struct(type) struct { size_t sz; size_t max; type *data; }

#define dvec_init(_v1) { \
	(_v1).sz = 0; \
	(_v1).max = 2; \
	(_v1).data = malloc((_v1).max * sizeof((_v1).data[0])); \
	if(!(_v1).data) logea(__FILE__, __LINE__, NULL); \
}

#define dvec_append(_v1, _value) { \
	dvec_reserve(_v1, 1); \
	(_v1).data[(_v1).sz] = _value; \
	(_v1).sz++; \
}

#define dvec_insert(_v1, _value, _idx) { \
	dvec_reserve(_v1, 1); \
	(_v1).sz++; \
	memmove((_v1).data + (_idx) + 1, (_v1).data + (_idx), \
			((_v1).sz - (_idx) - 1)*sizeof((_v1).data[0])); \
	(_v1).data[_idx] = _value; \
}

#define dvec_reserve(_v1, _space) \
	if((_v1).sz + (_space) >= (_v1).max) { \
		while((_v1).sz + (_space) >= (_v1.max)) (_v1).max <<= 1; \
		(_v1).data = realloc((_v1).data, \
				(_v1).max * sizeof((_v1).data[0])); \
		if(!(_v1).data) logea(__FILE__, __LINE__, NULL); \
	} \

#define dvec_memcmp(_v1, _v2) ((_v1).sz != ((_v2).sz) ? \
	((_v1).sz > (_v2).sz) - ((_v1).sz < (_v2).sz) : \
	memcmp((_v1).data, (_v2).data, (_v1).sz * sizeof((_v1).data[0])))
