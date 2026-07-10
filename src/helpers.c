/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.1337.ma>              +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:46:24 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:46:28 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_queue(t_queue *queue)
{
	t_queue	*node;

	while (queue)
	{
		node = queue->next;
		queue->next = NULL;
		free(queue);
		queue = node;
	}
}

void	free_all(t_dongle *dongles, t_coder *coders, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < coders[0].shared->n_coders)
	{
		if (queuelen(dongles[i].headq) != 0)
			free_queue(dongles[i].headq);
		i++;
	}
	if (coders != NULL)
		free(coders);
	if (dongles != NULL)
		free(dongles);
	if (threads != NULL)
		free(threads);
}

void	safe_print(char *s, t_coder *c)
{
	long long	t;

	t = ft_time() - c->init_time;
	pthread_mutex_lock(&c->shared->p_safe);
	printf("%lld %d %s\n", t, c->id, s);
	pthread_mutex_unlock(&c->shared->p_safe);
}

t_queue	*deletefirst(t_queue **head)
{
	t_queue	*first_node;

	if (*head == NULL)
		return (NULL);
	first_node = *head;
	*head = (*head)->next;
	first_node->next = NULL;
	return (first_node);
}

int	is_stopped(t_coder *c)
{
	int	s;

	pthread_mutex_lock(&c->safe_check);
	s = c->stop;
	pthread_mutex_unlock(&c->safe_check);
	return (s);
}
