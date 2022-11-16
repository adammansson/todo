#ifndef TODO_VLA_H
#define TODO_VLA_H

#include <stddef.h>

typedef struct vla {
  // the array itself
  char **data;

  // number of elements in the array
  size_t length;

  // amount of elements currently allocated for the array
  size_t _allocated;
} vla_t;

// create a new vla with an initial amount of elements allocated
vla_t *VLA_new(size_t allocated);

// free the pointer and the array it stores
void VLA_free(vla_t *self);

// insert element at index in self, returns 0 if it worked
int VLA_insert(vla_t *self, unsigned int index, char *element);

// append element to self, returns 0 if it worked
int VLA_append(vla_t *self, char *element);

// remove the element at index and shift the array left, returns 0 if it worked
int VLA_remove(vla_t *self, unsigned int index);

#endif // TODO_VLA_H
