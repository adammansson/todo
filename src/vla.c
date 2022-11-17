#include "vla.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vla_t *VLA_new(size_t length) {
  vla_t *res;

  res = malloc(sizeof(vla_t));
  res->_allocated = length * sizeof(char *);
  res->data = malloc(res->_allocated * sizeof(char *));
  res->length = 0;
  return res;
}

void VLA_free(vla_t *self) {
  unsigned int i;

  for (i = 0; i < self->length; i++) {
    free(self->data[i]);
  }
  free(self->data);
  free(self);
}

char *VLA_get(vla_t *self, unsigned int index) {
  if (index > self->length) {
    return NULL;
  }
  return self->data[index];
}

static void VLA_expand(vla_t *self) {
  char **new_data;
  size_t new_allocated;

  new_allocated = self->_allocated * 2;
  new_data = realloc(self->data, new_allocated);

  self->data = new_data;
  self->_allocated = new_allocated;
}

int VLA_insert(vla_t *self, unsigned int index, char *element) {
  if (index > self->length) {
    return 1;
  }

  if (self->_allocated - self->length * sizeof(char *) <= 0) {
    VLA_expand(self);
  }

  memmove(&self->data[index + 1], &self->data[index],
          (self->length - index) * sizeof(char *));

  self->data[index] = element;
  self->length++;
  return 0;
}

int VLA_append(vla_t *self, char *element) {
  return VLA_insert(self, self->length, element);
}

int VLA_prepend(vla_t *self, char *element) {
  return VLA_insert(self, 0, element);
}

int VLA_remove(vla_t *self, unsigned int index) {
  if (index >= self->length) {
    return 1;
  }

  memmove(&self->data[index], &self->data[index + 1],
          (self->length - index) * sizeof(char *));
  self->length--;
  return 0;
}

void VLA_print(vla_t *self) {
  unsigned int i;

  for (i = 0; i < self->length; i++) {
    printf("%s\n", self->data[i]);
  }
}
