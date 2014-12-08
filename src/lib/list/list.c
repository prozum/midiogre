#include "list.h"

#include <stdlib.h>
#include <string.h>

list_t *list_create(size_t n, size_t size)
{
	list_t *list;

	/* Temp pointer for pointer arithmetic,
	 * since arithmetic on void pointers are undefined */
	char *tmp;

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
		
		if (direct == LIST_FORWARD && list->n > offset) {

			list->i = offset;
			
			tmp = list->ptr;

			list->cur = tmp + list->size * offset;

			return 0;

		}

		break;

	/* Current item */
	case LIST_CUR:

        if (direct == LIST_FORWARD &&
            list->n > list->i + offset) {

            list->i += offset;

            tmp = list->ptr;

            list->cur = tmp + list->size * offset;

            return 0;
        }

        if (direct == LIST_BACK &&
            list->i >= offset) {

            list->i -= offset;

            tmp = list->ptr;

            list->cur = tmp - list->size * offset;

            return 0;
        }

        if (list->i + offset > 0 &&
		    list->i + offset < list->n) {

			list->i += offset;

			tmp = list->ptr;

			list->cur = tmp + list->size * offset;
			
			return 0;
		} 

		break;

	/* Last item */
	case LIST_END:
		
		if (direct == LIST_BACK && list->n >offset) {
	
			list->i = list->n - 1 - offset;

			tmp = list->end;

			list->cur = tmp + list->size * offset;
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
	if (list->cur == list->end) {

		list->i++;
		list->cur = NULL;
		return list->end;

	/* If list is exceeded */
	} else if (list->cur == NULL) {

		return NULL;

	/* If normal */
	} else {

		/* Step index */
		list->i++;

		/* Set temp pointer to current item */
		tmp = list->cur;

		/* Set current pointer to next item */
		list->cur = tmp + list->size;

		return tmp;
	}
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

list_t *list_copy(list_t *src)
{
	list_t *dest;

	/* Create list */
	dest = list_create(src->n, src->size);

	/* Copy data */
	memcpy(dest->ptr, src->ptr, src->n * src->size);

	return dest;
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


void list_free(list_t *list)
{
	/* Free items */
	free(list->ptr);

	/* Free list */
	free(list);
}
