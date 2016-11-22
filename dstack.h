#ifndef __DSTACK_H__
#define __DSTACK_H__

struct dstack {
  struct dnode *top;
  struct dnode *bottom;
  int count;
};

struct dnode {
  struct dnode *prev;
  struct dnode *next;
  void *data;
};

typedef void (*dstack_data_func)(void *data);
typedef int (*dstack_cmp_func)(const void *e1, const void *e2, void *userdata); 

struct dstack *dstack_create(void);
void dstack_destroy(struct dstack *dl, dstack_data_func);

void *dstack_pop(struct dstack *dl);
void *dstack_push(struct dstack *dl, void *data);

/* this function calls =cmp to compare =data and each value in =dl.  if a
 * match is found, it is removed from the stack and its pointer is returned.
 * returns NULL if no match is found. */
void *dstack_find_remove(struct dstack *dl, void *data, dstack_cmp_func cmp,
    void *userdata);

int dstack_empty(struct dstack *dl);

/* gets the data at index =idx.  =idx can be negative. */
void * dstack_get_index(const struct dstack *dl, int idx);
/* changes the data at index =idx.  does nothing if =idx does not exist. */
void dstack_set_index(struct dstack *dl, int idx, void *data);

#endif
