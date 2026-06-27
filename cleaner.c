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

long long	ft_time(void)
{
	struct timeval	d;
	long long		time;

	gettimeofday(&d, NULL);
	time = d.tv_sec * 1000 + d.tv_usec / 1000;
	return (time);
}

void	ft_time_to_sleep(struct timespec *t, int delay_ms)
{
	struct timeval	d;
	long long		total_nsec;

	gettimeofday(&d, NULL);
	total_nsec = (long long) d.tv_usec * 1000 + (long long) delay_ms * 1000000;
	t->tv_sec = d.tv_sec + (total_nsec / 1000000000);
	t->tv_nsec = total_nsec % 1000000000;
}

int	ft_smartsleep(int time_to_sleep, t_coder *c)
{
	long long	current_time;

	current_time = 0;
	while (current_time < time_to_sleep * 1000)
	{
		pthread_mutex_lock(c->check_time);
		if (c->stop)
		{
			pthread_mutex_unlock(c->check_time);
			return (0);
		}
		pthread_mutex_unlock(c->check_time);
		current_time += 500;
		usleep(500);
	}
	return (1);
}

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
