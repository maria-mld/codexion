/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:12 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:13 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	heap_peek(t_heap *h, t_request *out)
{
	if (h->size == 0)
		return (0);
	*out = h->data[0];
	return (1);
}

int	heap_remove(t_heap *h, int coder_id)
{
	int	index;

	if (h->size == 0)
		return (0);
	index = 0;
	while (index < h->size && h->data[index].coder_id != coder_id)
		index++;
	if (index == h->size)
		return (0);
	h->size--;
	if (index < h->size)
	{
		h->data[index] = h->data[h->size];
		heap_sift_down(h, index);
		heap_sift_up(h, index);
	}
	return (1);
}
