#include <stdio.h>
#include <pthread.h>

int value = 0;

void *my_function(void *args)
{
    printf("i'm thread %d\n",value);
    for(int i=0; i<50000;i++){
        (*((int *) args))++;
        printf("the value of value is %d\n",*(int *)args);
    }
    value+=1;
    return NULL;
}

int main(){
    pthread_t thread;
    pthread_t thread1;
    int a=0;

    pthread_create(&thread,NULL,my_function,&a);
    pthread_create(&thread1,NULL,my_function,&a);

    pthread_join(thread,NULL);
    pthread_join(thread1,NULL);
    return 0;
}