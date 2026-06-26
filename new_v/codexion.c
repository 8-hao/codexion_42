#include "codexion.h"

int	check_burnout(t_monitor *m, int i)
{
	long long	current_time;
	long long	last_compile_start;
	long long	time_to_burnout;

	pthread_mutex_lock(m->coders[i].check_time);
	current_time = ft_time();
	last_compile_start = m->coders[i].last_compile_start;
	time_to_burnout = m->coders[i].shared->t_burnout;
	pthread_mutex_unlock(m->coders[i].check_time);
	if (current_time - last_compile_start >= time_to_burnout && m->coders[i].finish == 0)
	{
		printf("%lld %d is burned out\n", current_time - m->coders[i].init_time, m->coders[i].id);
		return (1);
	}
	return (0);
}

void	stop_all(t_monitor *m)
{
	int	i;

	i = 0;
	while (i < m->num_of_coders)
	{
		m->coders[i].stop = 1;
		i++;
	}
	//pthread_cond_broadcast();
}
void	wake_all_dongles(t_monitor *m)
{
	int	i;

	i = 0;
	while (i < m->num_of_coders)
	{
		pthread_mutex_lock(&m->dongles[i].mutex_v);
		pthread_cond_broadcast(&m->dongles[i].cond_v);
		pthread_mutex_unlock(&m->dongles[i].mutex_v);
		i++;
	}
}
static void	*monitor_func(void *monitor)
{
	t_monitor	*m;
	int			i;
	int			counter;
	int			finished;

	m = (t_monitor *) monitor;
	while (1)
	{
		i = 0;
		counter = 0;
		finished = 0;
		while (i < m->num_of_coders)
		{
			if (check_burnout(m, i))
				return (stop_all(m), wake_all_dongles(m), NULL);
			check_finished(m, i, &finished, &counter);
			i++;
		}
		if (finished == m->num_of_coders && finished == counter)
			return (NULL);
		usleep(1000);
	}
	
	return (NULL);
}

static void *coder_func(void *coders)
{
    t_coder *c;
     int i;

    c = (t_coder *)coders;
    i = 0;
    while(i++ < c->shared->n_compiles)
    {
        if (c->stop)
	 		return (NULL);
	 	if (acquire_dongle(c, c->left_d, 'l') == 0)
	 		return (NULL);
	 	if (acquire_dongle(c, c->right_d, 'r') == 0)
	 		return (NULL);
	 	if (compiling(c) == 0)
	 	  	return (NULL);
	 	release_dongle(c->left_d);
	 	release_dongle(c->right_d);
		if (c->stop)
	 		return (NULL);
	 	if (debug_and_refactor(c) == 0)
	 		return (NULL);
	 	c->compile_count = i;
    }
	c->finish = 1;
    return NULL;
}

void	ft_codexion(t_shared *data)
{
    t_coder		*coders;
	t_dongle	*dongles;
	t_monitor		monitor;
	pthread_t	*threads;
	pthread_t	id_monitor;
	dongles = dongles_initializer(data);
	if (dongles == NULL)
		return ;
	coders = coders_init(data, dongles);
	if (coders == NULL){
        free_all(dongles, coders, NULL);
        return;
    }
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	if (threads == NULL){
        free_all(dongles, coders, NULL);
        return;
    }
    if (threads_init(threads, coders, coder_func) == 0)
	{
		free_all(dongles, coders, threads);
		return;
	}
	monitor_init(&monitor, dongles, coders);
	pthread_create(&id_monitor, NULL, monitor_func, &monitor);
	threads_join(threads, data->n_coders, id_monitor);
	free_all(dongles, coders, threads);
}

//15:34