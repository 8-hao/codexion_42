#ifndef THREADS_H
#define THREADS_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


typedef struct dongle{
    pthread_mutex_t mutex_v;
    long long release_time;
    int cooldown;
    int is_available;
    pthread_cond_t cond_v;

}Dongle;

typedef struct coder{
    int id;
    int num_of_compiles_required;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int time_to_burnout;

    Dongle *left_d;
    Dongle *right_d;

    pthread_mutex_t *check_time;

    int compile_count;
    long long last_compile_start;
    long long init_time;

    int finish;
    int stop;

}Coder;

typedef struct monitor{
    int num_of_coders;
    Coder *coders;
    Dongle *dongles;
    pthread_t *threads;
}Monitor;

int *parser(int argc, char **argv);
void ft_threads(int *data);
//int ft_smartsleep(int time_to_sleep, Coder *c);


#endif