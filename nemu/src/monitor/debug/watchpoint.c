#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}
WP *new_wp(char *e)
{
	if(head==NULL)
	{
		head=free_;
		free_=free_->next;
		head->next=NULL;
		strcpy(head->expr, e);
		printf("Set watchpoint %d successfully!\n",head->NO+1);
		return head;
	}
	else
	{	
		WP *p;
		p=head;
		while(p->next!=NULL)
			p=p->next;
		if(free_==NULL)
			assert(free_);
		else
		{
			p->next=free_;
			free_=free_->next;
		}
		p=p->next;
		p->next=NULL;
		strcpy(p->expr, e);
		printf("Set watchpoint %d successfully!\n",p->NO+1);
		return p;
	}
}
void free_wp(int num)
{
	WP *p=head,*q=head;
	int flag=0;
	while(1)
	{
		if(p->NO==num-1)
			break;
		else 
			{
				if(p!=head)
					q=q->next;
				p=p->next;
				if(p==NULL)
				{	
					flag=1;
					break;
				}
			}
	}
	if(flag==1)
		printf("No such watchpoint!\n");
	else
	{
		if(p==head)
		{
			if(head->next!=NULL)
				head=head->next;
			else 
				head=NULL;
			p->next=free_;
			free_=p;
		}
		else
		{
			if(p->next!=NULL)
				q->next=p->next;
			else
				q->next=NULL;
			p->next=free_;
			free_=p;
		}
		printf("Delete watchpoint %d successfully\n",num);
	}

}


WP * gethead()
{
	return head;
}

/* TODO: Implement the functionality of watchpoint */


