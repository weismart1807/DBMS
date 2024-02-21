#include<stdio.h>
#include<stdlib.h>
#include "hw1.h"

int main(){
	NODE *first,*node;
	first = (NODE*)malloc(sizeof(NODE));
	printf("~Press 'c' to create  ~\n");
	printf("~Press 'd' to delete  ~\n");
	printf("~Press 'i' to insert  ~\n");
	printf("~Press 'r' to rewrite ~\n");
	printf("~Press 'p' to print   ~\n");
	printf("~Press 'e' to end     ~\n");
	printf("Press'a' if you forget the bottom meaning\n");
	setbuf(stdin,NULL);
	printf( "\tkey\tid\tname\n");
	printf( "input\tstring\tnumber\tstring\n");
	printf("Press new bottom:\n");
	
	while(1){
	setbuf(stdin,NULL);
	char input=getchar();
	getchar();//read the "enter"

	//create
	if(input=='c'){
		createList(first);
	}

	//delete
	else if(input=='d'){
		char k[20];
		printf("which key you want to delete:\n");
		scanf("%s",k);
		node=searchNode(first,k);
		if(node!=NULL){
			first=deleteNode(first,node);
		}
		else printf("No the key!\n\n");
	}
	
	//print
	else if(input=='p'){
		char pr;
		char k[20];
		printf("print all(press 'l') or only the key message(press 'k'):");
		scanf("%c",&pr);
		getchar();
		if(pr=='k'){
			printf("which key you want to print:\n");
			scanf("%s",k);
			node=searchNode(first,k);
			printf( "key\tid\tname\n");
			printf( "%s\t%d\t%s\n\n" , node->msg.data, node->msg.number, node->msg.name);
		}
		else 
			printList(first);
	}
	
	//insert
	else if(input=='i'){
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
	else if(input=='r'){
		char ip[20];
		printf("Which key you want to rewrite:\n");
		scanf("%s",ip);
		node=searchNode(first,ip);
		rewriteNode(node);
	}

	//free
	else if(input=='e'){
		freeList(first);
		printf("END\n");
		break;
	}

	//bottom meaning
	else if(input=='a'){
		printf("Press 'c' to create,'d' to delete,'i' to insert,'r' to rewrite,'p' to print,'e' to end: \n");
	}

	printf("Press new bottom:\n");
	setbuf(stdin,NULL);
	}
	
	return 0;
}