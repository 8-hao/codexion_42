/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:06:00 by username         #+#    #+#              */
/*   Updated: 2026/06/22 17:02:50 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"


void	*monitor_func(void *monitor)
{
	Monitor		*m;
	long long	last_compile_start;
	long long	time_to_burnout;
	long long	current_time;
	int			i;
	int			counter;
	int			finished;

	m = (Monitor *) monitor;
	i = 0;
	counter = 0;
	finished = 0;
	while (1)
	{
		while (i < m->num_of_coders)
		{
			pthread_mutex_lock(m->coders[i].check_time);
			current_time = ft_time();
			last_compile_start = m->coders[i].last_compile_start;
			time_to_burnout = m->coders[i].time_to_burnout;
			pthread_mutex_unlock(m->coders[i].check_time);
			if (current_time - last_compile_start >= time_to_burnout && m->coders[i].finish == 0)
			{
				printf("%lld %d is burned out\n", (current_time - m->coders[i].init_time), m->coders[i].id);
				i = 0;
				while (i < m->num_of_coders)
				{
					m->coders[i].stop = 1;
					i++;
				}
				return (NULL);
			}
			if (m->coders[i].finish == 1)
			{
				finished++;
				if (m->coders[i].compile_count == m->coders[i].num_of_compiles_required)
					counter++;
			}
			i++;
		}
		if (finished == m->num_of_coders)
			if (finished == counter)
			return (NULL);
		i = 0;
		finished = 0;
		counter = 0;
		usleep(1000);
	}
	return (NULL);
}


void	*func(void *coder)
{
	Coder			*c;
	int				i;

	c = (Coder *) coder;
	i = 0;
	while (i++ < c->num_of_compiles_required)
	{
		if (c->stop)
			return (NULL);
		
		if (acquire_dongle(c, c->left_d, 'l')==0)
			return (NULL);

		if (acquire_dongle(c, c->right_d, 'r')==0)
			return (NULL);

		if (compiling(c) == 0)
			return (NULL);

		release_dongle(c->left_d);
		release_dongle(c->right_d);

		if (debug_and_refactor(c) == 0)
			return NULL;

		c->compile_count = i;
	}
	return (c->finish = 1,NULL);
}

void	ft_codexion(int *data)
{
	Dongle		*dongles;
	Coder		*coders;
	Monitor		monitor;
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
	monitor_initializer(&monitor, data, dongles, coders);
	threads_creation(id_threads, data, coders, func);
	pthread_create(&id_monitor, NULL, monitor_func, &monitor);
	threads_join(id_threads,id_monitor, data);
}
