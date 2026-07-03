
#include "codexion.h"

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