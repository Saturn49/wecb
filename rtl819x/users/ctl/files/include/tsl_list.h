
#ifndef TSL_LIST_H
#define TSL_LIST_H

typedef struct tsl_list_head{
		struct tsl_list_head *prev;
		struct tsl_list_head *next;
}tsl_list_head_t;


static inline void TSL_INIT_LIST_HEAD(tsl_list_head_t *ptr)
{
	ptr->prev = ptr;
	ptr->next = ptr;
}


static inline void tsl_list_add(struct tsl_list_head *new, struct tsl_list_head *head)
{
	(head->next)->prev = new;
	new->next = (head->next);
	new->prev = head;
	head->next = new;
}

static inline void tsl_list_addp(struct tsl_list_head *new, struct tsl_list_head *head)
{
        new->prev = head->prev;
        new->next = head->prev->next;
        head->prev = new;
        
}


static inline void tsl_list_del(struct tsl_list_head *entry)
{
	(entry->next)->prev = entry->prev;
	(entry->prev)->next = entry->next;

	entry->next = entry->prev = entry;
}

static inline int tsl_is_list_empty(struct tsl_list_head *head)
{
	return head->next == head;
	
}

static inline void tsl_list_combine(struct tsl_list_head *list,  struct tsl_list_head *head )
{
	struct tsl_list_head *first = list->next;
	
	if (first != list){
		struct tsl_list_head *last = list->prev;
		struct tsl_list_head *at = head->next;
		
		first->prev = head;
		head->next = first;
		
		last->next = at;
		at->prev = last;
	}
}

#define tsl_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)


#define tsl_list_entry(ptr, type, member) ((type *)((unsigned char *)(ptr)-(unsigned char *)(&((type *)0)->member)))

#endif

