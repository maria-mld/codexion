/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_refactor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:41 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:09:42 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	do_refactor(t_coder *coder)
{
	log_action(coder->sim, coder->id, "is refactoring");
	return (sleep_interruptible(coder->sim, coder->sim->args.time_to_refactor));
}
