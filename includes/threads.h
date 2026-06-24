/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   threads.h                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:08:26 by username         #+#    #+#              */
/*   Updated: 2026/06/24 16:18:33 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct queue
{
	struct coder	*c;
	int				index;
	struct queue	*next;
}	t_queue;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	long long		release_time;
	int				cooldown;
	int				is_available;
	t_queue			*headq;
	pthread_cond_t	cond_v;
	int				arb;

}	t_dongle;

typedef struct coder
{
	int				id;
	int				num_of_compiles_required;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				time_to_burnout;
	int				compile_count;
	int				finish;
	int				stop;
	long long		last_compile_start;
	t_dongle		*left_d;
	t_dongle		*right_d;
	long long		init_time;
	pthread_mutex_t	*check_time;

}	t_coder;

typedef struct monitor
{
	int			num_of_coders;
	t_coder		*coders;
	t_dongle	*dongles;
}	t_monitor;

int			*parser(int argc, char **argv);
void		ft_codexion(int *data);
t_queue		*newnode(t_coder *c, int index);
void		add_back(t_queue **head, t_queue *node);
int			queuelen(t_queue *head);
void		sort_min(t_queue **head);
int			is_inqueue(t_queue *head, t_coder *c);
int			ft_smartsleep(int time_to_sleep, t_coder *c);
void		ft_print(t_queue *head);
void		ft_time_to_sleep(struct timespec *t, int delay_ms);
t_queue		*deletefirst(t_queue **head);
t_dongle	*dongles_initializer(int num, int cooldown);
t_coder		*coders_initializer(int *data, t_dongle *dongles);
void		m_init(t_monitor *monitor, int *data, t_dongle *dongles, t_coder *c);
int		threads(pthread_t *ids, int *data, t_coder *coders, void * (*f)(void *));
void		threads_join(pthread_t *id_threads, pthread_t id_monitor, int *data);
void		check_finished(t_monitor *m, int i, int *finished, int *counter);
int			acquire_dongle(t_coder *c, t_dongle *d, char ch);
void		release_dongle(t_dongle *d);
void 		free_all(t_dongle	*dongles, t_coder *coders);
int			compiling(t_coder *c);
int			debug_and_refactor(t_coder *c);

long long	ft_time(void);

#endif
