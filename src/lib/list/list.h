/** @file list.h contains functions used to manipulate lists */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

#define EOL -10

typedef enum {
    LIST_FORW,
    LIST_BACK
} list_direct_t;

typedef enum {
    LIST_BEG,
    LIST_CUR,
    LIST_END
} list_whence_t;

typedef struct {
    void *ptr;   /**< Points to first element          */
    void *cur;   /**< Points to current element        */
    void *end;   /**< Points to last element           */
    size_t i;    /**< Numeric value of current element */
    size_t n;    /**< Ammount of elements              */
    size_t size; /**< Size of elements                 */
} list_t;

list_t *list_create(size_t n, size_t size);

int list_set(list_t *list, size_t offset, list_direct_t direct, list_whence_t whence);

void list_reset(list_t *list);

void list_append(list_t *list, void *item);

void *list_next(list_t *list);

int list_get(list_t *list);

size_t list_get_fixed(list_t *list, size_t buf_size);

void *list_index(list_t *list, size_t index);

list_t *list_copy(list_t *list);

list_t *list_slicing(list_t *list, size_t offset, size_t n);

list_t *list_sort(list_t *list, int(cmp)(const void *, const void *));

list_t *list_dump_file(FILE *file);

void list_free(list_t *list);

#endif
