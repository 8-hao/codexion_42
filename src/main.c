/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:00:38 by obakri            #+#    #+#             */
/*   Updated: 2026/06/12 16:06:22 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

int main(int argc, char **argv)
{
    int     i;
    int     *ptr;

    i = 0;
    if (argc != 9)
    {
        printf("Error: Expected 8 arguments, but received %d.\n", argc-1);
        return (1);
    }
    ptr = parser(argc-1, argv);
    if (ptr == NULL)
        return (1);
    while(ptr[i] != -1)
        printf("value: %d\n", ptr[i++]);
    return (0);
}
