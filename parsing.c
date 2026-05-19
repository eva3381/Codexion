/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:53:20 by marvin            #+#    #+#             */
/*   Updated: 2026/05/19 12:54:02 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_values(t_hub *hub, char *sched)
{
	if (strcmp(sched, "fifo") == 0)
		hub->is_edf = 0;
	else if (strcmp(sched, "edf") == 0)
		hub->is_edf = 1;
	else
	{
		printf("Error: Invalid scheduling mode. Use 'fifo' or 'edf'.\n");
		return (0);
	}
	if (hub->num_coders < 1 || hub->time_burnout < 1)
	{
		printf("Error: Invalid numeric values.\n");
		return (0);
	}
	return (1);
}

static int	check_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("Error: Wrong format.\nUsage: ./codexion <n_coders> ");
		printf("<t_burn> <t_comp> <t_debug> <t_refact> <n_comp_req> ");
		printf("<cooldown> <sched>\n");
		return (0);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_numeric(argv[i]))
		{
			printf("Error: Argument '%s' is not a valid number.\n", argv[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parse_args(t_hub *hub, int argc, char **argv)
{
	if (!check_args(argc, argv))
		return (0);
	hub->num_coders = atoi(argv[1]);
	hub->time_burnout = atoi(argv[2]);
	hub->time_compile = atoi(argv[3]);
	hub->time_debug = atoi(argv[4]);
	hub->time_refactor = atoi(argv[5]);
	hub->must_compile = atoi(argv[6]);
	hub->cooldown = atoi(argv[7]);
	return (validate_values(hub, argv[8]));
}
