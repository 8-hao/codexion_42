/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   threads.h                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:08:26 by username         #+#    #+#              */
/*   Updated: 2026/06/23 22:19:10 by username        ###   ########.fr        */
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
	Queue			*headq;
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
	long long		init_time;
	Dongle			*left_d;
	Dongle			*right_d;
	long long		init_time;
	pthread_mutex_t	*check_time;

}	t_coder;

typedef struct monitor
{
	int		num_of_coders;
	Coder	*coders;
	Dongle	*dongles;
}	t_monitor;

int		*parser(int argc, char **argv);
void	ft_codexion(int *data);
Queue	*newnode(Coder *c, int index);
void	add_back(Queue **head, Queue *node);
int		queuelen(Queue *head);
void	sort_min(Queue **head);
int		is_inqueue(Queue *head, Coder *c);
int		ft_smartsleep(int time_to_sleep, Coder *c);
void	ft_print(Queue *head);
void	ft_time_to_sleep(struct timespec *t, int delay_ms);
Queue	*deletefirst(Queue **head);
Dongle	*dongles_initializer(int num, int cooldown);
Coder	*coders_initializer(int *data, Dongle *dongles);
void	m_init(Monitor *monitor, int *data, Dongle *dongles, Coder *coders);
void	threads(pthread_t *ids, int *data, Coder *coders, void * (*f)(void *));
void	threads_join(pthread_t *id_threads, pthread_t id_monitor, int *data);
void	check_finished(Monitor *m, int i, int *finished, int *counter);
int		acquire_dongle(Coder *c, Dongle *d, char ch);
void	release_dongle(Dongle *d);
int		compiling(Coder *c);
int		debug_and_refactor(Coder *c);

long long	ft_time(void);

#endif
