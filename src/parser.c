/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:10:56 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:10:57 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int	parse_number(const char *str, long *out, int allow_zero)
{
	long	value;
	int		digit;

	if (!str || !str[0])
		return (0);
	value = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		digit = *str - '0';
		if (value > (LONG_MAX - digit) / 10)
			return (0);
		value = value * 10 + digit;
		str++;
	}
	if (!allow_zero && value == 0)
		return (0);
	*out = value;
	return (1);
}

static int	parse_scheduler(const char *str, int *out)
{
	if (strcmp(str, "fifo") == 0)
	{
		*out = SCHED_FIFO_MODE;
		return (1);
	}
	if (strcmp(str, "edf") == 0)
	{
		*out = SCHED_EDF_MODE;
		return (1);
	}
	return (0);
}

static int	print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	return (0);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	long		tmp;

	if (argc != 9)
		return (print_error("expected 8 arguments"));
	if (!parse_number(argv[1], &tmp, 0) || tmp > INT_MAX)
		return (print_error("invalid number_of_coders"));
	args->number_of_coders = (int)tmp;
	if (!parse_number(argv[2], &args->time_to_burnout, 1))
		return (print_error("invalid time_to_burnout"));
	if (!parse_number(argv[3], &args->time_to_compile, 1))
		return (print_error("invalid time_to_compile"));
	if (!parse_number(argv[4], &args->time_to_debug, 1))
		return (print_error("invalid time_to_debug"));
	if (!parse_number(argv[5], &args->time_to_refactor, 1))
		return (print_error("invalid time_to_refactor"));
	if (!parse_number(argv[6], &tmp, 1) || tmp > INT_MAX)
		return (print_error("invalid number_of_compiles_required"));
	args->number_of_compiles_required = (int)tmp;
	if (!parse_number(argv[7], &args->dongle_cooldown, 1))
		return (print_error("invalid dongle_cooldown"));
	if (!parse_scheduler(argv[8], &args->scheduler))
		return (print_error("scheduler must be 'fifo' or 'edf'"));
	return (1);
}
