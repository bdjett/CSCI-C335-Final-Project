/***************************
 * queue.c
 *
 * Created by: Brian Jett (bdjett) & Adam Combs (addcombs)
 * Created on: 4 April 2014
 * Part of: Lab 11
 */

#include <queue.h>

void init_queue(queue_t *buf) {
	buf->head = 0;
	buf->tail = 0;
}

int queue_full(queue_t *buf) {
	return (((buf->head + 1) % QUEUE_SIZE) == buf->tail);
}

int enqueue (queue_t *buf, int data) {
	if (queue_full(buf)) {
		return 0;
	} else {
		buf->buffer[buf->head] = data;
		buf->head = ((buf->head + 1) == QUEUE_SIZE) ? 0 : buf->head + 1;
	}
	return 1;
}

int dequeue (queue_t *buf) {
	int data;
	if (queue_empty(buf)) {
		return 0;
	} else {
		data = buf->buffer[buf->tail];
		buf->tail = ((buf->tail + 1) == QUEUE_SIZE) ? 0 : buf->tail + 1;
	}
	return data;
}

int queue_empty(queue_t *buf) {
	return (buf->head == buf->tail);
}


/* queue.c ends here */
