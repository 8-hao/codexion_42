#include "../includes/threads.h"

static long long ft_time()
{
    struct timeval d;
    long long time;

    gettimeofday(&d, NULL);
    time = d.tv_sec * 1000 + d.tv_usec / 1000;
    return time;
}

void *monitor_func(void *monitor)
{
    Monitor *m;
    int last_compile_start;
    int i;

    m = (Monitor *) monitor;
    i = 0;
    while(1)
    {
        while(i < m->num_of_coders)
        {   
            pthread_mutex_lock(coders[i]->left_d);
            
            pthread_mutex_unlock(coders[i]->left_d);
            i++;
        }

        usleep(1000);
    }
    return NULL;
}

void    *myfunction(void *coder)
{
    Coder *c;
    int i;

    c = (Coder *) coder;
    i  = 0;
    while (i < c ->num_of_compiles_required)
    {
        pthread_mutex_lock(c->left_d);
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time ,c->id);
        pthread_mutex_lock(c->right_d);
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time , c->id);
        printf("%lld %d is compiling\n", ft_time()-c->init_time ,c->id);
        usleep(c->time_to_compile * 1000);
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->left_d);
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->right_d);
        printf("%lld %d is debugging\n", ft_time()-c->init_time ,c->id);
        usleep(c->time_to_debug * 1000);
        printf("%lld %d is refactoring\n",ft_time()-c->init_time ,c->id);
        usleep(c->time_to_refactor * 1000);
        i++;
        c->compile_count = i;
        c->last_compile_start = ft_time();
    }
    
    return NULL;
}

void ft_threads(int *data)
{
    pthread_t *id_arr;
    pthread_t id_monitor;
    struct timeval g;
    pthread_mutex_t *dongles;
    long long init_time;
    Coder *coders;
    Monitor monitor;
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
        coders[i].time_to_burnout = data[1];
        coders[i].time_to_compile = data[2];
        coders[i].time_to_debug = data[3];
        coders[i].time_to_refactor = data[4];
        coders[i].num_of_compiles_required = data[5];
        coders[i].dongle_cooldown = data[6];
        coders[i].compile_count = 0;
        pthread_mutex_init(&dongles[i], NULL);
        i++;
    }
    i = 0;
    monitor.coders = coders;
    monitor.dongles = dongles;
    monitor.threads = id_arr;
    monitor.num_of_coders = data[0];
    gettimeofday(&g, NULL);
    init_time = g.tv_sec * 1000 + g.tv_usec / 1000;

    while(i < data[0])
    {
        coders[i].init_time = init_time;
        coders[i].last_compile_start = init_time;
        pthread_create(&id_arr[i], NULL, myfunction, &coders[i]);
        i++;
    }
    pthread_create(&id_monitor, NULL, monitor_func, &monitor);
    i = 0;
    while(i < data[0])
    {
        pthread_join(id_arr[i], NULL);
        i++;
    }
}



// Step 3 — Add the monitor thread
// Add a dedicated thread that loops every ~1ms, checks if any 
// coder exceeded time_to_burnout, prints the burnout message and sets the stop flag. 
// Also check if all coders reached number_of_compiles_required. Every coder thread 
// must check this stop flag at the start of each cycle.

// Step 4 — Add dongle cooldown
// Once basic flow works, add cooldown: when a dongle is released, 
// record its release timestamp. When a coder tries to acquire it, 
// if cooldown hasn't elapsed, the thread waits. Use pthread_cond_timedwait 
// for this rather than busy-waiting.