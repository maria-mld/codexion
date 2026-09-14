/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_take.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:55 by marmoldo          #+#    #+#             */
/*   Updated: 2026/09/14 15:34:19 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	take_one_dongle(t_dongle *dongle, t_coder *coder)
{
	long		order;
	t_request	top;

	pthread_mutex_lock(&dongle->lock);
	order = dongle->next_order++;
	if (!heap_push(&dongle->wait_queue, coder->id,
			dongle_key(coder), order))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	if (!wait_for_dongle(dongle, coder, order))
	{
		heap_remove(&dongle->wait_queue, coder->id);
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	heap_pop(&dongle->wait_queue, &top);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

static int	take_single_dongle(t_coder *coder)
{
	if (!take_one_dongle(coder->left, coder))
		return (0);
	log_action(coder->sim, coder->id, "has taken a dongle");
	while (!sim_is_stopped(coder->sim))
		usleep(1000);
	release_one(coder->left);
	return (0);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left == coder->right)
		return (take_single_dongle(coder));
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
	{
		release_one(first);
		return (0);
	}
	log_action(coder->sim, coder->id, "has taken a dongle");
	return (1);
}
