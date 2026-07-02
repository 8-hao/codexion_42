/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 22:22:39 by obakri            #+#    #+#             */
/*   Updated: 2026/06/27 22:22:42 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void safe_print(char *s, t_coder *c)
{
	long long	t;

	t = ft_time() - c->init_time;
	pthread_mutex_lock(&c->shared->p_safe);
	printf("%lld %d %s\n", t, c->id, s);
	pthread_mutex_unlock(&c->shared->p_safe);
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
