/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   setters.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/27 22:22:11 by username         #+#    #+#              */
/*   Updated: 2026/07/03 09:40:23 by username        ###   ########.fr        */
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
	(void)ch;
	pthread_mutex_unlock(&d->mutex_v);
	if (c->left_d < c->right_d)
	{
		if (d == c->right_d)
			pthread_mutex_unlock(&c->left_d->mutex_v);
	}
	else
	{
		if (d == c->left_d)
			pthread_mutex_unlock(&c->right_d->mutex_v);
	}
	return (0);
}

void	set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n)
{
	if (i == n - 1)
	{
		coders[i].left_d = &dongles[i];
		coders[i].right_d = &dongles[0];
	}
	else
	{
		coders[i].left_d = &dongles[i];
		coders[i].right_d = &dongles[i + 1];
	}
}

long long	set_queue_val(t_coder *c, t_dongle *d)
{
	long long	v;

	pthread_mutex_lock(c->check_time);
	v = c->last_compile_start;
	pthread_mutex_unlock(c->check_time);
	if (d->arb == 2)
		return ((long long) v + c->shared->t_burnout);
	return (ft_time());
}
