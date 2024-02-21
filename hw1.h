#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct info
{
    char data[20];
    int number;
    char name[20];
};
typedef struct info info;
struct node
{
    info msg;
    struct node *next;
};
typedef struct node NODE;

NODE* createList(NODE*);
void printList(NODE *);
void freeList(NODE *);
void insertNode(NODE *);
void rewriteNode(NODE *);
NODE *searchNode(NODE *,char* );
NODE *deleteNode(NODE *,NODE *);


NODE* createList(NODE* head){
    NODE* new = (NODE*)malloc(sizeof(NODE));
    setbuf(stdin,NULL);
    printf("key id name\n");
    scanf("%s %d %s",new->msg.data, &new->msg.number, new->msg.name);
    NODE* visit = head;
    while(visit->next != NULL){
        visit = visit->next;
    }
    visit -> next = new;
    return head;
}

NODE* deleteNode(NODE *first, NODE *point){
     NODE *ptr=first;
     if(first==NULL){
        printf("Nothing to delete!\n");
        return NULL;
     }
     
     else{
        while(ptr->next!=point)
            ptr=ptr->next;
        ptr->next=point->next;
     }
     free(point);
     return first;
}

void insertNode(NODE *node){
     NODE *newnode;
     newnode = (NODE*) malloc (sizeof(NODE));
     printf("key id name\n");
     scanf("%s %d %s",newnode->msg.data, &newnode->msg.number, newnode->msg.name);
     newnode->next = node->next;
     node->next=newnode;
}

void rewriteNode(NODE *node){
    printf("id name\n");
    scanf("%d %s",&node->msg.number, node->msg.name);
}

void printList(NODE * head){
     NODE *node = head;
     node = node->next;//no print 0,0
     if (head == NULL )
         printf( "List is empty.\n" );
     else
     {
        printf( "key\tid\tname\n");
         while (node != NULL)
        {
            printf( "%s\t%d\t%s\n" , node->msg.data, node->msg.number, node->msg.name);
            node = node-> next;
        }
     }
     printf(" \n" );
}

void freeList(NODE * first){
     NODE *current, *tmp;
     current = first;
     while (current)
     {
         tmp = current;
         current = current-> next;
         free (tmp);
     }
}

NODE *searchNode(NODE *first, char *item){
     NODE *node = first;
     while (node!=NULL)
     {
         if (strcmp(node->msg.data,item)==0)
             return node;
         else
             node = node-> next;
     }
     return NULL;
}

#endif // LINKLIST_H_INCLUDED
