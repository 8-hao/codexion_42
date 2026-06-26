/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   initializer.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 15:51:11 by username         #+#    #+#              */
/*   Updated: 2026/06/24 16:17:32 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

t_dongle	*dongles_initializer(int num, int cooldown)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(num * sizeof(t_dongle));
	i = 0;
	if (dongles == NULL)
		return (NULL);
	while (i < num)
	{
		pthread_mutex_init(&dongles[i].mutex_v, NULL);
		pthread_cond_init(&dongles[i].cond_v, NULL);
		dongles[i].cooldown = cooldown;
		dongles[i].is_available = 1;
		dongles[i].headq = NULL;
		i++;
	}
	return (dongles);
}

t_coder	*coders_initializer(int *data, t_dongle *dongles)
{
	int				i;
	t_coder			*coders;
	pthread_mutex_t	*check_time;

	coders = malloc(sizeof(t_coder) * data[0]);
	check_time = malloc(sizeof(pthread_mutex_t) * data[0]);
	i = 0;
	if (coders == NULL)
		return (free(dongles), NULL);
	if (check_time == NULL)
		return (free(dongles), free(coders),NULL);
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

void	m_init(t_monitor *monitor, int *data, t_dongle *dongles, t_coder *c)
{
	(*monitor).coders = c;
	(*monitor).dongles = dongles;
	(*monitor).num_of_coders = data[0];
}

int	threads(pthread_t *ids, int *data, t_coder *coders, void * (*f)(void *))
{
	t_dongle	*dongles;
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
		if(pthread_create(&ids[i], NULL, f, &coders[i]) != 0)
			return 0;
		i++;
	}
	return 1;
}

void	threads_join(pthread_t *id_threads, pthread_t id_monitor, int *data)
{
	int	i;

	i = 0;
	while (i < data[0])
		pthread_join(id_threads[i++], NULL);
	pthread_join(id_monitor, NULL);
	free(id_threads);
}
