#include "vla.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vla_t *VLA_new(size_t allocated) {
  vla_t *res;

  res = malloc(sizeof(vla_t));
  res->data = calloc(allocated, sizeof(char *));
  res->length = 0;
  res->_allocated = allocated;
  return res;
}

void VLA_free(vla_t *self) {
  free(self->data);
  free(self);
}

static void VLA_expand(vla_t *self) {
  char **data;
  size_t new_allocated;

  new_allocated = self->_allocated * 2;
  data = realloc(self->data, new_allocated);

  self->data = data;
  self->_allocated = new_allocated;
}

int VLA_insert(vla_t *self, unsigned int index, char *element) {
  if (index > self->length) {
    return 1;
  }

  if (self->_allocated - self->length <= 1) {
    VLA_expand(self);
  }

  memmove(&self->data[index + 1], &self->data[index],
          (self->length - index) * sizeof(char *));

  self->data[index] = element;
  self->length += 1;
  return 0;
}

int VLA_append(vla_t *self, char *element) {
  return VLA_insert(self, self->length, element);
}

int VLA_remove(vla_t *self, unsigned int index) {
  if (index >= self->length) {
    return 1;
  }

  memmove(&self->data[index], &self->data[index + 1],
          (self->length - index) * sizeof(char *));
  self->length -= 1;
  return 0;
}
