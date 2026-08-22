/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_compile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:33 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:09:34 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	do_compile(t_coder *coder)
{
	if (!take_dongles(coder))
		return (0);
	pthread_mutex_lock(&coder->data_lock);
	coder->last_compile_start = get_timestamp_ms();
	pthread_mutex_unlock(&coder->data_lock);
	log_action(coder->sim, coder->id, "is compiling");
	if (!sleep_interruptible(coder->sim, coder->sim->args.time_to_compile))
	{
		release_dongles(coder);
		return (0);
	}
	release_dongles(coder);
	pthread_mutex_lock(&coder->data_lock);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->data_lock);
	return (1);
}
