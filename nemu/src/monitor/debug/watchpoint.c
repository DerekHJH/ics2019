#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp()
{
	if(free_==NULL)
	{
    printf("Too many watchpoints!!\n");
		assert(0);
	}
	WP* temp=free_;
	free_=free_->next;
	temp->next=head;
	head=temp;
	return;
}
void free_wp(WP *wp)
{
  WP *temp=head;
	if(head==wp)
	{
		head=head->next;
		wp->next=free_;
		free_=wp;
	}
	else
	{
    while(temp->next!=NULL&&temp->next!=wp)temp=temp->next;
		if(temp->next==NULL)return;
		else if(temp->next==wp)
		{
			temp->next=wp->next;
      wp->next=free_;
			free_=wp;
		}
	}
	return;
}
