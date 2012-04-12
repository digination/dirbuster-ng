#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include <pthread.h>

struct elt {
    char* entry;
    struct elt *next;
};

struct queue {
    struct elt *head;
    struct elt *tail;
	pthread_mutex_t* mutex;
};

struct queue *queue_add(struct queue *,char*);
struct queue *queue_rem(struct queue *);
struct queue *queue_new(void);
struct queue *queue_free(struct queue *);