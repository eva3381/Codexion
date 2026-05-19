/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:08:09 by marvin            #+#    #+#             */
/*   Updated: 2026/05/12 16:08:17 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_queue(t_queue *q, int capacity)
{
	q->nodes = malloc(sizeof(t_node) * capacity);
	if (!q->nodes)
		return (0);
	q->size = 0;
	q->capacity = capacity;
	return (1);
}

void	push_queue(t_queue *q, int id, long priority)
{
	if (q->size >= q->capacity)
		return ;
	q->nodes[q->size].coder_id = id;
	q->nodes[q->size].priority_value = priority;
	bubble_up(q, q->size);
	q->size++;
}

int	pop_queue(t_queue *q)
{
	int	root_id;

	if (q->size <= 0)
		return (-1);
	root_id = q->nodes[0].coder_id;
	q->size--;
	if (q->size > 0)
	{
		q->nodes[0] = q->nodes[q->size];
		bubble_down(q, 0);
	}
	return (root_id);
}

int	peek_queue(t_queue *q)
{
	if (q->size <= 0)
		return (-1);
	return (q->nodes[0].coder_id);
}
