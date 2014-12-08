#include <stdlib.h>

typedef enum {
    LIST_FORWARD,
    LIST_BACK
} list_direct_t;

typedef enum {
    LIST_BEG,
    LIST_CUR,
    LIST_END
} list_whence_t;

typedef struct {
    void  *ptr;
    void  *cur;
    void  *end;
    size_t i;
    size_t n;
    size_t size;
} list_t;

list_t *list_create(size_t n, size_t size);

int list_set(list_t *list, size_t offset, list_direct_t direct, list_whence_t whence);

void list_append(list_t *list, void *item);

void *list_next(list_t *list);

void *list_index(list_t *list, size_t index);

list_t *list_copy(list_t *src);

list_t *list_sort(list_t *list, int(cmp)(const void *, const void *));

void list_free(list_t *list);
