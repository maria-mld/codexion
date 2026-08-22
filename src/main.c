/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:52 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:53 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void	set_start_times(t_sim *sim)
{
	int	index;

	sim->start_time = get_timestamp_ms();
	index = 0;
	while (index < sim->args.number_of_coders)
		sim->coders[index++].last_compile_start = sim->start_time;
}

static void	join_coders(t_sim *sim, int count)
{
	int	index;

	index = 0;
	while (index < count)
		pthread_join(sim->coders[index++].thread, NULL);
}

static int	start_coders(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->args.number_of_coders)
	{
		if (pthread_create(&sim->coders[index].thread, NULL,
				coder_routine, &sim->coders[index]) != 0)
		{
			stop_simulation(sim);
			join_coders(sim, index);
			return (0);
		}
		index++;
	}
	return (1);
}

static int	start_simulation(t_sim *sim)
{
	set_start_times(sim);
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (0);
	if (!start_coders(sim))
	{
		pthread_join(sim->monitor, NULL);
		return (0);
	}
	join_coders(sim, sim->args.number_of_coders);
	pthread_join(sim->monitor, NULL);
	return (1);
}

int	main(int argc, char **argv)
{
	t_args	args;
	t_sim	sim;

	if (!parse_args(argc, argv, &args))
		return (1);
	if (!init_sim(&sim, &args))
		return (fprintf(stderr, "Error: initialization failed\n"), 1);
	if (args.number_of_compiles_required != 0 && !start_simulation(&sim))
	{
		cleanup_sim(&sim);
		return (fprintf(stderr, "Error: pthread_create failed\n"), 1);
	}
	cleanup_sim(&sim);
	return (0);
}
