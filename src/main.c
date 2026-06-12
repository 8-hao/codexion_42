#include <stdio.h>


int main(int argc, char **argv){
    if (argc != 9){
        printf("ht");
        return 1;
    }
    printf("%d", argc);
    return 0;
}