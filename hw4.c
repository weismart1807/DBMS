#include<stdio.h>
#include<stdlib.h>
#include "hw4.h"
#define treenumber 10
#define len 30
int main(){

		NODE *first,*node;
		int numTrees = 0;
    NODE* trees[treenumber];  // Assuming a maximum of 10 trees, you can adjust this based on your needs.

    while (1) {
			printf("SET,GET,LPUSH,RPUSH,LPOP,RPOP,LLEN,LRANGE,DELETE,INSERT,UPGRADE,FREE \n");
      printf("Enter 'add' to add a new tree, 'addnode' to add a new node, 'print' to print all trees,\n");
			printf("Enter 'search' to search for a node, 'delete' to delete node, 'exit' to quit: ");
      char input[20];
      scanf("%s", input);

        if (strcmp(input, "add") == 0) {
            addTree(trees, &numTrees);
        } else if (strcmp(input, "addnode") == 0) {
            addNodeToTree(trees, numTrees);
        } else if (strcmp(input, "print") == 0) {
            printAllTrees(trees, numTrees);
        } else if (strcmp(input, "search") == 0) {
            char keyToSearch[100];
            printf("Enter key to search: ");
            scanf("%s", keyToSearch);
            NODE *foundNode = searchNodeInAllTrees(trees, numTrees, keyToSearch);
            if (foundNode != NULL) {
                printf("Node found. Key: %s, Value: %s\n", foundNode->key, cJSON_Print(foundNode->value));
            } else {
                printf("Node not found.\n");
            }
        } else if (strcmp(input, "delete") == 0) {
						// 刪除節點或樹
						char deleteType[10];
						printf("Enter 'node' to delete a node, 'tree' to delete a tree: ");
						scanf("%s", deleteType);

						if (strcmp(deleteType, "node") == 0) {
							// 刪除節點
							char nodeKey[len];
							printf("Enter key of the node to delete: ");
							scanf("%s", nodeKey);

							for (int i = 0; i < numTrees; i++) {
								trees[i] = deleteNode(trees[i], nodeKey);
							}
						} else if (strcmp(deleteType, "tree") == 0) {
							// 刪除樹
							char treeKey[len];
							printf("Enter key of the tree to delete: ");
							scanf("%s", treeKey);

							for (int i = 0; i < numTrees; i++) {
								if (strcmp(treeKey, trees[i]->key) == 0) {
									deleteTree(trees[i]);
									// 將被刪除的樹設為NULL，以防止後續訪問
									trees[i] = NULL;
									break;
								}
							}
								} else {
										printf("Invalid delete type. Try again.\n");
								}
    	} else if (strcmp(input, "exit") == 0) {
            // Free the memory allocated for all trees
						for (int i = 0; i < numTrees; i++) {
							freeTree(trees[i]);  // Pass the root of the tree, not the entire tree
						}
									break;
      } else if(strcmp(input,"LPUSH")==0){
						LPUSH(&first);
			} else if(strcmp(input,"RPUSH")==0){
						RPUSH(&first);
			} else if(strcmp(input,"LPOP")==0){
						LPOP(&first);
			} else if(strcmp(input,"RPOP")==0){
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
				node=searchlNode(first,k);
				if(node!=NULL){
					first=deletelNode(first,node);
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
				node=searchlNode(first,k);
				printf( "'key'\t'value'\n");
				printf( "%s\t%s\n" , node->msg.key, node->msg.value);
			}
			
			//insert
			else if(strcmp(input,"INSERT")==0){
				char ip[20];
				printf("which key you want to follow:");
				scanf("%s",ip);
				node=searchlNode(first,ip);
				if(node!=NULL){
					insertlNode(node);
				}
				else printf("No the key!\n\n");
			}

			//upgrade
			else if(strcmp(input,"UPGRADE")==0){
				char ip[20];
				printf("Which key you want to rewrite:\n");
				scanf("%s",ip);
				node=searchlNode(first,ip);
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
						printf("Enter 'add' to add a new tree, 'addnode' to add a new node, 'print' to print all trees,\n");
				printf("Enter 'search' to search for a node, 'delete' to delete node, 'exit' to quit: ");
			} else {
					printf("Invalid command. Try again.\n");
				}
			}
		return 0;
}