/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:13 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:09:17 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include "types.h"

int		parse_args(int argc, char **argv, t_args *args);
long	get_timestamp_ms(void);
int		sim_is_stopped(t_sim *sim);
int		sleep_interruptible(t_sim *sim, long duration);
void	stop_simulation(t_sim *sim);
void	announce_burnout(t_sim *sim, int coder_id);
void	log_action(t_sim *sim, int coder_id, const char *action);
int		init_sim(t_sim *sim, t_args *args);
void	cleanup_sim(t_sim *sim);
int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
long	dongle_key(t_coder *coder);
int		wait_for_dongle(t_dongle *dongle, t_coder *coder, long order);
int		do_compile(t_coder *coder);
int		do_debug(t_coder *coder);
int		do_refactor(t_coder *coder);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
int		heap_init(t_heap *heap, int capacity);
void	heap_destroy(t_heap *heap);
int		heap_push(t_heap *heap, int coder_id, long key, long order);
int		heap_pop(t_heap *heap, t_request *out);
int		heap_peek(t_heap *heap, t_request *out);
int		heap_is_empty(t_heap *heap);
void	heap_sift_up(t_heap *heap, int index);
void	heap_sift_down(t_heap *heap, int index);

#endif
