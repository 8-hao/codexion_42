#ifndef THREADS_H
#define THREADS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct coder{
    int id;
    pthread_mutex_t *left_d;
    pthread_mutex_t *right_d;

}Coder;

int *parser(int argc, char **argv);
void ft_threads(int *data);


#endif