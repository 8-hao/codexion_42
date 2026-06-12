#include "../includes/threads.h"

static int ft_isdigit(char *num){
    int i;
    long number;

    i = 0;
    while(num[i]){
        if (num[i] < '0' || num[i] > '9'){
            printf("%s is not a valid integer value \n", num);
            return (-1);
        }
        i++;
    }
    if (i > 10){
        printf("%s is not a valid integer value \n", num);
        return (-1);
    }
    number = atoi(num);
    if (number > 2147483647){
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
        return 1;
    }

}

int *parser(int argc, char **argv)
{
    int i;
    int *data;
    int a;

    i = 1;
    data = malloc(sizeof(int) * argc);
    while(i<argc-1)
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
    if (ft_scheduler(argv[i])){
        return (NULL);
    }
    data[i-1] = -1;
    return (data);
}