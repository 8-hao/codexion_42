#include "../includes/threads.h"

Queue *newNode(Coder *c, int index)
{
    Queue *node;

    node = malloc(sizeof(Queue));
    if (node == NULL)
        return (NULL);

    node-> c = c;
    node-> index = index;
    node-> next = NULL;
    return (node);
}

void add_back(Queue **head, Queue *node)
{
    Queue *second = *head;
    if (*head == NULL){
        *head = node;
        return;
    }

    while(second->next){
        second =second->next;
    }
    second->next = node;
}

int queuelen(Queue *head)
{
    int i;

    i = 0;
    while(head)
    {
        i++;
        head = head->next;
    }
    return i;
}

void sort_min(Queue **head)
{
    int i;
    int ql;
    Queue *helper;
    Queue *n;
    Coder *c;
    int tin;

    i = 0;
    ql = queuelen(*head);
    while(i < ql)
    {
        helper = *head;
        while(helper && helper->next)
        {
            if (helper->index > (helper->next)->index)
            {
                c = helper-> c;
                tin = helper-> index;
                helper->index = (helper->next)->index;
                (helper->next)->index = tin;
                helper->c = (helper->next)->c;
                (helper->next)->c = c;

            }
            helper= helper->next;
        }
        i++;
    }
}

int is_inqueue(Queue *head,Coder *c)
{
    while(head)
    {
        if(c->id == head->c->id)
            return 0;
        head = head->next;
    }   
    return 1;
}


void ft_print(Queue *head)
{
    while(head){
        printf("%d\n",head->c->id);
        head = head->next;
    }
    printf("end\n");
}

Queue *deleteFirst(Queue **head)
{
    Queue *first_node;
    if (*head == NULL)
        return NULL;
    first_node = *head;

    *head = (*head)->next;
    first_node->next = NULL;
    return (first_node);
}