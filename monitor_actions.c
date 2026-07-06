/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   monitor_actions.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/05 21:37:40 by username         #+#    #+#              */
/*   Updated: 2026/07/05 21:37:40 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_monitor *m, int i)
{
	long long	current_time;
	long long	last_compile_start;
	long long	time_to_burnout;
	int			n;

	pthread_mutex_lock(&m->coders[i].safe_check);
	current_time = ft_time();
	last_compile_start = m->coders[i].last_compile_start;
	time_to_burnout = m->coders[i].shared->t_burnout;
	n = m->coders[i].finish;
	pthread_mutex_unlock(&m->coders[i].safe_check);
	if (current_time - last_compile_start > time_to_burnout && n == 0)
		return (1);
	return (0);
}

void	stop_wake_all(t_monitor *m)
{
	int		i;
	t_coder	c;

	i = 0;
	c = m->coders[0];
	pthread_mutex_lock(&c.shared->m_check);
	while (i < m->num_of_coders)
	{
		m->coders[i].stop = 1;
		i++;
	}
	pthread_mutex_unlock(&c.shared->m_check);
	pthread_cond_broadcast(&c.shared->global_cond);
}

void	check_finished(t_monitor *m, int i, int *finished, int *counter)
{
	int	n;
	int	compile_count;

	pthread_mutex_lock(&m->coders[i].safe_check);
	n = m->coders[i].finish;
	compile_count = m->coders[i].compile_count;
	pthread_mutex_unlock(&m->coders[i].safe_check);
	if (n == 1)
	{
		(*finished)++;
		if (compile_count == m->coders[i].shared->n_compiles)
			(*counter)++;
	}
}
