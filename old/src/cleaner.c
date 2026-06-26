/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   cleaner.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/23 21:29:26 by username         #+#    #+#              */
/*   Updated: 2026/06/24 16:17:55 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

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

void free_all(t_dongle	*dongles, t_coder *coders)
{
	if (coders != NULL)
	{
		free(coders[0].check_time);
		free(coders);
	}
	if (dongles != NULL)
		free(dongles);
}