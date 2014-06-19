#pragma once

/**********************************
 * queue.h
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Createdy on: 4 April 2014
 * Part of: Lab 11
 */

#define QUEUE_SIZE 32

typedef struct queue {
  int head;
  int tail;
  int buffer[QUEUE_SIZE];
} queue_t;

void init_queue(queue_t *);
int enqueue(queue_t *, int);
int dequeue(queue_t *);            
int queue_empty(queue_t *);            

/* queue.h ends here */
