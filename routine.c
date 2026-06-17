/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* routine.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/05/19 10:54:13 by marvin            #+#    #+#             */
/* Updated: 2026/06/17 12:56:00 by evafletefol      ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "codexion.h"

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

static void	handle_single_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	print_status(coder, "has taken a dongle");
	smart_sleep(coder->hub->time_burnout, coder->hub);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
}

static int	routine_loop(t_coder *coder, t_hub *hub)
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
	if (hub->must_compile != -1
		&& coder->compiles_done >= hub->must_compile)
		return (0);
	print_status(coder, "is debugging");
	smart_sleep(hub->time_debug, hub);
	usleep(500);
	return (1);
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
	{
		handle_single_coder(coder);
		return (NULL);
	}
	while (!check_stop(hub))
	{
		if (!routine_loop(coder, hub))
			break ;
	}
	return (NULL);
}
