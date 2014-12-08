#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

list_t *list_create(size_t n, size_t size)
{
    list_t *list;

    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    unsigned char *tmp;

    /* Return NULL if size is invalid or malloc fails */
    if (size == 0 || (list = malloc(sizeof(list_t))) == NULL) {

        return NULL;
    };

    /* Allocate memory if list contains items */
    if (n > 0) {

        list->ptr = calloc(n, size);

        /* Assign to current & temp pointer */
        list->cur = tmp = list->ptr;

        /* Pointer to last item */
        list->end = tmp + (n - 1) * size;

    } else {

        list->ptr = NULL;
        list->cur = NULL;
        list->end = NULL;
    }

    list->i = 0;
    list->n = n;
    list->size = size;

    return list;
}

int list_set(list_t *list, size_t offset, list_direct_t direct, list_whence_t whence)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    char *tmp;

    switch (whence) {

    /* First item */
    case LIST_BEG:

        if (direct == LIST_FORW && list->n > offset) {

            list->i = offset;

            tmp = list->ptr;

            list->cur = tmp + list->size * offset;

            return 0;

        }

        break;

    /* Current item */
    case LIST_CUR:

        if (direct == LIST_FORW &&
            list->n > list->i + offset) {

            list->i += offset;

            tmp = list->cur;

            list->cur = tmp + list->size * offset;

            return 0;
        }

        if (direct == LIST_BACK &&
            list->i >= offset) {

            list->i -= offset;

            tmp = list->cur;

            list->cur = tmp - list->size * offset;

            return 0;
        }

        break;

    /* Last item */
    case LIST_END:

        if (direct == LIST_BACK && list->n > offset) {

            list->i = list->n - 1 - offset;

            tmp = list->end;

            list->cur = tmp - list->size * offset;

            return 0;
        }

        break;
    }

    return -1;
}

void list_append(list_t *list, void *item)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    char *tmp;

    /* One more item */
    list->n++;

    /* Reallocate pointer & assign to temp */
    list->ptr = tmp = realloc(list->ptr, list->n * list->size);

    /* Point to item i */
    list->cur = tmp + list->i * list->size;

    /* Point to last item */
    list->end = tmp + (list->n - 1) * list->size;

    /* Copy item to end of list */
    memcpy(list->end, item, list->size);
}

void *list_next(list_t *list)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    char *tmp;

    /* If last item */
    if (list->i == list->n - 1) {

        list->i++;
        list->cur = NULL;

        return list->end;
    }

    /* If list is exceeded */
    if (list->i == list->n) {

        return NULL;

    }

    /* If normal */
    list->i++;

    /* Set temp pointer to current item */
    tmp = list->cur;

    /* Set current pointer to next item */
    list->cur = tmp + list->size;

    return tmp;

}

int list_get(list_t *list)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    unsigned char *tmp;

    if (list->i >= list->n) {

        return EOL;

    }

    /* Only works with 1 byte lists */
    if (list->size != 1) {

        return -1;

    }

    tmp = list->cur;
    list->cur = tmp + list->size;

    list->i++;

    return *tmp;
}

/**
 * DANGER:
 * No way to check if end of list is reached...
 */
size_t list_get_fixed(list_t *list, size_t buf_size)
{
    size_t i,result = 0;

    /* Check buf_size */
    if (buf_size > sizeof(size_t) || buf_size == 0) {
        fprintf(stderr,"Buffer size invalid\n");
        return 0;
    }

    for(i = 1; i <= buf_size; i++) {
        result += list_get(list);

        if (i != buf_size) {
            result <<= 8;
        }
    }

    return result;
}


void *list_index(list_t *list, size_t index)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    char *tmp;

    /* If index value is valid */
    if (index <= list->n - 1) {

        tmp = list->ptr;

        return tmp + list->size * index;

    } else {

        return NULL;
    }
}

list_t *list_copy(list_t *list)
{
    list_t *copy_list;

    /* Create list */
    copy_list = list_create(list->n, list->size);

    /* Copy data */
    memcpy(copy_list->ptr, list->ptr, list->n * list->size);

    return copy_list;
}

list_t *list_slicing(list_t *list, size_t offset, size_t n)
{
    /* Temp pointer for pointer arithmetic,
     * since arithmetic on void pointers are undefined */
    char *tmp;

    list_t *sliced_list;

    sliced_list = list_create(n, list->size);

    tmp = list->ptr;

    memcpy(sliced_list->ptr, tmp + list->size * offset, n * list->size);

    return sliced_list;
}


list_t *list_sort(list_t *list, int(cmp)(const void *, const void *))
{
    list_t *sorted_list;

    /* Copy list */
    sorted_list = list_copy(list);

    /* Sort list */
    qsort(sorted_list->ptr, sorted_list->n, sorted_list->size, *cmp);

    return sorted_list;
}

list_t *list_dump_file(FILE *file)
{
    list_t *list;
    size_t n,rv;

    /* Count bytes in file*/
    fseek(file, 0, SEEK_END);
    n = ftell(file);

    /* Create byte list */
    list = list_create(n, sizeof(char));

    /* Read bytes */
    fseek(file, 0, SEEK_SET);
    if ((rv = fread(list->ptr, list->size, list->n, file)) != list->n) {
        fprintf(stderr, "Failed to read file on element %lu\n", (unsigned long) rv);
        list_free(list);
        return NULL;
    }

    /* Reset list */
    list_set(list, 0, LIST_FORW, LIST_BEG);

    return list;
}


void list_free(list_t *list)
{
    /* Free items */
    free(list->ptr);

    /* Free list */
    free(list);
}
