#include<stdio.h>
#include<stdlib.h>
#include "hw5.h"
#define treenumber 10
#define len 30
#define MAX_SETS 100

int main(){

	NODE *first,*node;
	int numTrees = 0;
    NODE* trees[treenumber];  

    SORTED_SET sets[MAX_SETS];
    int numSets = 0;
    char input[20];

    while (1) {
		printf("/HW1,3/SET,GET,LPUSH,RPUSH,LPOP,RPOP,LLEN,LRANGE,DELETE,INSERT,UPGRADE,FREE \n");
        printf("/HW4/Enter 'add' to add a new tree, 'addnode' to add a new node, 'print' to print all trees,\n");
		printf("/HW4/Enter 'search' to search for a node, 'delete' to delete node, 'exit' to quit \n");
        printf("/HW5/Enter ZADD, ZCARD, ZRANGE, ZCOUNT, ZINTERSTORE, ZUNIONSTORE, ZRANGEBYSCORE, ZRANK, ZREM, ZREMRANGE, ZREMRANGEBYSCORE, QUIT: ");
        setbuf(stdin,NULL);
        scanf(" %19s%*c", input);

        if (strcmp(input, "QUIT") == 0) {
            for (int i = 0; i < numSets; ++i) {
                FREE_SORTED_SET(&sets[i]);
            }
            break;
        } else if (strcmp(input, "ZADD") == 0) {
            char setName[100];
            printf("Enter the set name: ");
            scanf(" %99s", setName);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set == NULL) {
                if (numSets < MAX_SETS) {
                    INITIALIZE_SORTED_SET(&sets[numSets], setName);
                    set = &sets[numSets];
                    numSets++;
                } else {
                    printf("Maximum number of sets reached.\n");
                }
            }

            double score;
            char member[100];

            printf("Enter the score and member (or enter '0 done' to finish):\n");
            while (scanf(" %lf %99s", &score, member) == 2) {
                if (strcmp(member, "done") == 0) {
                    break;
                }

                ZADD(set, member, score);
                printf("Enter the score and member (or enter '0 done' to finish):\n");
            }
        } else if (strcmp(input, "ZRANGE") == 0) {
            char setName[100];
            printf("Enter the set name: ");
            scanf(" %99s", setName);

            size_t start, stop;
            printf("Enter the start index:(number) ");
            scanf(" %zu", &start);
            printf("Enter the stop index:(number) ");
            scanf(" %zu", &stop);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (start <= stop && start < set->size && stop < set->size) {
                ZRANGE(set, start, stop);
            } else {
                printf("Invalid range.\n");
            }
        } else if (strcmp(input, "ZCARD") == 0){
            char setName[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);
            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                size_t count = ZCARD(set);
                printf("Number of members in the set: %zu\n", count);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZCOUNT") == 0) {
            char setName[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);

            double minScore, maxScore;
            printf("Enter the minimum score: ");
            scanf("%lf", &minScore);
            printf("Enter the maximum score: ");
            scanf("%lf", &maxScore);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                size_t count = ZCOUNT(set, minScore, maxScore);
                printf("Number of members in the specified range: %zu\n", count);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZINTERSTORE") == 0) {
            char setName1[100], setName2[100];
            printf("Enter the first set name: ");
            scanf("%99s", setName1);
            printf("Enter the second set name: ");
            scanf("%99s", setName2);

            SORTED_SET *set1 = NULL, *set2 = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName1) == 0) {
                    set1 = &sets[i];
                }
                if (strcmp(sets[i].setName, setName2) == 0) {
                    set2 = &sets[i];
                }
            }

            if (set1 != NULL && set2 != NULL) {
                SORTED_SET result = ZINTERSTORE(set1, set2);
                printf("Intersection result set created.\n");
                
                size_t count = ZCARD(&result);
                printf("ZINTERSTORE: %zu\n", count);

                FREE_SORTED_SET(&result);
            } else {
                printf("One or more sets not found.\n");
            }
        } else if (strcmp(input, "ZUNIONSTORE") == 0) {
            char setName1[100], setName2[100];
            printf("Enter the first set name: ");
            scanf("%99s", setName1);
            printf("Enter the second set name: ");
            scanf("%99s", setName2);

            SORTED_SET *set1 = NULL, *set2 = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName1) == 0) {
                    set1 = &sets[i];
                }
                if (strcmp(sets[i].setName, setName2) == 0) {
                    set2 = &sets[i];
                }
            }

            if (set1 != NULL && set2 != NULL) {
                SORTED_SET result = ZUNIONSTORE(set1, set2);
                printf("Union result set created.\n");
                
                size_t count = ZCARD(&result);
                printf("ZINTERSTORE: %zu\n", count);

                FREE_SORTED_SET(&result);
            } else {
                printf("One or more sets not found.\n");
            }
        } else if (strcmp(input, "ZRANGEBYSCORE") == 0) {
            char setName[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);

            double minScore, maxScore;
            printf("Enter the minimum score: ");
            scanf("%lf", &minScore);
            printf("Enter the maximum score: ");
            scanf("%lf", &maxScore);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                ZRANGEBYSCORE(set, minScore, maxScore);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZRANK") == 0) {
            char setName[100], member[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);
            printf("Enter the member: ");
            scanf("%99s", member);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                ssize_t rank = ZRANK(set, member);
                if (rank != -1) {
                    printf("Rank of member %s in set %s: %zd\n", member, setName, rank);
                } else {
                    printf("Member not found in set.\n");
                }
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZREM") == 0) {
            char setName[100], member[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);
            printf("Enter the member: ");
            scanf("%99s", member);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                ZREM(set, member);
                printf("Member %s removed from set %s.\n", member, setName);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZREMRANGE") == 0) {
            char setName[100];
            size_t start, stop;

            printf("Enter the set name: ");
            scanf("%99s", setName);
            SORTED_SET *set = NULL;

            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                printf("Enter the start index: ");
                scanf("%zu", &start);
                printf("Enter the stop index: ");
                scanf("%zu", &stop);
                ZREMRANGE(set, start, stop);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "ZREMRANGEBYSCORE") == 0) {
            char setName[100];
            printf("Enter the set name: ");
            scanf("%99s", setName);

            double minScore, maxScore;
            printf("Enter the minimum score: ");
            scanf("%lf", &minScore);
            printf("Enter the maximum score: ");
            scanf("%lf", &maxScore);

            SORTED_SET *set = NULL;
            for (int i = 0; i < numSets; ++i) {
                if (strcmp(sets[i].setName, setName) == 0) {
                    set = &sets[i];
                    break;
                }
            }

            if (set != NULL) {
                ZREMRANGEBYSCORE(set, minScore, maxScore);
                printf("Members removed from set %s within the specified score range.\n", setName);
            } else {
                printf("Set not found.\n");
            }
        } else if (strcmp(input, "add") == 0) {
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