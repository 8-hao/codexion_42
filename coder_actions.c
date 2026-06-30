/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 01:37:23 by obakri            #+#    #+#             */
/*   Updated: 2026/06/28 01:37:46 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(c->check_time);
	d->is_available = 1;
	d->release_time = ft_time();
	pthread_mutex_unlock(c->check_time);
	pthread_mutex_unlock(&d->mutex_v);
}

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

int	compiling(t_coder *c)
{
	if (is_stopped(c))
	{
		pthread_mutex_unlock(&c->left_d->mutex_v);
		pthread_mutex_unlock(&c->right_d->mutex_v);
		return (0);
	}
	safe_print("is compiling", c);
	pthread_mutex_lock(c->check_time);
	c->last_compile_start = ft_time();
	pthread_mutex_unlock(c->check_time);
	if (ft_smartsleep(c->shared->t_compile, c) == 0)
	{
		pthread_mutex_unlock(&c->left_d->mutex_v);
		pthread_mutex_unlock(&c->right_d->mutex_v);
		return (0);
	}
	return (1);
}

int	acquire_dongle(t_coder *c, t_dongle *d, char ch)
{
	struct timespec	tmp;

	pthread_mutex_lock(&d->mutex_v);
	if (is_stopped(c))
		return (abort_acquire(c, d, ch));
	while (!c->stop && ft_time() - d->release_time < d->cooldown)
	{
		if (is_inqueue(d->headq, c))
		{
			add_back(&d->headq, newnode(c, set_queue_val(c, d)));
			if (d->arb == 2)
				sort_min(&d->headq);
		}
		ft_time_to_sleep(&tmp, d->cooldown - (ft_time() - d->release_time));
		pthread_cond_timedwait(&d->cond_v, &d->mutex_v, &tmp);
	}
	if (is_stopped(c))
		return (abort_acquire(c, d, ch));
	if (queuelen(d->headq) != 0 && c->id == d->headq->c->id)
		free(deletefirst(&d->headq));
	safe_print("has taken a dongle", c);
	return (1);
}

