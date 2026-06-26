#include "codexion.h"

void *coder_func(void *coders)
{
    t_coder *c;
    // int i;

    c = (t_coder *)coders;
    printf("%d",c->shared->n_compiles );
    // i = 0;
    //  while(i++ < c->shared->n_compiles)
    // // {
    //     if (c->stop)
	// 		return (NULL);
	// 	if (acquire_dongle(c, c->left_d, 'l') == 0)
	// 		return (NULL);
	// 	if (acquire_dongle(c, c->right_d, 'r') == 0)
	// 		return (NULL);
	// 	// if (compiling(c) == 0)
	// 	//  	return (NULL);
	// 	release_dongle(c->left_d);
	// 	release_dongle(c->right_d);
	// 	// if (debug_and_refactor(c) == 0)
	// 	// 	return (NULL);
	// 	c->compile_count = i;
    // }
    return NULL;
}

void	ft_codexion(t_shared *data)
{
    t_coder		*coders;
	t_dongle	*dongles;
	// Monitor		monitor;
	pthread_t	*threads;
	//pthread_t	id_monitor;
	dongles = dongles_initializer(data);
	if (dongles == NULL)
		return ;
	coders = coders_init(data, dongles);
	if (coders == NULL){
        free_all(dongles, coders, NULL);
        return;
    }
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	if (threads == NULL){
        free_all(dongles, coders, NULL);
        return;
    }
    if (threads_init(threads, coders, coder_func) == 0)
	{
		free_all(dongles, coders, threads);
		return;
	}
	// monitor_initializer(&monitor, data, dongles, coders);
	// pthread_create(&id_monitor, NULL, monitor_func, &monitor);
	threads_join(threads, data->n_coders);
    free_all(dongles, coders, threads);
}

//15:34