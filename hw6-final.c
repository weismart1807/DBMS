#include <stdio.h>
#include <stdlib.h>
#include <ev.h>
#include "hw6-final.h"
#define treenumber 10
#define len 30
#define MAX_SETS 100
#define MAX_HASHTABLES 100

// Function prototypes
void command_callback(struct ev_loop *loop, ev_io *w, int revents);
void timer_callback(struct ev_loop *loop, ev_timer *w, int revents);

NODE *first,*node;
int numTrees = 0;
NODE* trees[treenumber];  // Assuming a maximum of 10 trees, you can adjust this based on your needs.

SORTED_SET sets[MAX_SETS];
int numSets = 0;
char command[20];
    
void printCommandPrompt() {
    printf("/HW1,3/ Enter SET,GET,LPUSH,RPUSH,LPOP,RPOP,LLEN,LRANGE,DELETE,INSERT,UPGRADE,FREE\n");
    printf("/HW4/ Enter 'add' to add a new tree, 'addnode' to add a new node, 'print' to print all trees,\n");
    printf("/HW4/ Enter 'search' to search for a node, 'delete' to delete node, 'exit' to quit\n");
    printf("/HW5/ Enter ZADD, ZCARD, ZRANGE, ZCOUNT, ZINTERSTORE, ZUNIONSTORE,\n");
    printf("/HW5/ Enter ZRANGEBYSCORE, ZRANK, ZREM, ZREMRANGE, ZREMRANGEBYSCORE, QUIT\n");
    printf("/HW6/ Enter HSET, HGET, HDEL, EXPIRE, HFREE\n");
    printf(">> ");
    fflush(stdout);  
}

int main() {
    // Initialize libev loop
    struct ev_loop *loop = ev_default_loop(0);

    printCommandPrompt();
    // Initialize an IO watcher for handling commands
    ev_io command_watcher;
    ev_io_init(&command_watcher, command_callback, 0, EV_READ);
    ev_io_start(loop, &command_watcher);

    // 初始化哈希表
    HashTable *hashTable = createHashTable(16, 0.75);
    // Pass hashTable to command_callback
    ev_set_userdata(&command_watcher, hashTable);
    // Set hashTable as userdata
    ev_set_userdata(loop, hashTable);

    // Initialize a timer watcher for expiration
    ev_timer timer_watcher;
    ev_timer_init(&timer_watcher, timer_callback, 0.0, 2.0);  // Set the timer to expire every 5 seconds
    // Pass hashTable to command_callback
    ev_set_userdata(&timer_watcher, hashTable);
    ev_timer_start(loop, &timer_watcher);

    // Run the event loop
    ev_run(loop, 0);

    // Cleanup
    ev_io_stop(loop, &command_watcher);
    ev_timer_stop(loop, &timer_watcher);
    ev_loop_destroy(loop);

    return 0;
}


void command_callback(struct ev_loop *loop, ev_io *w, int revents) {
    fflush(stdin);
    char command[256];
    fgets(command, sizeof(command), stdin);

    // 移除換行符
    size_t length = strlen(command);
    if (length > 0 && command[length - 1] == '\n') {
        command[length - 1] = '\0';
    }

    // For example, if the command is "exit", stop the event loop
    if (strcmp(command, "EXIT") == 0) {
        ev_unloop(loop, EVUNLOOP_ALL);
        ev_break(loop, EVBREAK_ONE);
        return;
    }

    // Get hashTable from userdata
    HashTable *hashTable = ev_userdata(loop);

    // Add your logic to handle other commands
    // You may call functions for hash table, tree, list operations based on the command
    // For simplicity, assuming all commands are executed in a single loop
        
        
        char key[len],field[len],value[len];
        
        if (strcmp(command, "HSET") == 0) {
            printf("key field value\n");
            scanf("%s %s %s", key, field, value);

            // Set key-value pair
            hset(hashTable, key, field, value);

        } else if (strcmp(command, "HGET") == 0) {
            printf("key field\n");
            scanf("%s %s", key, field);

            // Get value for a specific field
            char *result = hget(hashTable, key, field);
            if (result != NULL) {
                printf("HGET result: %s\n", result);
            } else {
                printf("HGET result: Not found\n");
            }

        } else if (strcmp(command, "HDEL") == 0) {
            printf("key field\n");
            scanf("%s %s", key, field);

            // Delete a specific field
            hdel(hashTable, key, field);

        } else if (strcmp(command, "HFREE") == 0) {
            // Free the memory of the hash table
            cleanupHashTable(hashTable);
            ev_break;
        } else if (strcmp(command, "EXPIRE") == 0) {
            int seconds;
            printf("key seconds\n");
            scanf("%s %d", key, &seconds);
            handleExpire(hashTable, key, seconds);
        } else if (strcmp(command, "QUIT") == 0) {
            for (int i = 0; i < numSets; ++i) {
                FREE_SORTED_SET(&sets[i]);
            }
        } else if (strcmp(command, "ZADD") == 0) {
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

            printf("Enter the score and member (or enter 'done' to finish):\n");
            while (scanf("%lf %99s%*c", &score, member) == 2) {
                if (strcmp(member, "done") == 0) {
                    break;
                }

                ZADD(set, member, score);
                printf("Enter the score and member (or enter 'done' to finish):\n");
            }
        } else if (strcmp(command, "ZRANGE") == 0) {
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
        } else if (strcmp(command, "ZCARD") == 0){
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
        } else if (strcmp(command, "ZCOUNT") == 0) {
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
        } else if (strcmp(command, "ZINTERSTORE") == 0) {
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
        } else if (strcmp(command, "ZUNIONSTORE") == 0) {
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
        } else if (strcmp(command, "ZRANGEBYSCORE") == 0) {
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
        } else if (strcmp(command, "ZRANK") == 0) {
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
        } else if (strcmp(command, "ZREM") == 0) {
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
        } else if (strcmp(command, "ZREMRANGE") == 0) {
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
        } else if (strcmp(command, "ZREMRANGEBYSCORE") == 0) {
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
        } else if (strcmp(command, "add") == 0) {
            addTree(trees, &numTrees);
        } else if (strcmp(command, "addnode") == 0) {
            addNodeToTree(trees, numTrees);
        } else if (strcmp(command, "print") == 0) {
            printAllTrees(trees, numTrees);
        } else if (strcmp(command, "search") == 0) {
            char keyToSearch[100];
            printf("Enter key to search: ");
            scanf("%s", keyToSearch);
            NODE *foundNode = searchNodeInAllTrees(trees, numTrees, keyToSearch);
            if (foundNode != NULL) {
                printf("Node found. Key: %s, Value: %s\n", foundNode->key, cJSON_Print(foundNode->value));
            } else {
                printf("Node not found.\n");
            }
        } else if (strcmp(command, "delete") == 0) {
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
    	} else if (strcmp(command, "exit") == 0) {
            // Free the memory allocated for all trees
			for (int i = 0; i < numTrees; i++) {
				freeTree(trees[i]);  // Pass the root of the tree, not the entire tree
			}
        } else if(strcmp(command,"LPUSH")==0){
			LPUSH(&first);
		} else if(strcmp(command,"RPUSH")==0){
			RPUSH(&first);
		} else if(strcmp(command,"LPOP")==0){
			LPOP(&first);
		} else if(strcmp(command,"RPOP")==0){
			RPOP(first);
		} else if(strcmp(command,"LLEN")==0){
			int nodenumber = LLEN(first);
			printf("%d",nodenumber);
		} else if(strcmp(command,"LRANGE")==0){
			printList(&first);
		} else if(strcmp(command,"DELETE")==0){
			char k[20];
			printf("which key you want to delete:\n");
			scanf("%s",k);
			node=searchlNode(first,k);
			if(node!=NULL){
				first=deletelNode(first,node);
			}
			else printf("No the key!\n\n");
		} else if(strcmp(command,"SET")==0){
			RPUSH(&first);
		} else if(strcmp(command,"GET")==0){
			char k[20];
			printf("which key you want to print:\n");
			scanf("%s",k);
			node=searchlNode(first,k);
			printf( "'key'\t'value'\n");
			printf( "%s\t%s\n" , node->msg.key, node->msg.value);
		} else if(strcmp(command,"INSERT")==0){
			char ip[20];
			printf("which key you want to follow:");
			scanf("%s",ip);
			node=searchlNode(first,ip);
			if(node!=NULL){
				insertlNode(node);
			}
			else printf("No the key!\n\n");
		} else if(strcmp(command,"UPGRADE")==0){
			char ip[20];
			printf("Which key you want to rewrite:\n");
			scanf("%s",ip);
			node=searchlNode(first,ip);
			rewriteNode(node);
		} else if(strcmp(command,"FREE")==0){
			freeList(first);
		} else {
				printf("Invalid command. Try again.\n");
		}
        printf(">> ");
        getchar();
        fflush(stdout);
        fflush(stdin);
}

void timer_callback(struct ev_loop *loop, ev_timer *w, int revents) {
    HashTable *hashTable = (HashTable *)ev_userdata(loop);

    for (size_t i = 0; i < hashTable->capacity; ++i) {
        HashNode *current = hashTable->buckets[i];
        HashNode *prev = NULL;

        while (current != NULL) {
            if (isExpired(current)) {
                printf("~ Key '%s' has expired! ~\n", current->key);
                printf(">> ");
                fflush(stdout);

                if (prev == NULL) {
                    hashTable->buckets[i] = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);
                
            }

            prev = current;
            current = current->next;
        }
    }
}
