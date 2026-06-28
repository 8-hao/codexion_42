/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <obakri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 22:22:51 by obakri            #+#    #+#             */
/*   Updated: 2026/06/27 22:22:54 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static char	*to_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 65 && str[i] <= 90)
			str[i] += 32;
		i++;
	}
	return (str);
}

static int	ft_isdigit(char *num)
{
	int		i;
	long	number;

	i = 0;
	while (num[i])
	{
		if (num[i] < '0' || num[i] > '9')
		{
			printf("%s is not a valid integer value \n", num);
			return (-1);
		}
		i++;
	}
	if (i > 10)
	{
		printf("%s is not a valid integer value \n", num);
		return (-1);
	}
	number = atoi(num);
	if (number > 2147483647)
	{
		printf("%s is not a valid integer value (max: 2147483647)\n", num);
		return (-1);
	}
	return (1);
}

static int	ft_scheduler(char *value)
{
	if (strlen(value) != 3 && strlen(value) != 4)
	{
		printf("The scheduler value must be exactly one of: fifo or edf.\n");
		return (0);
	}
	value = to_lower(value);
	if (strcmp(value, "edf") == 0)
		return (2);
	if (strcmp(value, "fifo") == 0)
		return (1);
	printf("The scheduler value must be exactly one of: fifo or edf.\n");
	return (0);
}

static t_shared	*shared_init(int *data)
{
	t_shared	*shared;

	shared = malloc(sizeof(t_shared));
	if (shared == NULL)
		return (NULL);
	if (pthread_mutex_init(&shared->p_safe, NULL) != 0)
	{
		free(shared);
		return (NULL);
	}
	shared->n_coders = data[0];
	shared->t_burnout = data[1];
	shared->t_compile = data[2];
	shared->t_debug = data[3];
	shared->t_refactor = data[4];
	shared->n_compiles = data[5];
	shared->d_cooldown = data[6];
	shared->c = data[7];
	free(data);
	return (shared);
}

t_shared	*parser(int argc, char **argv)
{
	int	i;
	int	*data;
	int	a;

	i = 1;
	data = malloc(sizeof(int) * argc);
	if (data == NULL)
		return (NULL);
	while (i < 8)
	{
		if (ft_isdigit(argv[i]) == -1)
			return (free(data), NULL);
		if (i != 7 && atoi(argv[i] == 0))
		{
			printf("0 is not a valid integer value (max: 2147483647)\n");
			return (free(data), NULL);
		}
		data[i - 1] = atoi(argv[i]);
		i++;
	}
	a = ft_scheduler(argv[i]);
	if (!a)
		return (free(data), NULL);
	data[i - 1] = a;
	return (shared_init(data));
}
