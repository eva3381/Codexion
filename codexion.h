/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:53:31 by marvin            #+#    #+#             */
/*   Updated: 2026/05/19 12:53:31 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_node
{
	int		coder_id;
	long	priority_value;
}	t_node;
typedef struct s_queue
{
	t_node	*nodes;
	int		size;
	int		capacity;
}	t_queue;
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_queue			queue;
	long			last_released;
}	t_dongle;
typedef struct s_hub
{
	int				num_coders;
	int				time_burnout;
	int				time_compile;
	int				time_debug;
	int				time_refactor;
	int				must_compile;
	int				cooldown;
	int				is_edf;
	int				stop_sim;
	long			start_time;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	t_dongle		*dongles;
	struct s_coder	*coders;
}	t_hub;
typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compiles_done;
	long			last_meal;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_hub			*hub;
}	t_coder;
int		parse_args(t_hub *hub, int argc, char **argv);
int		init_simulation(t_hub *hub);
void	cleanup_all(t_hub *hub);
int		init_queue(t_queue *q, int capacity);
void	start_threads(t_hub *hub);
void	*coder_routine(void *arg);
long	get_time_ms(void);
void	smart_sleep(long time, t_hub *hub);
void	print_status(t_coder *coder, char *status);
void	push_queue(t_queue *q, int id, long priority);
int		pop_queue(t_queue *q);
void	bubble_up(t_queue *q, int index);
void	bubble_down(t_queue *q, int index);
void	swap_nodes(t_node *a, t_node *b);
void	take_dongles(t_coder *coder);
void	drop_dongles(t_coder *coder);
#endif