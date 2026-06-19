#include "../includes/threads.h"

static long long ft_time()
{
    struct timeval d;
    long long time;

    gettimeofday(&d, NULL);
    time = d.tv_sec * 1000 + d.tv_usec / 1000;
    return time;
}

// void ft_time_to_sleep(struct timespec *t, int delay_ms)
// {
//     struct timeval d;

//     gettimeofday(&d, NULL);
//     t->tv_sec = d.tv_sec + (delay_ms / 1000);
//     t->tv_nsec = d.tv_usec*1000 + (delay_ms*1000000);
// }

void ft_time_to_sleep(struct timespec *t, int delay_ms)
{
    struct timeval d;
    long long total_nsec;

    gettimeofday(&d, NULL);
    total_nsec = (long long)d.tv_usec *1000 + (long long)delay_ms * 1000000;
    t->tv_sec = d.tv_sec + (total_nsec / 1000000000);
    t->tv_nsec = total_nsec % 1000000000;
}

int ft_smartsleep(int time_to_sleep, Coder *c)
{
    long long current_time;

    current_time = 0;
    while(current_time < time_to_sleep*1000)
    {
        if (c->stop)
            return 0;
        current_time += 500;
        usleep(500);
    }
    return 1;
}

void *monitor_func(void *monitor)
{
    Monitor *m;
    long long last_compile_start;
    long long time_to_burnout;
    long long current_time;
    int i;
    int counter;
    int finished;

    m = (Monitor *) monitor;
    i = 0;
    counter = 0;
    finished =0;
    while(1)
    {
        while(i < m->num_of_coders)
        {   
            pthread_mutex_lock(m->coders[i].check_time);
            current_time = ft_time();
            last_compile_start = m->coders[i].last_compile_start;
            time_to_burnout = m->coders[i].time_to_burnout;
            pthread_mutex_unlock(m->coders[i].check_time);
            if (current_time - last_compile_start >= time_to_burnout &&  m->coders[i].finish == 0)
            {
                printf("%lld %d is burned out\n",(current_time - m->coders[i].init_time), m->coders[i].id);
                i = 0;
                while(i < m->num_of_coders)
                {
                    m->coders[i].stop = 1;
                    i++;
                }
                return NULL;
            }
            if (m->coders[i].finish == 1){
                finished++;
                if(m->coders[i].compile_count == m->coders[i].num_of_compiles_required)
                    counter++;
            }
            i++;
        }
        if (finished == m->num_of_coders)
            if (finished == counter)
                return NULL;
        i = 0;
        finished = 0;
        counter = 0;
        usleep(1000);
    }
    return NULL;
}

void *myfunction(void *coders)
{
    Coder *c;
    int i;
    int r;
    struct timespec l_dongle;
    struct timespec r_dongle;

    c = (Coder *) coders;
    i = 0;
    while(i++ < c->num_of_compiles_required)
    {

        if (c->stop)
            return NULL;

        pthread_mutex_lock(&c->left_d->mutex_v);

        if (c->stop){
            pthread_mutex_unlock(c->left_d);
            return NULL;
        }
        while(c->left_d->is_available == 0 && ft_time() - c->left_d->release_time < c->left_d->cooldown)
        {
            ft_time_to_sleep(&l_dongle, c->left_d->cooldown - (ft_time() - c->left_d->release_time));
            pthread_cond_timedwait(&c->left_d->cond_v, &c->left_d->mutex_v, &l_dongle);
        }
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time ,c->id);

        pthread_mutex_lock(&c->right_d->mutex_v);
        if (c->stop){
            pthread_mutex_unlock(c->left_d);
            pthread_mutex_unlock(c->right_d);
            return NULL;
        }
        while(c->right_d->is_available == 0 && ft_time() - c->right_d->release_time < c->right_d->cooldown)
        {
    
            ft_time_to_sleep(&r_dongle, c->left_d->cooldown -(ft_time() - c->right_d->release_time));
            pthread_cond_timedwait(&c->right_d->cond_v, &c->right_d->mutex_v, &r_dongle);
        }
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time , c->id);

        printf("%lld %d is compiling\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_compile, c);
        if (r == 0){
            pthread_mutex_unlock(c->left_d);
            pthread_mutex_unlock(c->right_d);
            return NULL;
        }
        pthread_mutex_lock(c->check_time);
        c->last_compile_start = ft_time();
        pthread_mutex_unlock(c->check_time);
        c->left_d->is_available = 1;
        c->left_d->release_time = ft_time();
        pthread_mutex_unlock(&c->left_d->mutex_v);

        c->right_d->is_available = 1;
        c->right_d->release_time = ft_time();
        pthread_mutex_unlock(&c->right_d->mutex_v);

        printf("%lld %d is debugging\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_debug, c);
        if (r == 0)
            return NULL;
        printf("%lld %d is refactoring\n",ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_refactor, c);
        if (r == 0)
            return NULL;
        c->compile_count = i;
    }
    c->finish = 1;
    return NULL;
}


void ft_threads(int *data)
{
    Dongle *dongles;
    Coder *coders;
    Monitor monitor;
    long long init_time;

    pthread_t *id_threads;
    pthread_mutex_t *check_time;

    pthread_t id_monitor;
    int i;

    i = 0;
    coders = malloc(sizeof(Coder)* data[0]);
    dongles = malloc(sizeof(Dongle) * data[0]);
    id_threads = malloc(sizeof(pthread_t) * data[0]);
    check_time = malloc(sizeof(pthread_mutex_t) * data[0]);

    if (dongles == NULL || coders == NULL || id_threads == NULL)
        return;

    // dongle creation
    while(i < data[0])
    {
        pthread_mutex_init(&dongles[i].mutex_v, NULL);
        pthread_cond_init(&dongles[i].cond_v, NULL);
        dongles[i].cooldown = data[6]*1000;
        dongles[i].is_available = 1;
        i++;
    }
    i = 0;
    //coders creation
    while(i < data[0])
    {
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
        coders[i].compile_count = 0;
        coders[i].check_time = &check_time[i];
        coders[i].finish = 0;
        coders[i].stop = 0;
        pthread_mutex_init(&check_time[i], NULL);
        i++;
    }

    i = 0;
    monitor.coders = coders;
    monitor.dongles = dongles;
    monitor.threads = id_threads;
    monitor.num_of_coders = data[0];
    init_time = ft_time();
    while(i < data[0])
    {
        coders[i].init_time = init_time;
        coders[i].last_compile_start = ft_time();
        dongles[i].release_time = ft_time();
        pthread_create(&id_threads[i], NULL, myfunction, &coders[i++]);
    }
    pthread_create(&id_monitor, NULL, monitor_func, &monitor);
    i = 0;

    while(i < data[0])
        pthread_join(id_threads[i++], NULL);
    pthread_join(id_monitor, NULL);
}