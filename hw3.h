#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct info
{
    char key[20];
    char value[20];
};
typedef struct info info;
struct node
{
    info msg;
    struct node *next;
    struct node *prev;
};
typedef struct node NODE;

void printList(NODE **);
void freeList(NODE *);
void insertNode(NODE *);
void rewriteNode(NODE *);
NODE *searchNode(NODE *,char* );
NODE *deleteNode(NODE *,NODE *);
void kv(NODE *);
void LPUSH(NODE **);
void RPUSH(NODE **);
void LPOP(NODE **);
void RPOP(NODE *);
int LLEN(NODE *);

void kv(NODE *newnode){
    printf("key value\n");
    scanf("%s %s",newnode -> msg.key, newnode -> msg.value);
}

void LPUSH(NODE **first){
    NODE* current = *first;
    setbuf(stdin,NULL);
    NODE *newNode = (NODE*)malloc(sizeof(NODE));
    kv(newNode);

    if (*first == NULL) {
        *first = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    } else {
        newNode->next = current;
        newNode->prev = NULL;
        (*first)->prev = newNode;
        *first = newNode;
    }
}

void RPUSH(NODE **first){
    NODE *newNode = (NODE*)malloc(sizeof(NODE));
    kv(newNode);

    if (*first == NULL) {
        *first = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
    } else {
        NODE* current = *first;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}

void LPOP(NODE **first){
    if (*first == NULL) {
        printf("NO LIST\n");
    } else {
        NODE* current = *first;
        *first = current->next;
        if(*first != NULL){
            (*first)->prev = NULL;
        }
        free(current);
    }
}

void RPOP(NODE *first){
    if (first == NULL) {
        printf("NO LIST\n");
    } else {
        NODE* current = first;
        NODE* previous;
        while (current->next != NULL) {
            previous = current;
            current = current->next;
        }
        previous->next = NULL;
        free(current);
    }
}

void printList(NODE **first){
     NODE *node = *first;
    if (*first == NULL )
        printf("List is empty.\n" );

    printf( "'key'\t'value'\n");
    while (node != NULL){
        printf("%s\t%s\n" , node->msg.key, node->msg.value);
        node = node-> next;
    }
    printf(" \n" );
}

int LLEN(NODE *first){
    NODE* current = first;
    int sum = 0;
    if(current->next == NULL)
        return sum;
    else{
        while (current->next != NULL) {
            current = current->next;
            sum++;
        }
        sum++;
        return sum;
    }
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
     newnode = (NODE*) malloc ( sizeof (NODE));
     printf("'key'\t'value'\n");
     scanf("%s %s",newnode->msg.key, newnode->msg.value);
     newnode->next = node->next;
     node->next=newnode;
}

void rewriteNode(NODE *node){
    printf("'value'\n");
    scanf("%s",node->msg.value);
}

void freeList(NODE *first){
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
         if (strcmp(node->msg.key,item)==0)
             return node;
         else
             node = node-> next;
     }
     return NULL;
}

#endif // LINKLIST_H_INCLUDED
