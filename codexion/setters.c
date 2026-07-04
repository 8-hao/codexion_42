#include "codexion.h"

void	set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n)
{
	if (i == n - 1)
	{
		coders[i].left_d = &dongles[0];
		coders[i].right_d = &dongles[i];
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