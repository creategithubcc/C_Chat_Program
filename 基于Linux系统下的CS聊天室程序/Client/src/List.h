#ifndef LIST_H_
#define LIST_H_
#include <assert.h>
#include <stdlib.h>
#define List_Init(list, list_node_t) {list=(list_node_t*)malloc(sizeof(list_node_t));(list)->next=NULL;}
#define List_Free(list, list_node_t) {assert(NULL!=list);list_node_t *tmpPtr;while(NULL!=(tmpPtr=(list)->next)){(list)->next=tmpPtr->next;free(tmpPtr);}}
#define List_Destroy(list, list_node_t) {assert(NULL!=list);List_Free(list, list_node_t)free(list);	(list)=NULL;}
#define List_For_Each(list, cur_Pos)for (   cur_Pos = (list)->next;cur_Pos != NULL;cur_Pos=cur_Pos->next)
#define List_AddHead(list, newNode) {(newNode)->next=(list)->next;(list)->next=newNode;}
#define List_AddTail(list, cur_Pos ,newNode) {for(cur_Pos = (list); ;cur_Pos=cur_Pos->next){if((cur_Pos)->next == NULL){(cur_Pos)->next = (newNode);break;}}}
#define List_InsertBefore(node, newNode) {(newNode)->prev=(node)->prev;(node)->prev->next=newNode;(node)->prev=newNode;(newNode)->next=node;}
#define List_InsertAfter(node, newNode) {(newNode)->next=node->next;(node)->next=newNode;}
#define List_IsEmpty(list)  (((list) != NULL)
#define List_FreeNode(head ,node,list_node_t) {assert(NULL!=node);if(node -> next != NULL){list_node_t *temp = node->next;memcpy(node,temp,sizeof(list_node_t));free(temp); }else{list_node_t *temp = head;while(temp -> next != node) temp = temp -> next; temp -> next = NULL;free(node);	node = NULL; }}
#endif