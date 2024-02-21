#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cJSON.h>
#define len 30
#define lenforjsonvalue 50
struct info
{
    char key[len];
    char value[len];
};
typedef struct info info;
struct node
{
    info msg;
    struct node *next;
    struct node *prev;
    char key[len];
    cJSON *value;
    struct NODE *left;
    struct NODE *right;
};
typedef struct node NODE;

NODE *createNodeFromUserInput();
NODE *searchNodeInTree(NODE *root, char *key);
NODE *minValueNode(NODE *node);

cJSON *getNewJsonValue() {
    char jsonString[lenforjsonvalue];
    cJSON *newValue = NULL;

    printf("Enter value in JSON format (type 'exit' to stop):\n");
    
    while (1) {
        printf("Example JSON format: {\"value1\":\"string1\",\"value2\":42,\"value3\":[1, 2, 3]}\n");
        printf("Enter value: ");
        scanf(" %29[^\n]%*c", jsonString);

        if (strcmp(jsonString, "exit") == 0) {
            break;  
        }

        newValue = cJSON_Parse(jsonString);
        if (newValue != NULL) {
            break;  
        } else {
            fprintf(stderr, "Error: Failed to parse JSON value. Please enter a valid JSON string.\n");
        }
    }
    return newValue;
}

void insertNodeToTree(NODE **root, NODE *newNode) {
    if (*root == NULL) {
        *root = newNode;
    } else {
        int cmpResult = strcmp(newNode->key, (*root)->key);

        if (cmpResult < 0) {
            insertNodeToTree(&(*root)->left, newNode);
        } else if (cmpResult > 0) {
            insertNodeToTree(&(*root)->right, newNode);
        } else {
            cJSON_Delete((*root)->value);

            if (newNode->value != NULL) {
                cJSON *newValue = cJSON_Duplicate(newNode->value, 1);

                if (newValue != NULL) {
                    (*root)->value = newValue;
                    printf("Successfully inserted: Key: %s, Value: %s\n", newNode->key, cJSON_Print(newValue));
                } else {
                    fprintf(stderr, "Error: cJSON_Duplicate failed\n");
                    printf("Failed to insert: Key: %s, Value: (null)\n", newNode->key);
                }
            } else {
                fprintf(stderr, "Error: Attempting to insert a node with NULL value\n");
                printf("Failed to insert: Key: %s, Value: (null)\n", newNode->key);
            }

            free(newNode);  
        }
    }
}

void addTree(NODE **trees, int *numTrees) {
    NODE *newTree = NULL;

    printf("Enter key-value pairs for the new tree (type 'exit' to stop):\n");
    while (1) {
        NODE *newNode = (NODE *)malloc(sizeof(NODE));
        printf("Enter key: ");
        scanf("%s", newNode->key);

        if (strcmp(newNode->key, "exit") == 0) {
            free(newNode);
            break;
        }

        newNode->value = getNewJsonValue();
        newNode->left = NULL;
        newNode->right = NULL;

        insertNodeToTree(&newTree, newNode);
    }

    trees[*numTrees] = newTree;
    (*numTrees)++;
}

void printAllTrees(NODE **trees, int numTrees) {
    for (int i = 0; i < numTrees; i++) {
        printf("Tree %d:\n", i + 1);
        printTree(trees[i]);
        printf("\n");
    }
}

void printTree(NODE *root) {
    if (root != NULL) {
        printTree(root->left);
        printf("Key: %s, Value: %s\n", root->key, cJSON_Print(root->value));
        printTree(root->right);
    }
}

void addNodeToTree(NODE **trees, int numTrees) {
    char treeKey[len];
    printf("Add to which tree: ");
    scanf("%s", treeKey);

    NODE *selectedTree = NULL;
    for (int i = 0; i < numTrees; i++) {
        if (strcmp(treeKey, trees[i]->key) == 0) {
            selectedTree = trees[i];
            break;
        }
    }

    if (selectedTree != NULL) {
        NODE *nodeToInsert = createNodeFromUserInput();
        insertNodeToTree(&selectedTree, nodeToInsert);
    } else {
        printf("Tree with key '%s' not found.\n", treeKey);
    }
}

NODE *createNodeFromUserInput() {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    printf("Enter key: ");
    scanf("%s", newNode->key);
    newNode->value = getNewJsonValue();
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

NODE *searchNodeInAllTrees(NODE **trees, int numTrees, char *key) {
    NODE *foundNode = NULL;
    for (int i = 0; i < numTrees; i++) {
        foundNode = searchNodeInTree(trees[i], key);
        if (foundNode != NULL) {
            printf("Found in %d. tree",i+1);
            break;  
        }
    }
    return foundNode;
}

NODE *searchNodeInTree(NODE *root, char *key) {
    if (root == NULL || strcmp(key, root->key) == 0) {
        return root;
    }

    if (strcmp(key, root->key) < 0) {
        return searchNodeInTree(root->left, key);
    } else if(strcmp(key, root->key) > 0){
        return searchNodeInTree(root->right, key);
    } 
}

NODE* deleteNode(NODE* root, char* key) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(key, root->key) < 0) {
        root->left = deleteNode(root->left, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            NODE* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            NODE* temp = root->left;
            free(root);
            return temp;
        }

        NODE* temp = minValueNode(root->right);
        strcpy(root->key, temp->key);
        cJSON_Delete(root->value);
        root->value = cJSON_Duplicate(temp->value, 1);

        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

NODE* minValueNode(NODE* node) {
    NODE* current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

void deleteTree(NODE* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        cJSON_Delete(root->value);
        free(root);
    }
}

void freeTree(NODE *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        cJSON_Delete(root->value);
        free(root);
    }
}

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
    int sum = 1;
    if(current->next == NULL)
        return 0;
    else{
        while (current->next != NULL) {
            current = current->next;
            sum++;
        }
        return sum;
    }
}

NODE* deletelNode(NODE *first, NODE *point){
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

void insertlNode(NODE *node){
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

NODE *searchlNode(NODE *first, char *item){
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
