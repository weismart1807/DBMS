#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define N 10
struct info
{
    char key[2*N];
    char value[N];
};
typedef struct info info;
struct node
{
    info msg;
    struct node *next;
};
typedef struct node NODE;

void createList(NODE *,char* ,char* );
void printList(NODE *);
void freeList(NODE *);
int genRandomString(int , char* );

void createList(NODE* head,char* a,char* b){
    NODE* new = (NODE*)malloc(sizeof(NODE));
    setbuf(stdin,NULL);
    strcpy(new->msg.key,a);
    strcpy(new->msg.value,b);
    NODE* visit = head;
    while(visit->next != NULL){
        visit = visit->next;
    }
    visit -> next = new;
}


void printList(NODE * head){
     NODE *node = head;
     node = node->next;//no print 0,0
     if (head == NULL)
         printf( "List is empty.\n" );
     else
     {
        while (node != NULL)
        {
            //printf( "%s\t%s\n" , node->msg.key, node->msg.value);
            node = node-> next;
        }
     }
     //printf(" \n" );
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

int genRandomString(int length, char* ouput)
{
	int flag, i;
	srand((unsigned)time(NULL));
	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
		case 0:
			ouput[i] = 'A' + rand() % 26;
			break;
		case 1:
			ouput[i] = 'a' + rand() % 26;
			break;
		case 2:
			ouput[i] = '0' + rand() % 10;
			break;
		default:
			ouput[i] = 'x';
			break;
		}
	}
	ouput[length-1]='\0';
	return 0;
}

#endif // LINKLIST_H_INCLUDED
