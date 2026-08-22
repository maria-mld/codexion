/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_release.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmoldo <marmoldo@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:09:50 by marmoldo          #+#    #+#             */
/*   Updated: 2026/08/16 18:09:51 by marmoldo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static void	release_one(t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->is_taken = 0;
	d->free_since = get_timestamp_ms();
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}

void	release_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
		return ;
	release_one(coder->left);
	release_one(coder->right);
}
