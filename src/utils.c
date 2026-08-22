/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:11:15 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:11:16 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	sim_is_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

int	sleep_interruptible(t_sim *sim, long duration)
{
	long	start;
	long	remaining;

	start = get_timestamp_ms();
	while (!sim_is_stopped(sim))
	{
		remaining = duration - (get_timestamp_ms() - start);
		if (remaining <= 0)
			return (1);
		if (remaining > 1)
			usleep(1000);
		else
			usleep((useconds_t)(remaining * 1000));
	}
	return (0);
}
