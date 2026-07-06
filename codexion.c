/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/05 21:37:21 by username         #+#    #+#              */
/*   Updated: 2026/07/05 21:37:21 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	debug_and_refactor(t_coder *c)
{
	if (is_stopped(c))
		return (0);
	safe_print("is debugging", c);
	if (ft_smartsleep(c->shared->t_debug, c) == 0)
		return (0);
	if (is_stopped(c))
		return (0);
	safe_print("is refactoring", c);
	if (ft_smartsleep(c->shared->t_refactor, c) == 0)
		return (0);
	return (1);
}

static void	*coder_func(void *args)
{
	t_coder	*c;
	int		i;

	c = (t_coder *) args;
	i = 0;
	if (c->id % 2 == 0)
		usleep(1000);
	while (i < c->shared->n_compiles)
	{
		if (acquire_dongles(c) == 0)
			return (NULL);
		if (compiling(c) == 0)
			return (NULL);
		if (release_dongles(c) == 0)
			return (NULL);
		if (debug_and_refactor(c) == 0)
			return (NULL);
		i++;
		if (set_compile_count(c, i) == 0)
			return (NULL);
	}
	pthread_mutex_lock(&c->safe_check);
	c->finish = 1;
	pthread_mutex_unlock(&c->safe_check);
	return (NULL);
}

static void	*monitor_func(void *monitor)
{
	t_monitor	*m;
	int			i;
	int			counter;
	int			finished;

	m = (t_monitor *) monitor;
	while (1)
	{
		i = 0;
		counter = 0;
		finished = 0;
		while (i < m->num_of_coders)
		{
			if (check_burnout(m, i))
				return (stop_wake_all(m),
					safe_print("is burned out", &m->coders[i]), NULL);
			check_finished(m, i, &finished, &counter);
			i++;
		}
		if (finished == m->num_of_coders && finished == counter)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

static void	ft_codexion(t_shared *data)
{
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	*threads;
	t_monitor	monitor;

	dongles = dongles_initializer(data);
	if (dongles == NULL)
		return ;
	coders = coders_init(data, dongles);
	if (coders == NULL)
		return ((void) free_all(dongles, coders, NULL));
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	if (threads == NULL)
		return ((void) free_all(dongles, coders, NULL));
	if (threads_init(threads, coders, coder_func) == 0)
		return ((void) free_all(dongles, coders, NULL));
	monitor_init(&monitor, dongles, coders);
	pthread_create(&monitor.id_monitor, NULL, monitor_func, &monitor);
	threads_join(threads, data->n_coders, monitor.id_monitor);
	free_all(dongles, coders, threads);
}

int	main(int argc, char **argv)
{
	t_shared	*data;

	if (argc != 9)
	{
		printf("Error: Expected 8 arguments, but received %d.\n", argc - 1);
		return (1);
	}
	data = parser(argc - 1, argv);
	if (data == NULL)
		return (1);
	ft_codexion(data);
	free(data);
	return (0);
}
