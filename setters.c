/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 22:22:11 by obakri            #+#    #+#             */
/*   Updated: 2026/06/27 22:22:14 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_compile_count(t_coder *c, int i)
{
	pthread_mutex_lock(c->check_time);
	c->compile_count = i;
	pthread_mutex_unlock(c->check_time);
}

void	set_finish(t_coder *c)
{
	pthread_mutex_lock(c->check_time);
	c->finish = 1;
	pthread_mutex_unlock(c->check_time);
}

int	abort_acquire(t_coder *c, t_dongle *d, char ch)
{
	if (c->id %2 == 0)
	{
		pthread_mutex_unlock(&d->mutex_v);
		if (ch == 'r')
			pthread_mutex_unlock(&c->left_d->mutex_v);
	}else
	{
		pthread_mutex_unlock(&d->mutex_v);
		if (ch == 'l')
			pthread_mutex_unlock(&c->right_d->mutex_v);
	}

	return (0);
}

void	set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n)
{
	if (((i + 1) % n) < i)
	{
		coders[i].left_d = &dongles[(i + 1) % n];
		coders[i].right_d = &dongles[i];
	}
	else
	{
		coders[i].left_d = &dongles[i];
		coders[i].right_d = &dongles[(i + 1) % n];
	}
}

int	set_queue_val(t_coder *c, t_dongle *d)
{
	long long	v;

	pthread_mutex_lock(c->check_time);
	v = c->last_compile_start;
	pthread_mutex_unlock(c->check_time);
	if (d->arb == 2)
		return (v + c->shared->t_burnout);
	return (c->id);
}
