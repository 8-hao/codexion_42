#include "codexion.h"


void release_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex_v);
	d->is_available = 1;
	d->release_time = ft_time();
	d->cooldown_time = (long long) ft_time() + d->cooldown;
	pthread_cond_signal(&d->cond_v);
	pthread_mutex_unlock(&d->mutex_v);
}

int	compiling(t_coder *c)
{
	safe_print("is compiling",c);
	ft_smartsleep(c->shared->t_compile, c);
	return 1;
}


static int is_dongle_avail(t_coder *c, t_dongle *d, int *ch)
{
	int i;
	pthread_mutex_lock(&d->mutex_v);
	if(is_inqueue(d->headq, c))
	{
		add_back(&d->headq, newnode(c, set_queue_val(c, d)));
		if(c->shared->c == 2)
			sort_min(&d->headq);
	}
	pthread_mutex_lock(c->check_time);
	i = d->is_available;
	pthread_mutex_unlock(c->check_time);
	if (i == 0 || d->cooldown_time > ft_time()  || d->headq->c->id != c->id)
	{
		if (d->cooldown_time > ft_time())
		*ch = d->cooldown_time - ft_time();
		pthread_mutex_unlock(&d->mutex_v);
		return *ch;
	}
	pthread_mutex_unlock(&d->mutex_v);
	return 1;
}

int max_value(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

void acquire_dongles(t_coder *c)
{
	struct timespec	tmp;
	int l;
	int r;
	int max;

	max = 0;
	l = 0;
	r = 0;
	while(is_dongle_avail(c, c->left_d, &l) != 1 || is_dongle_avail(c, c->right_d, &r) != 1)
	{
		if(l != 0 || r != 0)
		{
			max = max_value(l, r);
			ft_time_to_sleep(&tmp, max);
			if (l == max)
				pthread_cond_timedwait(&c->left_d->cond_v, &c->left_d->mutex_v, &tmp);
			else
				pthread_cond_timedwait(&c->right_d->cond_v, &c->right_d->mutex_v, &tmp);
		}else{
			if (l ==0)
				pthread_cond_wait(&c->left_d->cond_v,&c->left_d->mutex_v);   
			else
				pthread_cond_wait(&c->right_d->cond_v, &c->right_d->mutex_v);
		}
	}


	c->left_d->is_available = 0;
	safe_print("took a dongle", c);
	c->right_d->is_available = 0;
	safe_print("took a dongle", c);
	free(deletefirst(&c->left_d->headq));
	free(deletefirst(&c->right_d->headq));

	pthread_mutex_unlock(&c->left_d->mutex_v);
	pthread_mutex_unlock(&c->right_d->mutex_v);    
}