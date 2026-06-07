#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


void *greeting(void *args){
    int *x = (int *) args;
    for (int i=0; i<2;i++){
        (*x)++;
        printf("%d\n", *(int *)args);
        sleep(2);
    }
    return args;
}

int main(){
    pthread_t mythread;
    int x = 0;

    int val = pthread_create(&mythread, NULL, greeting, &x);
    
    printf("finale: %d\n", x);
    pthread_join(mythread, NULL);

   

    return 0;
}