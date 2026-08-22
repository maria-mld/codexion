/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:00 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:01 by marmoldo         ###   ########.fr       */
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

static int	cooldown_active(t_dongle *dongle, t_coder *coder)
{
	long	elapsed;

	if (dongle->free_since == 0 || coder->sim->args.dongle_cooldown == 0)
		return (0);
	elapsed = get_timestamp_ms() - dongle->free_since;
	return (elapsed < coder->sim->args.dongle_cooldown);
}

static void	wait_cooldown(t_dongle *dongle, t_coder *coder)
{
	struct timespec	deadline;
	long			target;

	target = dongle->free_since + coder->sim->args.dongle_cooldown;
	deadline.tv_sec = target / 1000;
	deadline.tv_nsec = (target % 1000) * 1000000L;
	pthread_cond_timedwait(&dongle->cond, &dongle->lock, &deadline);
}

int	wait_for_dongle(t_dongle *dongle, t_coder *coder, long order)
{
	t_request	top;

	while (!sim_is_stopped(coder->sim))
	{
		heap_peek(&dongle->wait_queue, &top);
		if (!dongle->is_taken && top.order == order)
		{
			if (cooldown_active(dongle, coder))
				wait_cooldown(dongle, coder);
			else
				return (1);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	return (0);
}
