/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/27 10:24:35 by username         #+#    #+#              */
/*   Updated: 2026/06/27 10:24:35 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
