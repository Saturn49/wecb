/*
linux 内核使用的链表结构，构造相当巧妙。

*/

#ifndef MC_LIST_H
#define MC_LIST_H

typedef struct mc_list_head{
		struct mc_list_head *prev;	//链表头指针
		struct mc_list_head *next; //链表尾指针
}mc_list_head_t;


/*链表头*/	
static inline void MC_INIT_LIST_HEAD(mc_list_head_t *ptr)
{
	ptr->prev = ptr;
	ptr->next = ptr;
}
/*加入链表元素*/

static inline void mc_list_add(struct mc_list_head *new, struct mc_list_head *head)
{
	(head->next)->prev = new;
	new->next = (head->next);
	new->prev = head;
	head->next = new;
}

static inline void mc_list_del(struct mc_list_head *entry)
{
	(entry->next)->prev = entry->prev;
	(entry->prev)->next = entry->next;

	entry->next = entry->prev = entry;
}

static inline int mc_is_list_empty(struct mc_list_head *head)
{
	return head->next == head;
	
}

/*
把list 链表加入到head链表中去，即组合两个链表。

*/
static inline void mc_list_combine(struct mc_list_head *list,  struct mc_list_head *head )
{
	struct mc_list_head *first = list->next;
	
	if (first != list){
		struct mc_list_head *last = list->prev;
		struct mc_list_head *at = head->next;
		
		first->prev = head;
		head->next = first;
		
		last->next = at;
		at->prev = last;
	}
}


/*
链表的精华所在，链表可以由不同的结构体组成，极大的提高其灵活性，其构思相当巧妙，简单说明一下：一个结构里mc_list_head的指针减去其mc_list_head结构在其结构中的偏移位，即可得知其结构的指针。
*/

#define mc_list_entry(ptr, type, member) ((type *)((unsigned char *)(ptr)-(unsigned char *)(&((type *)0)->member)))




 
#endif

