/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:15 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:16 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	comes_before(t_request a, t_request b)
{
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.order < b.order);
}

static void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_sift_up(t_heap *h, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!comes_before(h->data[index], h->data[parent]))
			break ;
		swap_req(&h->data[parent], &h->data[index]);
		index = parent;
	}
}

void	heap_sift_down(t_heap *h, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		smallest = index;
		if (left < h->size && comes_before(h->data[left], h->data[smallest]))
			smallest = left;
		if (right < h->size && comes_before(h->data[right], h->data[smallest]))
			smallest = right;
		if (smallest == index)
			break ;
		swap_req(&h->data[index], &h->data[smallest]);
		index = smallest;
	}
}
