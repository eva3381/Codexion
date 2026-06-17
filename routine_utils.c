/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* routine_utils.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: evafletefolgueira <marvin@42.fr>           +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/06/17 13:00:00 by evafletefol       #+#    #+#             */
/* Updated: 2026/06/17 13:05:00 by evafletefol      ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "codexion.h"

static void	lock_dongle_by_sched(t_coder *coder, t_dongle *dongle)
{
	long	deadline;

	pthread_mutex_lock(&dongle->mutex);
	if (coder->hub->is_edf)
	{
		pthread_mutex_lock(&coder->hub->log_mutex);
		deadline = coder->last_meal + coder->hub->time_burnout;
		pthread_mutex_unlock(&coder->hub->log_mutex);
		push_queue(&dongle->queue, coder->id, deadline);
		while (dongle->queue.size > 0
			&& dongle->queue.nodes[0].coder_id != coder->id)
		{
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		}
	}
	print_status(coder, "has taken a dongle");
}

void	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		lock_dongle_by_sched(coder, coder->right_dongle);
		lock_dongle_by_sched(coder, coder->left_dongle);
	}
	else
	{
		lock_dongle_by_sched(coder, coder->left_dongle);
		lock_dongle_by_sched(coder, coder->right_dongle);
	}
}

void	drop_dongles(t_coder *coder)
{
	if (coder->hub->is_edf)
	{
		pop_queue(&coder->left_dongle->queue);
		pthread_cond_broadcast(&coder->left_dongle->cond);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		pop_queue(&coder->right_dongle->queue);
		pthread_cond_broadcast(&coder->right_dongle->cond);
		pthread_mutex_unlock(&coder->right_dongle->mutex);
	}
	else
	{
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		pthread_mutex_unlock(&coder->right_dongle->mutex);
	}
}
