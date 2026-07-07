/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:46:10 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:46:13 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

	pthread_mutex_lock(&c->safe_check);
	v = c->last_compile_start;
	pthread_mutex_unlock(&c->safe_check);
	if (d->arb == 2)
		return (v + c->shared->t_burnout);
	return (ft_time());
}

int	set_compile_count(t_coder *c, int i)
{
	if (is_stopped(c))
		return (0);
	pthread_mutex_lock(&c->safe_check);
	c->compile_count = i;
	pthread_mutex_unlock(&c->safe_check);
	if (is_stopped(c))
		return (0);
	return (1);
}

int	lock_both_dongles(t_dongle *a, t_dongle *b)
{
	if (a == b)
		return (0);
	if (a < b)
	{
		pthread_mutex_lock(&a->mutex_v);
		pthread_mutex_lock(&b->mutex_v);
	}
	else
	{
		pthread_mutex_lock(&b->mutex_v);
		pthread_mutex_lock(&a->mutex_v);
	}
	return (1);
}

void	add_to_queue(t_dongle *d, t_coder *c)
{
	if (c->shared->n_coders == 1)
		return ;
	if (is_inqueue(d->headq, c))
	{
		add_back(&d->headq, newnode(c, set_queue_val(c, d)));
		if (d->arb == 2)
			sort_min(&d->headq);
	}
}
