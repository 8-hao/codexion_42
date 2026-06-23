/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   initializer.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 15:51:11 by username         #+#    #+#              */
/*   Updated: 2026/06/23 21:44:03 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

Dongle	*dongles_initializer(int num, int cooldown)
{
	int		i;
	Dongle	*dongles;

	dongles = malloc(num * sizeof(Dongle));
	i = 0;
	if (dongles == NULL)
		return (NULL);
	while (i < num)
	{
		pthread_mutex_init(&dongles[i].mutex_v, NULL);
		pthread_cond_init(&dongles[i].cond_v, NULL);
		dongles[i].cooldown = cooldown;
		dongles[i].is_available = 1;
		i++;
	}
	return (dongles);
}

Coder	*coders_initializer(int *data, Dongle *dongles)
{
	int				i;
	Coder			*coders;
	pthread_mutex_t	*check_time;

	coders = malloc(sizeof(Coder) * data[0]);
	check_time = malloc(sizeof(pthread_mutex_t) * data[0]);
	i = 0;
	if (coders == NULL || check_time == NULL)
		return (NULL);
	while (i < data[0])
	{
		coders[i].id = i + 1;
		if (((i + 1) % data[0]) < i)
		{
			coders[i].left_d = &dongles[(i + 1) % data[0]];
			coders[i].right_d = &dongles[i];
		}
		else
		{
			coders[i].left_d = &dongles[i];
			coders[i].right_d = &dongles[(i + 1) % data[0]];
		}
		coders[i].time_to_burnout = data[1];
		coders[i].time_to_compile = data[2];
		coders[i].time_to_debug = data[3];
		coders[i].time_to_refactor = data[4];
		coders[i].num_of_compiles_required = data[5];
		coders[i].compile_count = 0;
		coders[i].check_time = &check_time[i];
		coders[i].finish = 0;
		coders[i].stop = 0;
		i++;
	}
	return (coders);
}

void	m_init(Monitor *monitor, int *data, Dongle *dongles, Coder *coders)
{
	(*monitor).coders = coders;
	(*monitor).dongles = dongles;
	(*monitor).num_of_coders = data[0];
}

void	threads(pthread_t *ids, int *data, Coder *coders, void * (*f)(void *))
{
	Dongle		*dongles;
	int			i;
	long long	init;

	dongles = coders[0].left_d;
	init = ft_time();
	i = 0;
	while (i < data[0])
	{
		coders[i].init_time = init;
		coders[i].last_compile_start = ft_time();
		dongles[i].release_time = ft_time() - dongles[i].cooldown;
		pthread_create(&id_threads[i], NULL, func, &coders[i]);
		i++;
	}
}

void	threads_join(pthread_t *id_threads, pthread_t id_monitor, int *data)
{
	int	i;

	i = 0;
	while (i < data[0])
		pthread_join(id_threads[i++], NULL);
	pthread_join(id_monitor, NULL);
}
