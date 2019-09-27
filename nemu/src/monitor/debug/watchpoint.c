#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include<stdlib.h>
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
WP* new_wp()
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
	return temp;
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
void info_w()
{
	WP* temp=head;
  printf("Num\tAddress\tExprssion\tValue\n");
	while(temp!=NULL)
	{
    printf("%d\t%u\t%s\t\t%u\n", temp->NO, temp->vaddr, temp->buf, temp->seq_val);
		temp=temp->next;
	}
}
void wp_d(int Number)
{
	WP* temp=head;
	while(temp!=NULL&&temp->NO!=Number)temp=temp->next;
	if(temp!=NULL)free_wp(temp);
}
void wp_d_all()
{
	while(head!=NULL)
	{
		free_wp(head);
	}
}
void wp_new(char *args, uint32_t val)
{
	WP* temp=new_wp();
	temp->seq_val=val;
	temp->ori_val=val;
	temp->vaddr=0;
	temp->buf[0]='\0';
	strcat(temp->buf,args);
}
bool wp_check()
{
	bool Flag=false;
	bool *Success=malloc(sizeof(bool));
  WP* temp=head;
	while(temp!=NULL)
	{
    temp->ori_val=temp->seq_val;
		temp->seq_val=expr(temp->buf, Success);
		if(temp->ori_val!=temp->seq_val)
		{
		  printf("Watchpoint %d: %s\nOld value: %u\nNew value: %u\n", temp->NO, temp->buf, temp->ori_val, temp->seq_val);
			Flag=true;
		}
		temp=temp->next;
	}
	return Flag;
}
