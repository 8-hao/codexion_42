/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   pqueue.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/05 21:37:49 by username         #+#    #+#              */
/*   Updated: 2026/07/05 21:37:49 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_queue	*newnode(t_coder *c, long long index)
{
	t_queue	*node;

	node = malloc(sizeof(t_queue));
	if (node == NULL)
		return (NULL);
	node->c = c;
	node->index = index;
	node->next = NULL;
	return (node);
}

void	add_back(t_queue **head, t_queue *node)
{
	t_queue	*second;

	second = *head;
	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	while (second->next)
		second = second->next;
	second->next = node;
}

int	queuelen(t_queue *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

void	sort_min(t_queue **head)
{
	int		i;
	int		ql;
	t_queue	*helper;
	t_coder	*c;
	int		tin;

	i = 0;
	ql = queuelen(*head);
	while (i++ < ql)
	{
		helper = *head;
		while (helper && helper->next)
		{
			if (helper->index > (helper->next)->index)
			{
				c = helper->c;
				tin = helper->index;
				helper->index = (helper->next)->index;
				(helper->next)->index = tin;
				helper->c = (helper->next)->c;
				(helper->next)->c = c;
			}
			helper = helper->next;
		}
	}
}

int	is_inqueue(t_queue *head, t_coder *c)
{
	while (head)
	{
		if (c->id == head->c->id)
			return (0);
		head = head->next;
	}
	return (1);
}
