/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:06:00 by username         #+#    #+#              */
/*   Updated: 2026/06/24 16:17:41 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

int	check_burnout(t_monitor *m, int i)
{
	long long	current_time;
	long long	last_compile_start;
	long long	time_to_burnout;

	pthread_mutex_lock(m->coders[i].check_time);
	current_time = ft_time();
	last_compile_start = m->coders[i].last_compile_start;
	time_to_burnout = m->coders[i].time_to_burnout;
	pthread_mutex_unlock(m->coders[i].check_time);
	if (current_time - last_compile_start >= time_to_burnout && m->coders[i].finish == 0)
	{
		printf("%lld %d is burned out\n", current_time - m->coders[i].init_time, m->coders[i].id);
		return (1);
	}
	return (0);
}

void	stop_all(t_monitor *m)
{
	int	i;

	i = 0;
	while (i < m->num_of_coders)
	{
		m->coders[i].stop = 1;
		i++;
	}
}

void	*monitor_func(void *monitor)
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
				return (stop_all(m), NULL);
			check_finished(m, i, &finished, &counter);
			i++;
		}
		if (finished == m->num_of_coders && finished == counter)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

void	*func(void *coder)
{
	t_coder	*c;
	int		i;

	c = (t_coder *) coder;
	i = 0;
	while (i++ < c->num_of_compiles_required)
	{
		if (c->stop)
			return (NULL);
		if (acquire_dongle(c, c->left_d, 'l') == 0)
			return (NULL);
		if (acquire_dongle(c, c->right_d, 'r') == 0)
			return (NULL);
		if (compiling(c) == 0)
			return (NULL);
		release_dongle(c->left_d);
		release_dongle(c->right_d);
		if (debug_and_refactor(c) == 0)
			return (NULL);
		c->compile_count = i;
	}
	return (c->finish = 1, NULL);
}

void	ft_codexion(int *data)
{
	t_dongle	*dongles;
	t_coder		*coders;
	t_monitor	monitor;
	pthread_t	*id_threads;
	pthread_t	id_monitor;

	dongles = dongles_initializer(data[0], data[6]);
	if (dongles == NULL)
		return ;
	coders = coders_initializer(data, dongles);
	if (coders == NULL)
		return ;
	id_threads = malloc(sizeof(pthread_t) * data[0]);
	if (id_threads == NULL)
		return ;
	m_init(&monitor, data, dongles, coders);
	if (threads(id_threads, data, coders, func) == 0)
	{
		free_all(dongles, coders);
		return;
	}
	if (pthread_create(&id_monitor, NULL, monitor_func, &monitor) != 0 ){
		free_all(dongles, coders);
		return;
	}
	threads_join(id_threads, id_monitor, data);
	free_all(dongles, coders);
}
