/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:44:56 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:45:00 by obakri           ###   ########.fr       */
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
		pthread_mutex_lock(&m->coders[i].safe_check);
		m->coders[i].stop = 1;
		pthread_mutex_unlock(&m->coders[i].safe_check);
		i++;
	}
	pthread_cond_broadcast(&c.shared->global_cond);
	pthread_mutex_unlock(&c.shared->m_check);
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
