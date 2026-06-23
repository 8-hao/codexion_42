/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   threads.h                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:08:26 by username         #+#    #+#              */
/*   Updated: 2026/06/22 15:50:00 by username        ###   ########.fr        */
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
}	Queue;

typedef struct dongle
{
	pthread_mutex_t	mutex_v;
	long long		release_time;
	int				cooldown;
	int				is_available;
	Queue			*headq;
	pthread_cond_t	cond_v;
	int				arb;

}	Dongle;

typedef struct coder
{
	int	id;
	int	num_of_compiles_required;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	time_to_burnout;

	Dongle	*left_d;
	Dongle	*right_d;

	pthread_mutex_t	*check_time;

	int			compile_count;
	long long	last_compile_start;
	long long	init_time;

	int	finish;
	int	stop;

}	Coder;

typedef struct monitor
{
	int			num_of_coders;
	Coder		*coders;
	Dongle		*dongles;
}	Monitor;

int		*parser(int argc, char **argv);
void	ft_codexion(int *data);

Queue	*newnode(Coder *c, int index);
void	add_back(Queue **head, Queue *node);
int		queuelen(Queue *head);
void	sort_min(Queue **head);
int		is_inqueue(Queue *head, Coder *c);
void	ft_print(Queue *head);
Queue	*deletefirst(Queue **head);



Dongle	*dongles_initializer(int num, int cooldown);
Coder	*coders_initializer(int *data, Dongle *dongles);
void	monitor_initializer(Monitor *monitor, int *data, Dongle *dongles, Coder *coders);
void threads_creation(pthread_t *id_threads, int *data, Coder *coders, void *(*func)(void*));
void threads_join(pthread_t *id_threads, pthread_t	id_monitor, int *data);


int	ft_smartsleep(int time_to_sleep, Coder *c);
void	ft_time_to_sleep(struct timespec *t, int delay_ms);
long long	ft_time(void);
void check_finished(Monitor *m, int i, int *finished, int *counter);


int acquire_dongle(Coder *c, Dongle *d, char ch);
void release_dongle(Dongle *d);
int compiling(Coder *c);
int debug_and_refactor(Coder *c);

#endif
