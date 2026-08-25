/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:46 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/25 15:24:28 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int	init_one_dongle(t_sim *sim, int index)
{
	t_dongle	*dongle;

	dongle = &sim->dongles[index];
	dongle->id = index;
	dongle->is_taken = 0;
	dongle->free_since = 0;
	dongle->next_order = -1;
	if (pthread_mutex_init(&dongle->lock, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
		return (pthread_mutex_destroy(&dongle->lock), 0);
	if (!heap_init(&dongle->wait_queue, sim->args.number_of_coders))
	{
		pthread_cond_destroy(&dongle->cond);
		return (pthread_mutex_destroy(&dongle->lock), 0);
	}
	sim->dongles_initialized++;
	return (1);
}

static int	init_dongles(t_sim *sim)
{
	int	index;

	sim->dongles = malloc(sizeof(t_dongle) * sim->args.number_of_coders);
	if (!sim->dongles)
		return (0);
	index = 0;
	while (index < sim->args.number_of_coders)
	{
		if (!init_one_dongle(sim, index))
			return (0);
		index++;
	}
	return (1);
}

static void	set_coder_dongles(t_sim *sim, int index)
{
	t_coder	*coder;

	coder = &sim->coders[index];
	coder->left = &sim->dongles[index];
	if (sim->args.number_of_coders == 1)
		coder->right = coder->left;
	else
		coder->right = &sim->dongles[(index + 1) % sim->args.number_of_coders];
}

static int	init_coders(t_sim *sim)
{
	int	index;

	sim->coders = malloc(sizeof(t_coder) * sim->args.number_of_coders);
	if (!sim->coders)
		return (0);
	index = 0;
	while (index < sim->args.number_of_coders)
	{
		sim->coders[index].id = index + 1;
		sim->coders[index].compiles_done = 0;
		sim->coders[index].last_compile_start = 0;
		sim->coders[index].sim = sim;
		if (pthread_mutex_init(&sim->coders[index].data_lock, NULL) != 0)
			return (0);
		sim->coders_initialized++;
		set_coder_dongles(sim, index++);
	}
	return (1);
}

int	init_sim(t_sim *sim, t_args *args)
{
	memset(sim, 0, sizeof(t_sim));
	sim->args = *args;
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (0);
	sim->stop_lock_initialized = 1;
	if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
		return (0);
	sim->log_lock_initialized = 1;
	if (!init_dongles(sim) || !init_coders(sim))
		return (0);
	return (1);
}
