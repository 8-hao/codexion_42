/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_actions.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/05 21:37:13 by username         #+#    #+#              */
/*   Updated: 2026/07/05 21:37:13 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiling(t_coder *c)
{
	if (is_stopped(c))
	{
		pthread_mutex_unlock(&c->left_d->mutex_v);
		pthread_mutex_unlock(&c->right_d->mutex_v);
		return (0);
	}
	safe_print("is compiling", c);
	pthread_mutex_lock(&c->safe_check);
	c->last_compile_start = ft_time();
	pthread_mutex_unlock(&c->safe_check);
	if (ft_smartsleep(c->shared->t_compile, c) == 0)
	{
		pthread_mutex_unlock(&c->left_d->mutex_v);
		pthread_mutex_unlock(&c->right_d->mutex_v);
		return (0);
	}
	return (1);
}

int	battle(t_coder *c)
{
	struct timespec	big;
	t_dongle		*left;
	t_dongle		*right;

	left = c->left_d;
	right = c->right_d;
	big = get_max_timespec(right->cooldown_time, left->cooldown_time);
	while (timespec_less(set_time(0), big))
		pthread_cond_timedwait(&c->shared->global_cond,
			&c->shared->m_check, &big);
	if (right->is_available && left->is_available
		&& left->headq != NULL && c->id == left->headq->c->id
		&& right->headq != NULL && c->id == right->headq->c->id)
		return (1);
	return (0);
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

int	acquire_dongles(t_coder *c)
{
	pthread_mutex_lock(&c->shared->m_check);
	if (is_stopped(c))
		return (pthread_mutex_unlock(&c->shared->m_check), 0);
	if (is_inqueue(c->left_d->headq, c))
	{
		add_back(&c->left_d->headq, newnode(c, set_queue_val(c, c->left_d)));
		if (c->left_d->arb == 2)
			sort_min(&c->left_d->headq);
	}
	if (is_inqueue(c->right_d->headq, c))
	{
		add_back(&c->right_d->headq, newnode(c, set_queue_val(c, c->right_d)));
		if (c->right_d->arb == 2)
			sort_min(&c->right_d->headq);
	}
	while (!battle(c))
	{
		if (is_stopped(c))
			break ;
		pthread_cond_wait(&c->shared->global_cond, &c->shared->m_check);
	}
	if (is_stopped(c))
		return (pthread_mutex_unlock(&c->shared->m_check), 0);
	lock_both_dongles(c->left_d, c->right_d);
	c->left_d->is_available = 0;
	c->right_d->is_available = 0;
	safe_print("has taken a dongle", c);
	safe_print("has taken a dongle", c);
	free(deletefirst(&c->left_d->headq));
	free(deletefirst(&c->right_d->headq));
	pthread_mutex_unlock(&c->shared->m_check);
	if (is_stopped(c))
		return (pthread_mutex_unlock(&c->left_d->mutex_v),
			pthread_mutex_unlock(&c->right_d->mutex_v), 0);
	return (1);
}

int	release_dongles(t_coder *c)
{
	long long	now;

	pthread_mutex_unlock(&c->left_d->mutex_v);
	pthread_mutex_unlock(&c->right_d->mutex_v);
	pthread_mutex_lock(&c->shared->m_check);
	now = ft_time();
	c->left_d->is_available = 1;
	c->right_d->is_available = 1;
	c->left_d->release_time = now;
	c->right_d->release_time = now;
	c->right_d->cooldown_time = set_time(c->right_d->cooldown);
	c->left_d->cooldown_time = set_time(c->left_d->cooldown);
	pthread_cond_broadcast(&c->shared->global_cond);
	pthread_mutex_unlock(&c->shared->m_check);
	if (is_stopped(c))
		return (0);
	return (1);
}
