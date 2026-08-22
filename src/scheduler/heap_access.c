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

int	heap_is_empty(t_heap *h)
{
	return (h->size == 0);
}
