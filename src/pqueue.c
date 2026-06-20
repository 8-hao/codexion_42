#include "../includes/threads.h"

Queue *newNode(int val, int index)
{
    Queue *node;

    node = malloc(sizeof(Queue));
    if (node == NULL)
        return (NULL);

    node-> val = val;
    node-> index = index;
    node-> next = NULL;
    return (node);
}

void add_back(Queue **head, Queue *node)
{
    Queue **second = head;
    if (head == NULL){
        *head = node;
        return;
    }

    while((*second)->next){
        *second =(*second)->next;
    }
    (*second)->next = node;
}


int main()
{
    Queue *head = NULL; 
    Queue *s1 = newNode(1, 10);
    Queue *s2 = newNode(10, 11);
    add_back(&head, s1);
    add_back(&head, s2);
    printf("hillow\n");
    while(head->next){
        printf("%d\n",head->val);
        head = head->next;
    }
    return 0;
}