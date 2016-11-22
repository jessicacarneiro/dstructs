#include <stdlib.h>
#include <assert.h>

#include "dstack.h"

struct dstack *dstack_create(void) /* {{{ */
{
  struct dstack *dl = malloc(sizeof(struct dstack));
  assert(dl);
  dl->top = NULL;
  dl->bottom = NULL;
  dl->count = 0;
  return dl;
} /* }}} */

void dstack_destroy(struct dstack *dl, dstack_data_func cb) /* {{{ */
{
  while(!dstack_empty(dl)) {
    void *data = dstack_pop(dl);
    if(cb) cb(data);
  }
  free(dl);
} /* }}} */

void *dstack_pop(struct dstack *dl) /* {{{ */
{
  void *data;
  struct dnode *node;

  if(dstack_empty(dl)) return NULL;

  node = dl->top;

  dl->top = node->next;
  if(dl->top == NULL) dl->bottom = NULL;
  if(node->next) node->next->prev = NULL;

  data = node->data;
  free(node);

  dl->count--;
  assert(dl->count >= 0);
  return data;
} /* }}} */

void *dstack_push(struct dstack *dl, void *data) /* {{{ */
{
  struct dnode *node = malloc(sizeof(struct dnode));
  assert(node);

  node->data = data;
  node->prev = NULL;
  node->next = dl->top;
  dl->top = node;

  if(dl->bottom == NULL) dl->bottom = dl->top;

  dl->count++;
  return data;
} /* }}} */

void *dstack_find_remove(struct dstack *dl, void *data, /* {{{ */
    dstack_cmp_func cmp, void *user_data)
{
  struct dnode *curr;
  for(curr = dl->top; curr; curr = curr->next) {
    if(!curr->data) continue;
    if(cmp(curr->data, data, user_data)) continue;
    void *ptr = curr->data;
    if(dl->top == curr) dl->top = curr->next;
    if(dl->bottom == curr) dl->bottom = curr->prev;
    if(curr->prev) curr->prev->next = curr->next;
    if(curr->next) curr->next->prev = curr->prev;
    dl->count--;
    free(curr);
    return ptr;
  }
  return NULL;
} /* }}} */

int dstack_empty(struct dstack *dl) /* {{{ */
{
  if(dl->top == NULL) {
    assert(dl->bottom == NULL);
    assert(dl->count == 0);
    return 1;
  } else {
    assert(dl->bottom != NULL);
    assert(dl->count > 0);
    return 0;
  }
} /* }}} */

void * dstack_get_index(const struct dstack *dl, int idx) /* {{{ */
{
  struct dnode *curr;
  if(idx >= 0) {
    curr = dl->top;
    while(curr && idx--) curr = curr->next;
  } else {
    curr = dl->bottom;
    while(curr && ++idx) curr = curr->prev;
  }
  if(!curr) return NULL;
  return curr->data;
} /* }}} */

void dstack_set_index(struct dstack *dl, int idx, void *data) /* {{{ */
{
  struct dnode *curr;
  if(idx >= 0) {
    curr = dl->top;
    while(curr && idx--) curr = curr->next;
  } else {
    curr = dl->bottom;
    while(curr && ++idx) curr = curr->prev;
  }
  if(!curr) return;
  curr->data = data;
} /* }}} */
