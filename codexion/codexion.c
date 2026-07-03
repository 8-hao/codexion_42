
#include "codexion.h"

void *coder_func(void *args)
{
    t_coder *c;

    c = (t_coder *) args;
    printf("%d\n",c->id);
    return NULL;
}

static void ft_codexion(t_shared *data)
{
	t_coder		*coders;
	t_dongle	*dongles;
	// t_monitor	monitor;
	pthread_t	*threads;
	//pthread_t	id_monitor;

	dongles = dongles_initializer(data);
	if (dongles == NULL)
        return;
	coders = coders_init(data, dongles);
	if (coders == NULL)
	 	return ((void) free_all(dongles, coders, NULL));
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	if (threads == NULL)
		return ((void) free_all(dongles, coders, NULL));
	if (threads_init(threads, coders, coder_func) == 0)
	 	return ((void) free_all(dongles, coders, NULL));
	// monitor_init(&monitor, dongles, coders);
	// pthread_create(&id_monitor, NULL, monitor_func, &monitor);
	threads_join(threads, data->n_coders);
	free_all(dongles, coders, threads);
}

int	main(int argc, char **argv)
{
	t_shared	*data;

	if (argc != 9)
	{
		printf("Error: Expected 8 arguments, but received %d.\n", argc - 1);
		return (1);
	}
	data = parser(argc - 1, argv);
	if (data == NULL)
		return (1);
	ft_codexion(data);
	free(data);
	return (0);
}
