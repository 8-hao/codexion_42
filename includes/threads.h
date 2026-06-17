#ifndef THREADS_H
#define THREADS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


typedef struct p{
    long long init_time;
}Program;

typedef struct coder{
    int id;

    int num_of_compiles_required;
    int time_to_compile;
    int dongle_cooldown;
    int time_to_debug;
    int time_to_refactor;
    int time_to_burnout;

    pthread_mutex_t *left_d;
    pthread_mutex_t *right_d;

    int compile_count;
    long long last_compile_start;
    long long init_time;

}Coder;

typedef struct monitor{
    int num_of_coders;
    Coder *coders;
    pthread_mutex_t *dongles;
    pthread_t *threads;


}Monitor;

int *parser(int argc, char **argv);
void ft_threads(int *data);


#endif