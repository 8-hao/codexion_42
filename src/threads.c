#include "../includes/threads.h"

static int ft_time(int initial_time){
    struct timeval d;
    int time;

    gettimeofday(&d, NULL);
    time = d.tv_sec * 1000 + d.tv_usec / 1000;
    printf("%d %d\n", initial_time, time);
    return time - initial_time;


}

void    *myfunction(void *coder)
{
    Coder *c;
    int i;

    c = (Coder *) coder;
    i  = 0;
    while (i < c ->num_of_compiles){
        pthread_mutex_lock(c->left_d);
        printf("%d %d has taken a dongle\n", ft_time(c->program.init_time) ,c->id);
        pthread_mutex_lock(c->right_d);
        printf("%d %d has taken a dongle\n", ft_time(c->program.init_time), c->id);
        printf("%d %d is compiling\n", ft_time(c->program.init_time),c->id);
        usleep(c->time_to_compile * 1000);
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->left_d);
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->right_d);
        printf("%d %d is debugging\n", ft_time(c->program.init_time),c->id);
        usleep(c->time_to_debug * 1000);
        printf("%d %d is refactoring\n", ft_time(c->program.init_time),c->id);
        usleep(c->time_to_refactor * 1000);
        i++;
    }
    
    return NULL;
}

void ft_threads(int *data)
{
    pthread_t *id_arr;
    struct timeval g;
    pthread_mutex_t *dongles;
    Coder *coders;
    Program p;
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
        if (((i+1) % data[0]) < i){
            coders[i].left_d = &dongles[(i+1) % data[0]];
            coders[i].right_d = &dongles[i];
        }else{
            coders[i].left_d = &dongles[i];
            coders[i].right_d = &dongles[(i+1) % data[0]];
        }
        coders[i].time_to_compile = data[2];
        coders[i].time_to_debug = data[3];
        coders[i].time_to_refactor = data[4];
        coders[i].num_of_compiles = data[5];
        coders[i].dongle_cooldown = data[6];
        pthread_mutex_init(&dongles[i], NULL);
        i++;
    }
    i = 0;
    gettimeofday(&g, NULL);
    p.init_time = g.tv_sec * 1000 + g.tv_usec / 1000;
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


// Step 1 — Define your data structures first
// Before writing any logic, nail down your structs. 
// You need a Coder struct (which you have partially), 
// a Dongle struct, and a global simulation State struct. 
// The State holds the shared data: array of coders, array of dongles, 
// a print mutex, a stop flag, and timing info. Get this right before anything 
// else — bad structs cause rewrites.


// Step 2 — Basic coder lifecycle (no scheduling yet)
// Make each coder thread run the simple loop: 
// take left dongle → take right dongle → compile → release both → debug → 
// refactor → repeat. Use simple pthread_mutex_lock for the dongles 
// (no queue, no cooldown yet). Goal: get the state machine working and logs 
// printing correctly. Your myfunction is a sketch of this but it has bugs 
// (prints "left dongle" twice, no compile/debug/refactor logic).