#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>


typedef struct shared
{
    int n_coders;
    int	t_burnout;
	int	t_compile;
	int	t_debug;
	int	t_refactor;
    int	n_compiles;
    int d_cooldown;
    char c;

}t_shared;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	long long		release_time;
	int				cooldown;
	int				is_available;
	//Queue			*headq;
	pthread_cond_t	cond_v;
	int				arb;

}	t_dongle;

typedef struct coder
{
 	int	id;
    int	compile_count;
    t_shared *shared;

 	t_dongle	*left_d;
 	t_dongle	*right_d;
 	pthread_mutex_t	*check_time;
 	long long	last_compile_start;
 	long long	init_time;
 	int	finish;
 	int	stop;

}	t_coder;

t_shared *parser(int argc, char **argv);
void	ft_codexion(t_shared *data);


t_dongle    *dongles_initializer(t_shared *data);
t_coder     *coders_init(t_shared *data, t_dongle *dongles);
int        threads_init(pthread_t *t, t_coder *coders, void * (*f)(void *));

long long	ft_time(void);


void free_all(t_dongle	*dongles, t_coder *coders, pthread_t *threads);
void	threads_join(pthread_t *threads, int n);
void	ft_time_to_sleep(struct timespec *t, int delay_ms);

void	release_dongle(t_dongle *d);
int	acquire_dongle(t_coder *c, t_dongle *d, char ch);
int	compiling(t_coder *c);
int	ft_smartsleep(int time_to_sleep, t_coder *c);


#endif