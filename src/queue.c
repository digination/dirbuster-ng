#include "queue.h"

struct queue *queue_add(struct queue *s, char* str)
{
    struct elt *p = malloc(1 * sizeof(*p));

    if (NULL == p) {
	fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__,
		"list_add");
	return s;
    }

	p->entry  = (char*) calloc (0,(strlen(str) +1) * sizeof(char));
	strncpy(p->entry,str,strlen(str));
    p->next = NULL;

    if (NULL == s) {
	printf("Queue not initialized\n");
	free(p);
	return s;
    } else if (NULL == s->head && NULL == s->tail) {
	s->head = s->tail = p;
	return s;
    } else if (NULL == s->head || NULL == s->tail) {
	fprintf(stderr,
		"There is something wrong with your assignment of head/tail\n");
	free(p);
	return NULL;
    } else {
	  s->tail->next = p;
	  s->tail = p;
    }
	
    return s;
}

struct queue *queue_rem(struct queue *s)
{
    struct elt *h = NULL;
    struct elt *p = NULL;

    if (NULL == s) {
	printf("List is empty\n");
	return s;
    } else if (NULL == s->head && NULL == s->tail) {
	printf("Well, List is empty\n");
	return s;
    } else if (NULL == s->head || NULL == s->tail) {
	printf("There is something seriously wrong with your list\n");
	printf("One of the head/tail is empty while other is not \n");
	return s;
    }

    h = s->head;
    p = h->next;
	free(h->entry);
    free(h);
    s->head = p;
    if (NULL == s->head)
	s->tail = s->head;
    return s;
}

struct queue *queue_free(struct queue *s)
{
    while (s->head) {
	queue_rem(s);
    }
    return s;
}

struct queue *queue_new(void)
{
    struct queue *p = malloc(1 * sizeof(*p));

    if (NULL == p) {
	fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
    }
    p->head = p->tail = NULL;
    return p;
}