/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:08:51 by marvin            #+#    #+#             */
/*   Updated: 2026/05/12 16:08:55 by evafletefol      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_node *a, t_node *b)
{
	t_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	bubble_up(t_queue *q, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (q->nodes[index].priority_value < q->nodes[parent].priority_value)
		{
			swap_nodes(&q->nodes[index], &q->nodes[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	bubble_down(t_queue *q, int index)
{
	int	smallest;
	int	left;
	int	right;

	while (1)
	{
		smallest = index;
		left = 2 * index + 1;
		right = 2 * index + 2;
		if (left < q->size && q->nodes[left].priority_value \
			< q->nodes[smallest].priority_value)
			smallest = left;
		if (right < q->size && q->nodes[right].priority_value \
			< q->nodes[smallest].priority_value)
			smallest = right;
		if (smallest != index)
		{
			swap_nodes(&q->nodes[index], &q->nodes[smallest]);
			index = smallest;
		}
		else
			break ;
	}
}
