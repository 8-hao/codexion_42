#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>


typedef struct shared
{
	int				n_coders;
	int				t_burnout;
	int				t_compile;
	int				t_debug;
	int				t_refactor;
	int				n_compiles;
	int				d_cooldown;
	int				c;
	pthread_mutex_t	p_safe;
}			t_shared;

typedef struct queue
{
	struct coder	*c;
	long long		index;
	struct queue	*next;
}			t_queue;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	pthread_cond_t	cond_v;
	long long		release_time;
	t_queue			*headq;
	int				cooldown;
	int				is_available;
	int				arb;
}			t_dongle;

typedef struct coder
{
	int				id;
	int				compile_count;
	long long		last_compile_start;
	long long		init_time;
	int				finish;
	int				stop;
	t_shared		*shared;
	t_dongle		*left_d;
	t_dongle		*right_d;
	pthread_mutex_t	*check_time;
}			t_coder;

typedef struct monitor
{
	int			num_of_coders;
	t_coder		*coders;
	t_dongle	*dongles;
}			t_monitor;

t_shared	*parser(int argc, char **argv);
t_dongle	*dongles_initializer(t_shared *data);
t_coder	    *coders_init(t_shared *data, t_dongle *dongles);
void	    monitor_init(t_monitor *monitor, t_dongle *dongles, t_coder *c);
void	    threads_join(pthread_t *threads, int n);
void	    free_all(t_dongle *dongles, t_coder *coders, pthread_t *threads);
void	    set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n);
int	        threads_init(pthread_t *t, t_coder *coders, void *(*f)(void *));
int	        ft_time(void);

#endif