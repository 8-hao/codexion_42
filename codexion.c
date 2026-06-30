/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 00:22:04 by obakri            #+#    #+#             */
/*   Updated: 2026/06/28 00:22:09 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_monitor *m, int i)
{
	long long	current_time;
	long long	last_compile_start;
	long long	time_to_burnout;
	int			n;
	long long	r;

	pthread_mutex_lock(m->coders[i].check_time);
	current_time = ft_time();
	last_compile_start = m->coders[i].last_compile_start;
	time_to_burnout = m->coders[i].shared->t_burnout;
	n = m->coders[i].finish;
	pthread_mutex_unlock(m->coders[i].check_time);
	if (current_time - last_compile_start >= time_to_burnout && n == 0)
	{
		r = current_time - m->coders[i].init_time;
		pthread_mutex_lock(&m->coders[i].shared->p_safe);
		printf("%lld %d is burned out\n", r, m->coders[i].id);
		pthread_mutex_unlock(&m->coders[i].shared->p_safe);
		return (1);
	}
	return (0);
}

void	wake_all_dongles(t_monitor *m)
{
	int	i;

	i = 0;
	while (i < m->num_of_coders)
	{
		pthread_mutex_lock(&m->dongles[i].mutex_v);
		pthread_cond_broadcast(&m->dongles[i].cond_v);
		pthread_mutex_unlock(&m->dongles[i].mutex_v);
		i++;
	}
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
				return (stop_all(m), wake_all_dongles(m), NULL);
			check_finished(m, i, &finished, &counter);
			i++;
		}
		if (finished == m->num_of_coders && finished == counter)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

static void	*coder_func(void *coders)
{
	t_coder	*c;
	int		i;

	c = (t_coder *) coders;
	i = 0;
	if (c->id %2 != 0)
		usleep(500);
	while (i++ < c->shared->n_compiles)
	{
		if (is_stopped(c))
			return (NULL);
		if (c->id %2 != 0){
			if (acquire_dongle(c, c->left_d, 'l') == 0)
				return (NULL);
			if (acquire_dongle(c, c->right_d, 'r') == 0)
				return (NULL);
		}
		else{
			if (acquire_dongle(c, c->right_d, 'r') == 0)
				return (NULL);
			if (acquire_dongle(c, c->left_d, 'l') == 0)
				return (NULL);
		}
		if (compiling(c) == 0)
			return (NULL);
		release_dongle(c, c->left_d);
		release_dongle(c, c->right_d);
		if (is_stopped(c))
			return (NULL);
		if (debug_and_refactor(c) == 0)
			return (NULL);
		set_compile_count(c, i);
	}
	set_finish(c);
	return (NULL);
}

void	ft_codexion(t_shared *data)
{
	t_coder		*coders;
	t_dongle	*dongles;
	t_monitor	monitor;
	pthread_t	*threads;
	pthread_t	id_monitor;

	dongles = dongles_initializer(data);
	if (dongles == NULL)
		return ;
	coders = coders_init(data, dongles);
	if (coders == NULL)
		return ((void)free_all(dongles, coders, NULL));
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	if (threads == NULL)
		return ((void)free_all(dongles, coders, NULL));
	if (threads_init(threads, coders, coder_func) == 0)
		return ((void)free_all(dongles, coders, NULL));
	monitor_init(&monitor, dongles, coders);
	pthread_create(&id_monitor, NULL, monitor_func, &monitor);
	threads_join(threads, data->n_coders, id_monitor);
	free_all(dongles, coders, threads);
}
