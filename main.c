/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:18:05 by marvin            #+#    #+#             */
/*   Updated: 2026/05/13 13:03:36 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_hub *hub)
{
	int		i;
	long	current_time;

	i = 0;
	current_time = get_time_ms();
	while (i < hub->num_coders)
	{
		pthread_mutex_lock(&hub->log_mutex);
		if ((current_time - hub->coders[i].last_meal) >= hub->time_burnout)
		{
			// BLOQUEAMOS LA SIMULACIÓN ANTES DE IMPRIMIR
			pthread_mutex_lock(&hub->stop_mutex);
			hub->stop_sim = 1;
			pthread_mutex_unlock(&hub->stop_mutex);
			
			// Imprimimos la muerte
			printf("%ld %d burned out\n", current_time - hub->start_time, hub->coders[i].id);
			
			pthread_mutex_unlock(&hub->log_mutex);
			return (1);
		}
		pthread_mutex_unlock(&hub->log_mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;
	int		i;

	hub = (t_hub *)arg;
	while (1)
	{
		if (check_burnout(hub))
			break ;
		pthread_mutex_lock(&hub->stop_mutex);
		if (hub->stop_sim)
		{
			pthread_mutex_unlock(&hub->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&hub->stop_mutex);
		usleep(1000);
	}
	i = 0;
	while (i < hub->num_coders)
	{

		pthread_join(hub->coders[i].thread, NULL);
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_hub	hub;


	hub.stop_sim = 0;
	hub.start_time = get_time_ms();
	
	if (argc != 9)
	{
		printf("Error: Wrong number of arguments.\n");
		return (1);
	}

	if (!parse_args(&hub, argc, argv))
		return (1);
	if (!init_simulation(&hub))
		return (1);
	

	start_threads(&hub);
	monitor_routine(&hub);
	
	cleanup_all(&hub);
	return (0);
}
