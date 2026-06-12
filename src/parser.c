/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obakri <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 21:53:11 by obakri            #+#    #+#             */
/*   Updated: 2026/06/12 21:55:37 by obakri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/threads.h"

static char *to_lower(char *str)
{
    int	i;

    i = 0;
    while(str[i])
    {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        i++;
    }
    return (str);
}

static int ft_isdigit(char *num)
{
    int		i;
    long 	number;

    i = 0;
    while(num[i])
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

static int ft_scheduler(char *value)
{
    if (strlen(value)!= 3 && strlen(value)!= 4)
    {
        printf("The scheduler value must be exactly one of: fifo or edf.\n");
        return (0);
    }
    value = to_lower(value);
    if(strcmp(value, "edf") == 0)
        return (2);
    if(strcmp(value, "fifo") == 0){
        return (1);
    }
    printf("The scheduler value must be exactly one of: fifo or edf.\n");
    return (0);
}

int *parser(int argc, char **argv)
{
    int		i;
    int		*data;
    int		a;

    i = 1;
    data = malloc(sizeof(int) * argc);
    while(i< 8)
    {
        if (ft_isdigit(argv[i]) == -1)
            return (NULL);

        a = atoi(argv[i]);
        if (i != 7 && a == 0){
            printf("0 is not a valid integer value (max: 2147483647)\n");
            return (NULL);
        }
        data[i-1] = a;
        i++;
    }
    if (!(a =ft_scheduler(argv[i])))
        return (NULL);
    data[i-1] = a;
    data[i] = -1;
    return (data);
}

