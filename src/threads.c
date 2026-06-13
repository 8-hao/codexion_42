#include "../includes/threads.h"

void    *myfunction(void *coder)
{
    Coder *c;

    c = (Coder *) coder;
    printf("i'm coder %d\n", c->id);
    pthread_mutex_lock(c->left_d);
    printf("i took left dongle %d\n", c->id);
    sleep(4);
    pthread_mutex_lock(c->right_d);
    printf("i took left dongle %d\n", c->id);
    pthread_mutex_unlock(c->left_d);
    pthread_mutex_unlock(c->right_d);
    return NULL;
}

void ft_threads(int *data)
{
    pthread_t *id_arr;
    pthread_mutex_t *dongles;
    Coder *coders;
    int i;

    i = 0;
    id_arr = malloc(sizeof(pthread_t) * data[0]);
    dongles = malloc(sizeof(pthread_mutex_t) * data[0]);
    coders = malloc(sizeof(Coder)* data[0]);
    if (id_arr == NULL || dongles == NULL)
        return;
    while(i < data[0])
    {
        Coder s;
        coders[i] = s;
        coders[i].id = i+1;
        coders[i].left_d = &dongles[i];
        coders[i].right_d = &dongles[(i+1) % data[0]];
        pthread_mutex_init(&dongles[i], NULL);
        i++;
    }
    i = 0;
    while(i < data[0])
    {
        pthread_create(&id_arr[i], NULL, myfunction, &coders[i]);
        i++;
    }
    i = 0;
    while(i < data[0])
    {
        pthread_join(id_arr[i], NULL);
        i++;
    }

}