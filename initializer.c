/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   initializer.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/27 10:24:30 by username         #+#    #+#              */
/*   Updated: 2026/06/27 10:24:30 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*dongles_initializer(t_shared *data)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(data->n_coders * sizeof(t_dongle));
	i = 0;
	if (dongles == NULL)
		return (NULL);
	while (i < data->n_coders)
	{
		pthread_mutex_init(&dongles[i].mutex_v, NULL);
		pthread_cond_init(&dongles[i].cond_v, NULL);
		dongles[i].cooldown = data->d_cooldown;
		dongles[i].is_available = 1;
		dongles[i].headq = NULL;
		i++;
	}
	return (dongles);
}

void	monitor_init(t_monitor *monitor, t_dongle *dongles, t_coder *c)
{
	(*monitor).coders = c;
	(*monitor).dongles = dongles;
	(*monitor).num_of_coders = c[0].shared->n_coders;
}

t_coder	*coders_init(t_shared *data, t_dongle *dongles)
{
	int				i;
	t_coder			*coders;
	pthread_mutex_t	*check_time;

	coders = malloc(sizeof(t_coder) * data->n_coders);
	if (coders == NULL)
		return (NULL);
	check_time = malloc(sizeof(pthread_mutex_t) * data->n_coders);
	if (check_time == NULL)
		return (free(dongles), free(coders), NULL);
	i = 0;
	while (i < data->n_coders)
	{
		coders[i].id = i + 1;
		if (((i + 1) % data->n_coders) < i)
		{
			coders[i].left_d = &dongles[(i + 1) % data->n_coders];
			coders[i].right_d = &dongles[i];
		}
		else
		{
			coders[i].left_d = &dongles[i];
			coders[i].right_d = &dongles[(i + 1) % data->n_coders];
		}
		coders[i].shared = data;
		coders[i].compile_count = 0;
		coders[i].check_time = &check_time[i];
		pthread_mutex_init(&check_time[i], NULL);
		coders[i].finish = 0;
		coders[i].stop = 0;
		i++;
	}
	return (coders);
}

int	threads_init(pthread_t *t, t_coder *coders, void * (*f)(void *))
{
	t_dongle	*dongles;
	int			i;
	long long	init_time;

	dongles = coders[0].left_d;
	init_time = ft_time();
	i = 0;
	while (i < coders[0].shared->n_coders)
	{
		coders[i].init_time = init_time;
		coders[i].last_compile_start = ft_time();
		dongles[i].release_time = ft_time() - dongles[i].cooldown;
		if (pthread_create(&t[i], NULL, f, &coders[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

void	threads_join(pthread_t *threads, int n, pthread_t id_monitor)
{
	int	i;

	i = 0;
	while (i < n)
		pthread_join(threads[i++], NULL);
	pthread_join(id_monitor, NULL);
}
