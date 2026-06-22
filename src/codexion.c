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
	int				r;
	struct timespec	l_dongle;
	struct timespec	r_dongle;

	c = (Coder *) coder;
	i = 0;
	while (i++ < c->num_of_compiles_required)
	{
		if (c->stop)
			return (NULL);
		pthread_mutex_lock(&c->left_d->mutex_v);
		if (c->stop)
		{
			pthread_mutex_unlock(c->left_d);
			return (NULL);
		}
		while (ft_time() - c->left_d->release_time < c->left_d->cooldown || (queuelen(c->left_d->headq) != 0 && c->id != c->left_d->headq->c->id))
		{
			if (is_inqueue(c->left_d->headq, c))
			{
				add_back(&c->left_d->headq, newnode(c, ft_time()));
				if (c->left_d->arb == 2)
					sort_min(&c->left_d->headq);
			}
			ft_time_to_sleep(&l_dongle, c->left_d->cooldown - (ft_time() - c->left_d->release_time));
			pthread_cond_timedwait(&c->left_d->cond_v, &c->left_d->mutex_v, &l_dongle);
		}
		if (queuelen(c->left_d->headq) != 0 && c->id == c->left_d->headq->c->id)
			deleteFirst(&c->left_d->headq);
		printf("%lld %d has taken a dongle\n", ft_time() - c->init_time, c->id);
		pthread_mutex_lock(&c->right_d->mutex_v);
		if (c->stop)
		{
			pthread_mutex_unlock(c->left_d);
			pthread_mutex_unlock(c->right_d);
			return (NULL);
		}
		while (ft_time() - c->right_d->release_time < c->right_d->cooldown || (queuelen(c->right_d->headq) != 0 && c->id != c->right_d->headq->c->id))
		{
			if (is_inqueue(c->right_d->headq, c))
			{
				add_back(&c->right_d->headq, newnode(c, ft_time()));
				if (c->right_d->arb == 2)
					sort_min(&c->right_d->headq);
			}
			ft_time_to_sleep(&r_dongle, c->right_d->cooldown - (ft_time() - c->right_d->release_time));
			pthread_cond_timedwait(&c->right_d->cond_v, &c->right_d->mutex_v, &r_dongle);
		}
		if (queuelen(c->right_d->headq) != 0 && c->id == c->right_d->headq->c->id)
			deleteFirst(&c->right_d->headq);
		printf("%lld %d has taken a dongle\n", ft_time() - c->init_time, c->id);
		printf("%lld %d is compiling\n", ft_time() - c->init_time, c->id);
		r = ft_smartsleep(c->time_to_compile, c);
		if (r == 0)
		{
			pthread_mutex_unlock(c->left_d);
			pthread_mutex_unlock(c->right_d);
			return (NULL);
		}
		pthread_mutex_lock(c->check_time);
		c->last_compile_start = ft_time();
		pthread_mutex_unlock(c->check_time);
		c->left_d->is_available = 1;
		c->left_d->release_time = ft_time();
		pthread_mutex_unlock(&c->left_d->mutex_v);
		c->right_d->is_available = 1;
		c->right_d->release_time = ft_time();
		pthread_mutex_unlock(&c->right_d->mutex_v);
		printf("%lld %d is debugging\n", ft_time() - c->init_time, c->id);
		r = ft_smartsleep(c->time_to_debug, c);
		if (r == 0)
			return (NULL);
		printf("%lld %d is refactoring\n", ft_time() - c->init_time, c->id);
		r = ft_smartsleep(c->time_to_refactor, c);
		if (r == 0)
			return (NULL);
		c->compile_count = i;
	}
	c->finish = 1;
	return (NULL);
}

void	ft_codexion(int *data)
{
	Dongle		*dongles;
	Coder		*coders;
	Monitor		monitor;
	pthread_t	*id_threads;
	pthread_t	id_monitor;
	int			i;

	i = 0;
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
