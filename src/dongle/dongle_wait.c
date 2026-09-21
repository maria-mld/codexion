/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:00 by marmoldo          #+#    #+#             */
/*   Updated: 2026/09/14 15:35:58 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

long	dongle_key(t_coder *coder)
{
	long	last_start;

	if (coder->sim->args.scheduler == SCHED_FIFO_MODE)
		return (0);
	pthread_mutex_lock(&coder->data_lock);
	last_start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->data_lock);
	if (coder->sim->args.time_to_burnout > LONG_MAX - last_start)
		return (LONG_MAX);
	return (last_start + coder->sim->args.time_to_burnout);
}

int	cooldown_active(t_dongle *dongle, long cd)
{
	long	elapsed;

	if (dongle->free_since == 0 || cd == 0)
		return (0);
	elapsed = get_timestamp_ms() - dongle->free_since;
	return (elapsed < cd);
}

int	has_dongle_priority(t_dongle *d, t_coder *coder)
{
	t_request	top;

	if (d->wait_queue.size == 0)
		return (1);
	heap_peek(&d->wait_queue, &top);
	return (top.coder_id == coder->id);
}
