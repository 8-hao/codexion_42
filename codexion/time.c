#include "codexion.h"

int	ft_time(void)
{
	struct timeval	d;
	long long		time;

	gettimeofday(&d, NULL);
	time = d.tv_sec * 1000 + d.tv_usec / 1000;
	return (time);
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

void	ft_time_to_sleep(struct timespec *t, int delay_ms)
{
	struct timeval	d;
	long long		total_nsec;

	gettimeofday(&d, NULL);
	total_nsec = (long long) d.tv_usec * 1000 + (long long) delay_ms * 1000000;
	t->tv_sec = d.tv_sec + (total_nsec / 1000000000);
	t->tv_nsec = total_nsec % 1000000000;
}