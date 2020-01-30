/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */ 

/* Represents a node in a queue */
typedef struct node
{
    void *item; // data contained in node
    NODE *next; // next item in the queue
} NODE;

/* Represents a queue */
typedef struct queue
{
    NODE *head; // head of queue
    NODE *tail; // tail of queue
    int size;   // size of queue
} QUEUE;

/*
 * Creates an empty queue.
 *
 * Input:
 * None
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int queueCreate(void);

/*
 * Adds and item to the back of the queue.
 *
 * Input:
 * Queue *: pointer to a QUEUE
 *
 * Output:
 * 0 on success, -1 on failure
 */
int enqueue(QUEUE *)

/*
 * Removes an item from the front of the queue.
 *
 * Input:
 * QUEUE *: pointer to a QUEUE
 *
 * Output:
 * 0 on success, -1 on failure
 */
int dequeue(QUEUE *);

/*
 * Returns the items at the front of the queue.
 *
 * Input:
 * QUEUE *: pointer to a QUEUE
 *
 * Output:
 * A pointer to the item at the front of the queue,
 * NULL pointer if the queue is empty.
 */ 
void *peek(QUEUE *);
