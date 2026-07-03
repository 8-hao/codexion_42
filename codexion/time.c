#include "codexion.h"

int	ft_time(void)
{
	struct timeval	d;
	long long		time;

	gettimeofday(&d, NULL);
	time = d.tv_sec * 1000 + d.tv_usec / 1000;
	return (time);
}

