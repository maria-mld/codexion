/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:06 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:07 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	check_burnout(t_sim *sim)
{
	int		index;
	long	last_start;

	index = 0;
	while (index < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[index].data_lock);
		last_start = sim->coders[index].last_compile_start;
		pthread_mutex_unlock(&sim->coders[index].data_lock);
		if (get_timestamp_ms() >= last_start + sim->args.time_to_burnout)
			return (announce_burnout(sim, sim->coders[index].id), 1);
		index++;
	}
	return (0);
}

static int	check_all_done(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->args.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[index].data_lock);
		if (sim->coders[index].compiles_done
			< sim->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->coders[index].data_lock);
			return (0);
		}
		pthread_mutex_unlock(&sim->coders[index++].data_lock);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!sim_is_stopped(sim))
	{
		if (check_burnout(sim) || check_all_done(sim))
			return (stop_simulation(sim), NULL);
		usleep(1000);
	}
	return (NULL);
}
