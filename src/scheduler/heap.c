#include "../../include/codexion.h"

int heap_init(t_heap *h, int capacity)
{
    h->data = malloc(sizeof(t_request) * capacity);
    if (!h->data)
        return (0);
    h->size = 0;
    h->capacity = capacity;
    return (1);
}


void    heap_destroy(t_heap *h)
{
    if (h->data)
        free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

static void swap_req(t_request *a, t_request *b)
{
    t_request   tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void sift_up(t_heap *h, int i)
{
    int parent;

    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (h->data[parent].key <= h->data[i].key)
            break ;
        swap_req(&h->data[parent], &h->data[i]);
        i = parent;
    }
}

static void sift_down(t_heap *h, int i)
{
    int left;
    int right;
    int smallest;

    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < h->size && h->data[left].key < h->data[smallest].key)
            smallest = left;
        if (right < h->size && h->data[right].key < h->data[smallest].key)
            smallest = right;
        if (smallest == i)
            break ;
        swap_req(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

int heap_push(t_heap *h, int coder_id, long key)
{
    if (h->size >= h->capacity)
        return (0);
    h->data[h->size].coder_id = coder_id;
    h->data[h->size].key = key;
    h->size++;
    sift_up(h, h->size -1);
    return (1);
}


int heap_pop(t_heap *h, t_request *out)
{
    if (h->size == 0)
        return (0);
    *out = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    sift_down(h, 0);
    return (1);
}

int heap_is_empty(t_heap *h)
{
    return (h->size == 0);
}
