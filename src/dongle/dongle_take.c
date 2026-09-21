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

static int	take_single_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->left->lock);
	coder->left->is_taken = 1;
	pthread_mutex_unlock(&coder->left->lock);
	log_action(coder->sim, coder->id, "has taken a dongle");
	while (!sim_is_stopped(coder->sim))
		usleep(1000);
	release_one(coder->left);
	return (0);
}

static int	try_take_both(t_coder *c, t_dongle *f, t_dongle *s)
{
	long	cd;

	cd = c->sim->args.dongle_cooldown;
	pthread_mutex_lock(&f->lock);
	pthread_mutex_lock(&s->lock);
	if (!f->is_taken && !s->is_taken && !cooldown_active(f, cd)
		&& !cooldown_active(s, cd) && has_dongle_priority(f, c)
		&& has_dongle_priority(s, c))
	{
		f->is_taken = 1;
		s->is_taken = 1;
		heap_remove(&f->wait_queue, c->id);
		heap_remove(&s->wait_queue, c->id);
		pthread_mutex_unlock(&s->lock);
		pthread_mutex_unlock(&f->lock);
		log_action(c->sim, c->id, "has taken a dongle");
		log_action(c->sim, c->id, "has taken a dongle");
		return (1);
	}
	pthread_mutex_unlock(&s->lock);
	pthread_mutex_unlock(&f->lock);
	return (0);
}

static void	enqueue_dongles(t_coder *c, t_dongle *f, t_dongle *s)
{
	long	order;

	pthread_mutex_lock(&f->lock);
	order = f->next_order++;
	heap_push(&f->wait_queue, c->id, dongle_key(c), order);
	pthread_mutex_unlock(&f->lock);
	pthread_mutex_lock(&s->lock);
	order = s->next_order++;
	heap_push(&s->wait_queue, c->id, dongle_key(c), order);
	pthread_mutex_unlock(&s->lock);
}

static void	dequeue_dongles(t_coder *c, t_dongle *f, t_dongle *s)
{
	pthread_mutex_lock(&f->lock);
	heap_remove(&f->wait_queue, c->id);
	pthread_mutex_unlock(&f->lock);
	pthread_mutex_lock(&s->lock);
	heap_remove(&s->wait_queue, c->id);
	pthread_mutex_unlock(&s->lock);
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
	enqueue_dongles(coder, first, second);
	while (!sim_is_stopped(coder->sim))
	{
		if (try_take_both(coder, first, second))
			return (1);
		usleep(1000);
	}
	dequeue_dongles(coder, first, second);
	return (0);
}
