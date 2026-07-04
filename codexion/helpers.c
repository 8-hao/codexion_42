
#include "codexion.h"

void	safe_print(char *s, t_coder *c)
{
	long long	t;

	t = ft_time() - c->init_time;
	pthread_mutex_lock(&c->shared->p_safe);
	printf("%lld %d %s\n", t, c->id, s);
	pthread_mutex_unlock(&c->shared->p_safe);
}

void	free_all(t_dongle *dongles, t_coder *coders, pthread_t *threads)
{
	if (coders != NULL)
	{
		free(coders[0].check_time);
		free(coders);
	}
	if (dongles != NULL)
		free(dongles);
	if (threads != NULL)
		free(threads);
}

t_queue	*deletefirst(t_queue **head)
{
	t_queue	*first_node;

	if (*head == NULL)
		return (NULL);
	first_node = *head;
	*head = (*head)->next;
	first_node->next = NULL;
	return (first_node);
}

int	is_stopped(t_coder *c)
{
	int	s;

	pthread_mutex_lock(c->check_time);
	s = c->stop;
	pthread_mutex_unlock(c->check_time);
	return (s);
}