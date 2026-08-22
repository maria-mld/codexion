/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:26 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:17:00 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <pthread.h>
# define SCHED_FIFO_MODE 0
# define SCHED_EDF_MODE 1

typedef struct s_request
{
	int				coder_id;
	long			key;
	long			order;
}	t_request;
typedef struct s_heap
{
	t_request		*data;
	int				size;
	int				capacity;
}	t_heap;
typedef struct s_dongle
{
	int				id;
	int				is_taken;
	long			free_since;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_heap			wait_queue;
	long			next_order;
}	t_dongle;
typedef struct s_coder
{
	int				id;
	int				compiles_done;
	long			last_compile_start;
	pthread_mutex_t	data_lock;
	t_dongle		*left;
	t_dongle		*right;
	pthread_t		thread;
	struct s_sim	*sim;
}	t_coder;
typedef struct s_args
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	int			scheduler;
}	t_args;
typedef struct s_sim
{
	t_args			args;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	int				stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	log_lock;
	pthread_t		monitor;
	int				dongles_initialized;
	int				coders_initialized;
	int				stop_lock_initialized;
	int				log_lock_initialized;
}	t_sim;
#endif
