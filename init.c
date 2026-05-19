/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:53:36 by marvin            #+#    #+#             */
/*   Updated: 2026/05/19 12:53:36 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "codexion.h"


static void	init_coder_values(t_hub *hub, int i)
{
	hub->coders[i].id = i + 1;
	hub->coders[i].compiles_done = 0;
	hub->coders[i].last_meal = get_time_ms();
	hub->coders[i].hub = hub;
	hub->coders[i].left_dongle = &hub->dongles[i];
	hub->coders[i].right_dongle = &hub->dongles[(i + 1) % hub->num_coders];
}

void	cleanup_all(t_hub *hub)
{
	int	i;

	i = -1;
	if (hub->dongles)
	{
		while (++i < hub->num_coders)
		{
			pthread_mutex_destroy(&hub->dongles[i].mutex);
			pthread_cond_destroy(&hub->dongles[i].cond);
			if (hub->dongles[i].queue.nodes)
				free(hub->dongles[i].queue.nodes);
		}
		free(hub->dongles);
		hub->dongles = NULL;
	}
	if (hub->coders)
	{
		free(hub->coders);
		hub->coders = NULL;
	}
	pthread_mutex_destroy(&hub->log_mutex);
	pthread_mutex_destroy(&hub->stop_mutex);
}

int	init_simulation(t_hub *hub)
{
	int	i;

	hub->coders = malloc(sizeof(t_coder) * hub->num_coders);
	hub->dongles = malloc(sizeof(t_dongle) * hub->num_coders);
	if (!hub->coders || !hub->dongles)
	{
		cleanup_all(hub);
		return (0);
	}
	pthread_mutex_init(&hub->log_mutex, NULL);
	pthread_mutex_init(&hub->stop_mutex, NULL);
	i = -1;
	while (++i < hub->num_coders)
	{
		pthread_mutex_init(&hub->dongles[i].mutex, NULL);
		pthread_cond_init(&hub->dongles[i].cond, NULL);
		if (!init_queue(&hub->dongles[i].queue, hub->num_coders))
		{
			cleanup_all(hub);
			return (0);
		}
		init_coder_values(hub, i);
	}
	return (1);
}
