/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:42 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:43 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void	cleanup_dongles(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->dongles_initialized)
	{
		pthread_mutex_destroy(&sim->dongles[index].lock);
		pthread_cond_destroy(&sim->dongles[index].cond);
		heap_destroy(&sim->dongles[index].wait_queue);
		index++;
	}
	free(sim->dongles);
}

static void	cleanup_coders(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->coders_initialized)
		pthread_mutex_destroy(&sim->coders[index++].data_lock);
	free(sim->coders);
}

void	cleanup_sim(t_sim *sim)
{
	if (sim->dongles)
		cleanup_dongles(sim);
	if (sim->coders)
		cleanup_coders(sim);
	if (sim->log_lock_initialized)
		pthread_mutex_destroy(&sim->log_lock);
	if (sim->stop_lock_initialized)
		pthread_mutex_destroy(&sim->stop_lock);
}
