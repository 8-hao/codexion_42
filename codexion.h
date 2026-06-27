/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/27 10:24:23 by username         #+#    #+#              */
/*   Updated: 2026/06/27 10:24:23 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

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
	int		n_coders;
	int		t_burnout;
	int		t_compile;
	int		t_debug;
	int		t_refactor;
	int		n_compiles;
	int		d_cooldown;
	char	c;

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
	long long		release_time;
	int				cooldown;
	int				is_available;
	t_queue			*headq;
	pthread_cond_t	cond_v;
	int				arb;

}	t_dongle;

typedef struct coder
{
	int			id;
	int			compile_count;
	t_shared	*shared;

	t_dongle		*left_d;
	t_dongle		*right_d;
	pthread_mutex_t	*check_time;
	long long		last_compile_start;
	long long		init_time;
	int				finish;
	int				stop;

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

int	acquire_dongle(t_coder *c, t_dongle *d, char ch);
int	compiling(t_coder *c);
int	ft_smartsleep(int time_to_sleep, t_coder *c);
int	debug_and_refactor(t_coder *c);
int	threads_init(pthread_t *t, t_coder *coders, void * (*f)(void *));
int	queuelen(t_queue *head);
int	is_inqueue(t_queue *head, t_coder *c);

void	monitor_init(t_monitor *monitor, t_dongle *dongles, t_coder *c);
void	release_dongle(t_dongle *d);
void	threads_join(pthread_t *threads, int n, pthread_t id_monitor);
void	ft_time_to_sleep(struct timespec *t, int delay_ms);
void	free_all(t_dongle *dongles, t_coder *coders, pthread_t *threads);
void	ft_codexion(t_shared *data);
void	check_finished(t_monitor *m, int i, int *finished, int *counter);
void	add_back(t_queue **head, t_queue *node);
void	sort_min(t_queue **head);

#endif
