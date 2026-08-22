/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:11:03 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:11:04 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void	wake_waiters(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[index].lock);
		pthread_cond_broadcast(&sim->dongles[index].cond);
		pthread_mutex_unlock(&sim->dongles[index].lock);
		index++;
	}
}

void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
	wake_waiters(sim);
}

void	announce_burnout(t_sim *sim, int coder_id)
{
	long	timestamp;

	pthread_mutex_lock(&sim->stop_lock);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_lock);
		return ;
	}
	sim->stop = 1;
	timestamp = get_timestamp_ms() - sim->start_time;
	pthread_mutex_lock(&sim->log_lock);
	printf("%ld %d burned out\n", timestamp, coder_id);
	pthread_mutex_unlock(&sim->log_lock);
	pthread_mutex_unlock(&sim->stop_lock);
	wake_waiters(sim);
}

void	log_action(t_sim *sim, int coder_id, const char *action)
{
	long	timestamp;

	pthread_mutex_lock(&sim->stop_lock);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_lock);
		return ;
	}
	timestamp = get_timestamp_ms() - sim->start_time;
	pthread_mutex_lock(&sim->log_lock);
	printf("%ld %d %s\n", timestamp, coder_id, action);
	pthread_mutex_unlock(&sim->log_lock);
	pthread_mutex_unlock(&sim->stop_lock);
}
