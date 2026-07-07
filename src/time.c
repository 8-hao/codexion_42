/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:46:24 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:46:28 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	ft_time(void)
{
	struct timeval	d;
	long long		time;

	gettimeofday(&d, NULL);
	time = d.tv_sec * 1000 + d.tv_usec / 1000;
	return (time);
}

struct timespec	set_time(long time_to_burnout)
{
	struct timeval	now;
	struct timespec	ts;
	long			add_sec;
	long			add_usec;

	gettimeofday(&now, NULL);
	add_sec = time_to_burnout / 1000;
	add_usec = (time_to_burnout % 1000) * 1000;
	now.tv_sec += add_sec;
	now.tv_usec += add_usec;
	if (now.tv_usec >= 1000000)
	{
		now.tv_sec++;
		now.tv_usec -= 1000000;
	}
	ts.tv_sec = now.tv_sec;
	ts.tv_nsec = now.tv_usec * 1000;
	return (ts);
}

struct timespec	get_max_timespec(struct timespec t1, struct timespec t2)
{
	struct timespec	max;

	max = t1;
	if (t2.tv_sec > max.tv_sec
		|| (t2.tv_sec == max.tv_sec && t2.tv_nsec > max.tv_nsec))
		max = t2;
	return (max);
}

int	timespec_less(struct timespec a, struct timespec b)
{
	if (a.tv_sec < b.tv_sec)
		return (1);
	if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec)
	{
		return (1);
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec)
	{
		return (1);
	}
	return (0);
}

int	ft_smartsleep(int time_to_sleep, t_coder *c)
{
	long long	start;

	start = ft_time();
	while (ft_time() - start < time_to_sleep)
	{
		if (is_stopped(c))
			return (0);
		usleep(500);
	}
	return (1);
}
