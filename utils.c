/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:13:15 by marvin            #+#    #+#             */
/*   Updated: 2026/05/12 20:05:20 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long time, t_hub *hub)
{
	long    	start;
	int			should_stop;

	start = get_time_ms();
	while (1)
	{
		// Protegemos la lectura de la variable compartida
		pthread_mutex_lock(&hub->stop_mutex);
		should_stop = hub->stop_sim;
		pthread_mutex_unlock(&hub->stop_mutex);

		// Si alguien ha muerto o el tiempo ha pasado, salimos
		if (should_stop || (get_time_ms() - start) >= time)
			break ;
		
		// Dormimos un intervalo pequeño (500us es un buen equilibrio)
		usleep(500);
	}
}


void	print_status(t_coder *coder, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&coder->hub->log_mutex);
	pthread_mutex_lock(&coder->hub->stop_mutex);
	if (!coder->hub->stop_sim)
	{
		timestamp = get_time_ms() - coder->hub->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->hub->stop_mutex);
	pthread_mutex_unlock(&coder->hub->log_mutex);
}
