/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 22:21:24 by obakri            #+#    #+#             */
/*   Updated: 2026/06/27 22:21:31 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_dongle *dongles, t_coder *coders, pthread_t *threads)
{
	if (coders != NULL)
	{
		free(coders[0].check_time);
		free(coders);
	}
	if (dongles != NULL)
		free(dongles);
	if (threads != NULL)
		free(threads);
}

void	stop_all(t_monitor *m)
{
	int	i;

	i = 0;
	while (i < m->num_of_coders)
	{
		pthread_mutex_lock(m->coders[i].check_time);
		m->coders[i].stop = 1;
		pthread_mutex_unlock(m->coders[i].check_time);
		i++;
	}
}

void	check_finished(t_monitor *m, int i, int *finished, int *counter)
{
	int	n;
	int	compile_count;

	pthread_mutex_lock(m->coders[i].check_time);
	n = m->coders[i].finish;
	compile_count = m->coders[i].compile_count;
	pthread_mutex_unlock(m->coders[i].check_time);
	if (n == 1)
	{
		(*finished)++;
		if (compile_count == m->coders[i].shared->n_compiles)
			(*counter)++;
	}
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

	pthread_mutex_lock(c->check_time);
	s = c->stop;
	pthread_mutex_unlock(c->check_time);
	return (s);
}
