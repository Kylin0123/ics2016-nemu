#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP *new_wp(){
	if(head == NULL){
		WP *temp = free_;
		free_ = free_->next;
		head = temp;
		head->next = NULL;
		return head;
	}	
	else if(head != NULL && free_ != NULL){
		WP *temp = free_;
		free_ = free_->next;
		temp->next = head;
		head = temp;
		return head;
	}
	else if(free_ == NULL){
		assert(0);
		return NULL;
	}
	else
		return NULL;
}

void free_wp(WP *wp){
	if(wp == head){
		printf("11111111111\n");
		head = head->next;
		wp->next = free_;
		free_ = wp;
	}
	else if(wp->next == NULL){
		printf("22222222222\n");
		WP *temp = head;
		for(;temp->next->next != NULL; temp = temp->next);
		temp->next = NULL;
		wp->next = free_;
		free_ = wp;
	}
	else{
		printf("33333333333\n");
		WP *temp = head;
		for(;temp->next != wp; temp = temp->next);
		WP *temp2 = temp->next;
		temp->next = wp;
		temp2->next = free_;
		free_ = temp2;
	}
}

WP *getHead(){
	return head;
}
