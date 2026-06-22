/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/22 13:06:47 by username         #+#    #+#              */
/*   Updated: 2026/06/22 15:49:52 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"






int	main(int argc, char **argv)
{
	int	i;
	int	*data;

	i = 0;
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
