#include<stdio.h>
#include<stdlib.h>
#include "hw3.h"

int main(){
	NODE *first=NULL,*node;
    printf("Please SET first one\n");

	while(1){
	setbuf(stdin,NULL);
	char input[10];
    printf("Press new bottom:\n");
    scanf("%s",input);
	getchar();//read the "enter"
    if(strcmp(input,"LPUSH")==0){
        LPUSH(&first);
    }
    else if(strcmp(input,"RPUSH")==0){
        RPUSH(&first);
    }
    else if(strcmp(input,"LPOP")==0){
        LPOP(&first);
    }
    else if(strcmp(input,"RPOP")==0){
        RPOP(first);
    }
    else if(strcmp(input,"LLEN")==0){
        int nodenumber = LLEN(first);
        printf("%d",nodenumber);
    }
    else if(strcmp(input,"LRANGE")==0){
        printList(&first);
    }

	//delete
	else if(strcmp(input,"DELETE")==0){
		char k[20];
		printf("which key you want to delete:\n");
		scanf("%s",k);
		node=searchNode(first,k);
		if(node!=NULL){
			first=deleteNode(first,node);
		}
		else printf("No the key!\n\n");
	}

	else if(strcmp(input,"SET")==0){
		RPUSH(&first);
	}

	//print
	else if(strcmp(input,"GET")==0){
		char k[20];
		printf("which key you want to print:\n");
		scanf("%s",k);
		node=searchNode(first,k);
		printf( "'key'\t'value'\n");
		printf( "%s\t%s\n" , node->msg.key, node->msg.value);
	}

	//insert
	else if(strcmp(input,"INSERT")==0){
		char ip[20];
		printf("which key you want to follow:");
		scanf("%s",ip);
		node=searchNode(first,ip);
		if(node!=NULL){
			insertNode(node);
		}
		else printf("No the key!\n\n");
	}

	//upgrade
	else if(strcmp(input,"UPGRADE")==0){
		char ip[20];
		printf("Which key you want to rewrite:\n");
		scanf("%s",ip);
		node=searchNode(first,ip);
		rewriteNode(node);
	}

	//free
	else if(strcmp(input,"FREE")==0){
		freeList(first);
		printf("END\n");
		break;
	}

	//bottom meaning
	else if(strcmp(input,"BOTTOM")==0){
		printf("SET,GET,LPUSH,RPUSH,LPOP,RPOP,LLEN,LRANGE,DELETE,INSERT,UPGRADE,FREE \n");
	}
	setbuf(stdin,NULL);
	}

	return 0;
}
