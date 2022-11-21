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

// create a new vla with an initial length
vla_t *VLA_new(size_t initial_length);

// free the pointer and the array it stores
void VLA_free(vla_t *self);

// return the element at index, return null if index is outside of range
char *VLA_get(vla_t *self, unsigned int index);

// set the element at index, return 0 if it worked
int VLA_set(vla_t *self, unsigned int index, char *element);

// insert element at index in self, return 0 if it worked
int VLA_insert(vla_t *self, unsigned int index, char *element);

// append element to self, return 0 if it worked
int VLA_append(vla_t *self, char *element);

// prepend element to self, return 0 if it worked
int VLA_prepend(vla_t *self, char *element);

// remove the element at index and shift the array left, return 0 if it worked
int VLA_remove(vla_t *self, unsigned int index);

// print the elements of the array
void VLA_print(vla_t *self);

#endif // TODO_VLA_H
