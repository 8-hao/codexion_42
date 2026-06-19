#include "../includes/threads.h"

static long long ft_time()
{
    struct timeval d;
    long long time;

    gettimeofday(&d, NULL);
    time = d.tv_sec * 1000 + d.tv_usec / 1000;
    return time;
}

void ft_time_to_sleep(struct timespec *t, int delay_ms)
{
    struct timeval d;

    gettimeofday(&d, NULL);
    t.
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


void *myfunction(void *coders)
{
    Coder *c;
    int i;
    int r;

    c = (Coder *) coders;
    i = 0;
    while(i++ < c->num_of_compiles_required)
    {
        pthread_mutex_lock(&c->left_d->mutex_v);

        while(c->left_d->is_available != 0 && ft_time() - c->left_d->release_time < c->left_d->cooldown)
        {
            pthread_cond_timedwait(&c->left_d->cond_v, &c->left_d->mutex_v, );
        }
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time ,c->id);

        pthread_mutex_lock(&c->right_d->mutex_v);
        printf("%lld %d has taken a dongle\n", ft_time()-c->init_time , c->id);

        printf("%lld %d is compiling\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_compile, c);

        c->left_d->is_available = 1;
        c->left_d->release_time = ft_time();
        pthread_mutex_unlock(&c->left_d->mutex_v);

        c->right_d->is_available = 1;
        c->right_d->release_time = ft_time();
        pthread_mutex_unlock(&c->right_d->mutex_v);

        printf("%lld %d is debugging\n", ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_debug, c);

        printf("%lld %d is refactoring\n",ft_time()-c->init_time ,c->id);
        r = ft_smartsleep(c->time_to_refactor, c);
        c->compile_count = i;
    }
    c->finish = 1;
    return NULL;
}


void ft_threads(int *data)
{
    Dongle *dongles;
    Coder *coders;
    pthread_t *id_threads;
    int i;

    i = 0;
    coders = malloc(sizeof(Coder)* data[0]);
    dongles = malloc(sizeof(Dongle) * data[0]);
    id_threads = malloc(sizeof(pthread_t) * data[0]);

    if (dongles == NULL || coders == NULL || id_threads == NULL)
        return;

    // dongle creation
    while(i < data[0])
    {
        pthread_mutex_init(&dongles[i].mutex_v, NULL);
        pthread_cond_init(&dongles[i].cond_v, NULL);
        dongles[i].cooldown = data[6]*1000;
        dongles[i].is_available = 1;
        dongles[i++].release_time = ft_time();
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
        coders[i].finish = 0;
        coders[i].stop = 0;
        i++;
    }

    i = 0;

    while(i < data[0])
    {
        coders[i].init_time = ft_time();
        coders[i].last_compile_start = ft_time();
        pthread_create(&id_threads[i], NULL, myfunction, &coders[i++]);
    }

    i = 0;

    while(i < data[0])
        pthread_join(id_threads[i++], NULL);
}