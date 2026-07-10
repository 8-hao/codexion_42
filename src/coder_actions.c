/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.1337.ma>              +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:46:24 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:46:28 by obakri           ###   ########.fr       */
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

	if (c->shared->n_coders == 1)
		return (1);
	left = c->left_d;
	right = c->right_d;
	big = get_max_timespec(right->cooldown_time, left->cooldown_time);
	while (timespec_less(set_time(0), big) && !is_stopped(c))
	{
		if (is_stopped(c))
			return (0);
		pthread_cond_timedwait(&c->shared->global_cond,
			&c->shared->m_check, &big);
	}
	if (is_stopped(c))
		return (0);
	if (right->is_available && left->is_available
		&& left->headq != NULL && c->id == left->headq->c->id
		&& right->headq != NULL && c->id == right->headq->c->id)
		return (1);
	return (0);
}

void	delete_coder_q(t_coder *c)
{
	free(deletefirst(&c->left_d->headq));
	free(deletefirst(&c->right_d->headq));
}

int	acquire_dongles(t_coder *c)
{
	pthread_mutex_lock(&c->shared->m_check);
	if (is_stopped(c))
		return (pthread_mutex_unlock(&c->shared->m_check), 0);
	add_to_queue(c->left_d, c);
	add_to_queue(c->right_d, c);
	while (!battle(c))
	{
		if (is_stopped(c))
			break ;
		pthread_cond_wait(&c->shared->global_cond, &c->shared->m_check);
	}
	if (is_stopped(c))
		return (pthread_mutex_unlock(&c->shared->m_check), 0);
	if (lock_both_dongles(c->left_d, c->right_d) == 0)
		return (pthread_mutex_unlock(&c->shared->m_check), 0);
	c->left_d->is_available = 0;
	c->right_d->is_available = 0;
	safe_print("has taken a dongle", c);
	safe_print("has taken a dongle", c);
	delete_coder_q(c);
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
