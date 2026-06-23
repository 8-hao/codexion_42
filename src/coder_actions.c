/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_actions.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/23 21:33:23 by username         #+#    #+#              */
/*   Updated: 2026/06/23 21:33:34 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_actions.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/23 21:29:30 by username         #+#    #+#              */
/*   Updated: 2026/06/23 21:33:12 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

int	debug_and_refactor(Coder *c)
{
	printf("%lld %d is debugging\n", ft_time() - c->init_time, c->id);
	if (ft_smartsleep(c->time_to_debug, c) == 0)
		return (0);
	printf("%lld %d is refactoring\n", ft_time() - c->init_time, c->id);
	if (ft_smartsleep(c->time_to_refactor, c) == 0)
		return (0);
	return (1);
}

int	compiling(Coder *c)
{
	int	r;

	printf("%lld %d is compiling\n", ft_time() - c->init_time, c->id);
	r = ft_smartsleep(c->time_to_compile, c);
	if (r == 0)
	{
		pthread_mutex_unlock(&c->left_d->mutex_v);
		pthread_mutex_unlock(&c->right_d->mutex_v);
		return (r);
	}
	pthread_mutex_lock(c->check_time);
	c->last_compile_start = ft_time();
	pthread_mutex_unlock(c->check_time);
	return (r);
}

void	release_dongle(Dongle *d)
{
	d->is_available = 1;
	d->release_time = ft_time();
	pthread_mutex_unlock(&d->mutex_v);
}

int	acquire_dongle(Coder *c, Dongle *d, char ch)
{
	struct timespec	tmp_dongle;

	pthread_mutex_lock(&d->mutex_v);
	if (c->stop)
	{
		pthread_mutex_unlock(&d->mutex_v);
		if (ch == 'r')
			pthread_mutex_unlock(&c->left_d->mutex_v);
		return (0);
	}
	while (ft_time() - d->release_time < d->cooldown || (queuelen(d->headq) != 0 && c->id != d->headq->c->id))
	{
		if (is_inqueue(d->headq, c))
		{
			add_back(&d->headq, newnode(c, ft_time()));
			if (d->arb == 2)
				sort_min(&d->headq);
		}
		ft_time_to_sleep(&tmp_dongle, d->cooldown - (ft_time() - d->release_time));
		pthread_cond_timedwait(&d->cond_v, &d->mutex_v, &tmp_dongle);
	}
	if (queuelen(d->headq) != 0 && c->id == d->headq->c->id)
		deletefirst(&d->headq);
	printf("%lld %d has taken a dongle\n", ft_time() - c->init_time, c->id);
	return (1);
}

void	check_finished(Monitor *m, int i, int *finished, int *counter)
{
	if (m->coders[i].finish == 1)
	{
		(*finished)++;
		if (m->coders[i].compile_count == m->coders[i].num_of_compiles_required)
			(*counter)++;
	}
}
