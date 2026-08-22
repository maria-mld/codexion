/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:24 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:25 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_request) * capacity);
	if (!h->data)
		return (0);
	h->size = 0;
	h->capacity = capacity;
	return (1);
}

void	heap_destroy(t_heap *h)
{
	if (h->data)
		free(h->data);
	h->data = NULL;
	h->size = 0;
	h->capacity = 0;
}

int	heap_push(t_heap *h, int coder_id, long key, long order)
{
	if (h->size >= h->capacity)
		return (0);
	h->data[h->size].coder_id = coder_id;
	h->data[h->size].key = key;
	h->data[h->size].order = order;
	h->size++;
	heap_sift_up(h, h->size - 1);
	return (1);
}

int	heap_pop(t_heap *h, t_request *out)
{
	if (h->size == 0)
		return (0);
	*out = h->data[0];
	h->size--;
	h->data[0] = h->data[h->size];
	heap_sift_down(h, 0);
	return (1);
}
