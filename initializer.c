/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:44:46 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:44:50 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*dongles_initializer(t_shared *data)
{
	int			i;
	t_dongle	*dongles;

	i = 0;
	dongles = malloc(data->n_coders * sizeof(t_dongle));
	if (dongles == NULL)
		return (NULL);
	while (i < data->n_coders)
	{
		if (pthread_mutex_init(&dongles[i].mutex_v, NULL) != 0)
			return (free(dongles), NULL);
		pthread_cond_init(&dongles[i].cond_v, NULL);
		dongles[i].cooldown = data->d_cooldown;
		dongles[i].is_available = 1;
		dongles[i].headq = NULL;
		dongles[i].arb = data->c;
		i++;
	}
	return (dongles);
}

t_coder	*coders_init(t_shared *data, t_dongle *dongles)
{
	int		i;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * data->n_coders);
	if (coders == NULL)
		return (NULL);
	i = 0;
	while (i < data->n_coders)
	{
		coders[i].id = i + 1;
		set_dongle_pair(coders, dongles, i, data->n_coders);
		coders[i].shared = data;
		coders[i].compile_count = 0;
		if (pthread_mutex_init(&coders[i].safe_check, NULL) != 0)
			return (free(coders), NULL);
		coders[i].finish = 0;
		coders[i].stop = 0;
		i++;
	}
	return (coders);
}

int	threads_init(pthread_t *t, t_coder *coders, void *(*f)(void *))
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
		coders[i].last_compile_start = init_time;
		dongles[i].release_time = init_time;
		dongles[i].cooldown_time = set_time(0);
		i++;
	}
	i = 0;
	while (i < coders[0].shared->n_coders)
	{
		if (pthread_create(&t[i], NULL, f, &coders[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

void	monitor_init(t_monitor *monitor, t_dongle *dongles, t_coder *c)
{
	(*monitor).coders = c;
	(*monitor).dongles = dongles;
	(*monitor).num_of_coders = c[0].shared->n_coders;
}

void	threads_join(pthread_t *threads, int n, pthread_t id_monitor)
{
	int	i;

	i = 0;
	while (i < n)
		pthread_join(threads[i++], NULL);
	pthread_join(id_monitor, NULL);
}
