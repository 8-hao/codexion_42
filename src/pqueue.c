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
    int tval;
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
                tval = helper-> val;
                tin = helper-> index;
                helper->index = (helper->next)->index;
                (helper->next)->index = tin;
                helper->val = (helper->next)->val;
                (helper->next)->val = tval;

            }
            helper= helper->next;
        }
        i++;
    }
}

int main()
{
    Queue *head = NULL; 
    Queue *s1 = newNode(1, 1);
    Queue *s2 = newNode(8, 8);
    Queue *s3 = newNode(2, 2);
    Queue *s4 = newNode(4, 4);
    Queue *s5 = newNode(7, 7);
    Queue *s6 = newNode(0, 0);
    add_back(&head, s1);
    add_back(&head, s2);
    add_back(&head, s3);
    add_back(&head, s4);
    add_back(&head, s5);
    add_back(&head, s6);
    printf("^^^^^^^^^^^^\n");
    sort_min(&head);
    while(head){
        printf("%d\n",head->val);
        head = head->next;
    }
    return 0;
}