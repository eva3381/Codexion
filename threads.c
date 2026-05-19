/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:06:41 by marvin            #+#    #+#             */
/*   Updated: 2026/05/12 20:06:53 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	start_threads(t_hub *hub)
{
	int	i;

	i = 0;
	hub->start_time = get_time_ms();
	while (i < hub->num_coders)
	{
		// Inicializamos el last_meal justo antes de crear el hilo
		hub->coders[i].last_meal = hub->start_time;
		if (pthread_create(&hub->coders[i].thread, NULL, &coder_routine, &hub->coders[i]))
			return ;
		i++;
	}
}
