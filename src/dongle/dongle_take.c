/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_take.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:55 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:09:56 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	take_one_dongle(t_dongle *dongle, t_coder *coder)
{
	long		order;
	t_request	top;

	pthread_mutex_lock(&dongle->lock);
	order = dongle->next_order++;
	if (!heap_push(&dongle->wait_queue, coder->id, dongle_key(coder), order))
		return (pthread_mutex_unlock(&dongle->lock), 0);
	if (!wait_for_dongle(dongle, coder, order))
		return (pthread_mutex_unlock(&dongle->lock), 0);
	heap_pop(&dongle->wait_queue, &top);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left == coder->right)
		return (0);
	first = coder->left;
	second = coder->right;
	if (first->id > second->id)
	{
		first = coder->right;
		second = coder->left;
	}
	if (!take_one_dongle(first, coder))
		return (0);
	log_action(coder->sim, coder->id, "has taken a dongle");
	if (!take_one_dongle(second, coder))
		return (release_dongles(coder), 0);
	log_action(coder->sim, coder->id, "has taken a dongle");
	return (1);
}
