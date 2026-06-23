/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   pqueue.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:06:18 by username         #+#    #+#              */
/*   Updated: 2026/06/22 13:24:26 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

Queue	*newnode(Coder *c, int index)
{
	Queue	*node;

	node = malloc(sizeof(Queue));
	if (node == NULL)
		return (NULL);
	node->c = c;
	node->index = index;
	node->next = NULL;
	return (node);
}

void	add_back(Queue **head, Queue *node)
{
	Queue	*second;

	second = *head;
	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	while (second->next)
	{
		second = second->next;
	}
	second->next = node;
}

int	queuelen(Queue *head)
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

void	sort_min(Queue **head)
{
	int		i;
	int		ql;
	Queue	*helper;
	Coder	*c;
	int		tin;

	i = 0;
	ql = queuelen(*head);
	while (i < ql)
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
		i++;
	}
}

int	is_inqueue(Queue *head, Coder *c)
{
	while (head)
	{
		if (c->id == head->c->id)
			return (0);
		head = head->next;
	}
	return (1);
}

Queue	*deletefirst(Queue **head)
{
	Queue	*first_node;

	if (*head == NULL)
		return (NULL);
	first_node = *head;
	*head = (*head)->next;
	first_node->next = NULL;
	return (first_node);
}
