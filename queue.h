#include <stdio.h>
#include <stdlib.h>
#include "proc.h"

// A linked list (LL) node to store a queue entry
struct QNode {
    struct proc* key;
    struct QNode* next;
};
 
// The queue, front stores the front node of LL and rear
// stores the last node of LL
struct Queue {
    struct QNode *front, *rear;
};
 
// A utility function to create a new linked list node.
struct QNode* newNode(struct proc* p)
{
    struct QNode* temp
        = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = p;
    temp->next = NULL;
    return temp;
}
 
// A utility function to create an empty queue
struct Queue* createQueue()
{
    struct Queue* q
        = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue* q, struct proc* k);
 
// Function to remove a key from given queue q
void deQueue(struct Queue* q);

int isempty(struct Queue* q);
struct QNode* getFirst(struct Queue* q);