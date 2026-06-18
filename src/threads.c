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
    long long last_compile_start;
    long long time_to_burnout;
    long long current_time;
    int i;

    m = (Monitor *) monitor;
    i = 0;
    while(1)
    {
        while(i < m->num_of_coders)
        {   
            pthread_mutex_lock(m->coders[i].check_time);
            current_time = ft_time();
            last_compile_start = m->coders[i]. last_compile_start;
            time_to_burnout = m->coders[i].time_to_burnout;
            pthread_mutex_unlock(m->coders[i].check_time);
            if (current_time - last_compile_start > time_to_burnout)
            {
                printf("**** ct: %lld, lcs:%lld , ttb: %lld\n", current_time, last_compile_start, time_to_burnout);
                printf("%lld %d is burned out",(current_time - m->coders[i].init_time), m->coders[i].id);
                i = 0;
                while(i < m->num_of_coders)
                {
                    m->coders[i].stop = 1;
                    i++;
                }
                return NULL;
            }
            i++;
        }
        i = 0;
        usleep(1000);
    }
    return NULL;
}

int ft_smartsleep(int time_to_sleep, Coder *c)
{
    long long current_time;

    current_time = 0;
    while(current_time < time_to_sleep*1000)
    {
        if (c->stop)
            return 0;
        c->last_compile_start = ft_time();
        current_time += 100;
        usleep(100);
    }
    return 1;
}

void    *myfunction(void *coder)
{
    Coder *c;
    int i;
    int r;

    c = (Coder *) coder;
    i  = 0;
    while (i < c ->num_of_compiles_required)
    {
        if (c->stop)
            return NULL;
        pthread_mutex_lock(c->left_d);
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time ,c->id);
        pthread_mutex_lock(c->right_d);
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time , c->id);
        printf("%lld %d is compiling\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_compile, c);
        if (r == 0)
            return NULL;
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->left_d);
        usleep(c->dongle_cooldown * 1000);
        pthread_mutex_unlock(c->right_d);
        printf("%lld %d is debugging\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_debug, c);
        if (r == 0)
            return NULL;
        printf("%lld %d is refactoring\n",ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_refactor, c);
        if (r == 0)
            return NULL;
        i++;
        c->compile_count = i;
        c->last_compile_start = ft_time();
    }
    c->finish = 1;
    return NULL;
}

void ft_threads(int *data)
{
    pthread_t *id_arr;
    pthread_t id_monitor;
    struct timeval g;
    pthread_mutex_t *dongles;
    pthread_mutex_t *check_time;
    long long init_time;
    Coder *coders;
    Monitor monitor;
    int i;

    i = 0;
    id_arr = malloc(sizeof(pthread_t) * data[0]);
    dongles = malloc(sizeof(pthread_mutex_t) * data[0]);
    check_time = malloc(sizeof(pthread_mutex_t) * data[0]);
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
        coders[i].check_time = &check_time[i];
        coders[i].finish = 0;
        coders[i].stop = 0;
        pthread_mutex_init(&dongles[i], NULL);
        pthread_mutex_init(&check_time[i], NULL);
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
    pthread_join(id_monitor, NULL);
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