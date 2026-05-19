/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:54:13 by marvin            #+#    #+#             */
/*   Updated: 2026/05/19 12:54:16 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->right_dongle->mutex);
		print_status(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->left_dongle->mutex);
		print_status(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->left_dongle->mutex);
		print_status(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->right_dongle->mutex);
		print_status(coder, "has taken a dongle");
	}
}

static void	drop_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

static void	handle_single_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	print_status(coder, "has taken a dongle");
	smart_sleep(coder->hub->time_burnout, coder->hub);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
}

static int	check_stop(t_hub *hub)
{
	pthread_mutex_lock(&hub->stop_mutex);
	if (hub->stop_sim)
	{
		pthread_mutex_unlock(&hub->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&hub->stop_mutex);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_hub	*hub;

	coder = (t_coder *)arg;
	hub = coder->hub;
	if (coder->id % 2 == 0)
		usleep(1000);
	if (hub->num_coders == 1)
		return (handle_single_coder(coder), NULL);
	while (!check_stop(hub))
	{
		take_dongles(coder);
		pthread_mutex_lock(&hub->log_mutex);
		coder->last_meal = get_time_ms();
		pthread_mutex_unlock(&hub->log_mutex);
		print_status(coder, "is compiling");
		smart_sleep(hub->time_compile, hub);
		drop_dongles(coder);
		pthread_mutex_lock(&hub->log_mutex);
		coder->compiles_done++;
		pthread_mutex_unlock(&hub->log_mutex);
		if (hub->must_compile != -1 && coder->compiles_done >= hub->must_compile)
			break ;
		print_status(coder, "is debugging");
		smart_sleep(hub->time_debug, hub);
	}
	return (NULL);
}
