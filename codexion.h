/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:44:14 by obakri            #+#    #+#             */
/*   Updated: 2026/07/07 20:44:19 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pthread_mutex_t	m_check;
	pthread_cond_t	global_cond;
}	t_shared;

typedef struct queue
{
	struct coder	*c;
	long long		index;
	struct queue	*next;
}	t_queue;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	pthread_cond_t	cond_v;
	long long		release_time;
	struct timespec	cooldown_time;
	t_queue			*headq;
	int				cooldown;
	int				is_available;
	int				arb;
}	t_dongle;

typedef struct coder
{
	pthread_t		a;
	int				id;
	int				compile_count;
	long long		last_compile_start;
	long long		init_time;
	int				finish;
	int				stop;
	t_shared		*shared;
	t_dongle		*left_d;
	t_dongle		*right_d;
	pthread_mutex_t	safe_check;
}	t_coder;

typedef struct monitor
{
	int			num_of_coders;
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	id_monitor;
}	t_monitor;

t_shared		*parser(int argc, char **argv);
t_dongle		*dongles_initializer(t_shared *data);
t_coder			*coders_init(t_shared *data, t_dongle *dongles);
t_queue			*newnode(t_coder *c, long long index);
t_queue			*deletefirst(t_queue **head);
struct timespec	set_time(long time_to_burnout);
struct timespec	get_max_timespec(struct timespec t1, struct timespec t2);

long long		ft_time(void);
long long		set_queue_val(t_coder *c, t_dongle *d);

void			free_all(t_dongle *dongles, t_coder *c, pthread_t *threads);
void			set_dongle_pair(t_coder *c, t_dongle *dongles, int i, int n);
void			add_back(t_queue **head, t_queue *node);
void			sort_min(t_queue **head);
void			safe_print(char *s, t_coder *c);
void			ft_time_to_sleep(struct timespec *t, int delay_ms);
void			threads_join(pthread_t *threads, int n, pthread_t id_monitor);
void			monitor_init(t_monitor *m, t_dongle *dongles, t_coder *c);
void			stop_wake_all(t_monitor *m);
void			check_finished(t_monitor *m, int i, int *finished, int *c);
int				lock_both_dongles(t_dongle *a, t_dongle *b);
void			add_to_queue(t_dongle *d, t_coder *c);

int				set_compile_count(t_coder *c, int i);
int				is_inqueue(t_queue *head, t_coder *c);
int				queuelen(t_queue *head);
int				threads_init(pthread_t *t, t_coder *coders, void *(*f)(void *));
int				ft_smartsleep(int time_to_sleep, t_coder *c);
int				compiling(t_coder *c);
int				debug_and_refactor(t_coder *c);
int				check_burnout(t_monitor *m, int i);
int				is_stopped(t_coder *c);
int				release_dongles(t_coder *c);
int				acquire_dongles(t_coder *c);
int				timespec_less(struct timespec a, struct timespec b);

#endif
