/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 00:25:39 by obakri            #+#    #+#             */
/*   Updated: 2026/06/28 00:25:45 by obakri           ###   ########.fr       */
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
}	t_shared;

typedef struct queue
{
	struct coder	*c;
	int				index;
	struct queue	*next;
}	t_queue;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	pthread_cond_t	cond_v;
	long long		release_time;
	t_queue			*headq;
	int				cooldown;
	int				is_available;
	int				arb;
}	t_dongle;

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
}	t_coder;

typedef struct monitor
{
	int			num_of_coders;
	t_coder		*coders;
	t_dongle	*dongles;
}	t_monitor;

t_shared	*parser(int argc, char **argv);
t_dongle	*dongles_initializer(t_shared *data);
t_coder		*coders_init(t_shared *data, t_dongle *dongles);
t_queue		*newnode(t_coder *c, int index);
t_queue		*deletefirst(t_queue **head);

long long	ft_time(void);

int			is_stopped(t_coder *c);
int			queuelen(t_queue *head);
int			is_inqueue(t_queue *head, t_coder *c);
int			ft_smartsleep(int time_to_sleep, t_coder *c);
int			acquire_dongle(t_coder *c, t_dongle *d, char ch);
int			compiling(t_coder *c);
int			debug_and_refactor(t_coder *c);
int			abort_acquire(t_coder *c, t_dongle *d, char ch);
int			threads_init(pthread_t *threads, t_coder *c, void *(*f)(void *));
int			set_queue_val(t_coder *c, t_dongle *d);

void		threads_join(pthread_t *threads, int n, pthread_t	id_monitor);
void		check_finished(t_monitor *m, int i, int	*finished, int *counter);
void		free_all(t_dongle *dongles, t_coder *coders, pthread_t	*threads);
void		ft_codexion(t_shared *data);
void		release_dongle(t_coder *c, t_dongle *d);
void		ft_time_to_sleep(struct timespec *t, int delay_ms);
void		add_back(t_queue **head, t_queue *node);
void		sort_min(t_queue **head);
void		monitor_init(t_monitor *monitor, t_dongle *dongles, t_coder	*c);
void		stop_all(t_monitor *m);
void		set_compile_count(t_coder *c, int i);
void		set_finish(t_coder *c);
void		set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n);

#endif
