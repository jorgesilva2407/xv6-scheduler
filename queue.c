#include "queue.h"
 
// The function to add a key k to q
void enQueue(struct Queue* q, struct proc* k)
{
    // Create a new LL node
    struct QNode* temp = newNode(k);
 
    // If queue is empty, then new node is front and rear
    // both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
 
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}
 
// Function to remove a key from given queue q
void deQueue(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (isempty(q) == 1)
        return;
 
    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;
 
    q->front = q->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
 
    free(temp);
}

struct QNode* getFirst(struct Queue* q){
    // If queue is empty, return NULL.
    if (isempty(q) == 1)
        return;
    
    return q->front;
}

int isempty(struct Queue* q){
    if (q->front == NULL)
        return 1; 
    return 0;
}