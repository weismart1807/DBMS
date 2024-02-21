#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cJSON.h>
#define len 30
#define lenforjsonvalue 50
#define MAX_SETS 10
#define MAX_MEMBERS 100
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
int LLEN(NODE *);
NODE *deletelNode(NODE *, NODE *);
NODE *searchlNode(NODE *, char *);

typedef struct {
    char *member;
    double score;
} SORTED_SET_MEMBER;

typedef struct {
    char setName[100];
    SORTED_SET_MEMBER *members;
    size_t size;
} SORTED_SET;

void INITIALIZE_SORTED_SET(SORTED_SET *set, const char *setName) ;
void ZADD(SORTED_SET *set, const char *member, double score);
size_t ZCARD(const SORTED_SET *set);
size_t ZCOUNT(const SORTED_SET *set, double min, double max);
SORTED_SET ZINTERSTORE(const SORTED_SET *set1, const SORTED_SET *set2);
SORTED_SET ZUNIONSTORE(const SORTED_SET *set1, const SORTED_SET *set2);
void ZRANGE(const SORTED_SET *set, size_t start, size_t end);
void ZRANGEBYSCORE(const SORTED_SET *set, double min, double max);
ssize_t ZRANK(const SORTED_SET *set, const char *member);
void ZREM(SORTED_SET *set, const char *member);
void ZREMRANGEBYSCORE(SORTED_SET *set, double min, double max);
void FREE_SORTED_SET(SORTED_SET *set);

typedef struct HashNode {
    char *key;
    char *field;
    char *value;
    struct HashNode *next;
    time_t expireTime;  // 新增 expireTime 成員
} HashNode;

typedef struct HashTable {
    size_t size;
    size_t capacity;
    HashNode **buckets;
    float loadFactor;
} HashTable;

HashTable *createHashTable(size_t capacity, float loadFactor);
unsigned int hash(const char *key, size_t size);

unsigned int hash(const char *key, size_t size) {
    unsigned int hashValue = 0;
    while (*key) {
        hashValue = (hashValue << 5) + *key++;
    }
    return hashValue % size;
}

HashTable *createHashTable(size_t capacity, float loadFactor) {
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    hashTable->size = 0;
    hashTable->capacity = capacity;
    hashTable->loadFactor = loadFactor;
    hashTable->buckets = (HashNode **)calloc(capacity, sizeof(HashNode *));
    return hashTable;
}

void resizeHashTable(HashTable *hashTable, size_t newCapacity) {
    HashNode **newBuckets = (HashNode **)calloc(newCapacity, sizeof(HashNode *));
    
    // Rehash all existing nodes into the new buckets
    for (size_t i = 0; i < hashTable->capacity; ++i) {
        HashNode *current = hashTable->buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            unsigned int newIndex = hash(current->key, newCapacity);
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            current = next;
        }
    }

    // Update the hash table properties
    free(hashTable->buckets);
    hashTable->buckets = newBuckets;
    hashTable->capacity = newCapacity;
}

// 設定過期時間的函式
void setExpiration(HashNode *node, int seconds) {
    if (seconds > 0) {
        // 使用 time 函式取得當前時間
        time_t currentTime = time(NULL);
        node->expireTime = currentTime + seconds;
    } else {
        // 如果 seconds 為 0 或負數，表示永不過期
        node->expireTime = 0;
    }
}

// 檢查節點是否過期的函式
int isExpired(const HashNode *node) {
    if (node->expireTime > 0) {
        // 使用 time 函式取得當前時間
        time_t currentTime = time(NULL);
        return (currentTime > node->expireTime);
    }
    return 0;  // 永不過期
}

// 處理過期邏輯的函式
void handleExpire(HashTable *hashTable, const char *key, int seconds) {
    // 在哈希表中尋找對應的節點
    unsigned int index = hash(key, hashTable->capacity);
    HashNode *current = hashTable->buckets[index];
    HashNode *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // 檢查節點是否過期
            if (isExpired(current)) {
                printf("Key '%s' has expired. Removing the key.\n", key);

                // 刪除節點
                if (prev == NULL) {
                    // 若為第一個節點
                    hashTable->buckets[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);

                return;
            } else {
                // 找到對應的節點，設定過期時間
                setExpiration(current, seconds);
                printf("Key '%s' will expire in %d seconds.\n", key, seconds);
            }
            return;
        }

        prev = current;
        current = current->next;
    }

    // 如果沒找到對應的節點
    printf("Key '%s' not found.\n", key);
}

void hset(HashTable *hashTable, const char *key, const char *field, const char *value) {
    if ((float)hashTable->size / hashTable->capacity > hashTable->loadFactor) {
        // Resize the hash table if load factor exceeds the threshold
        resizeHashTable(hashTable, 2 * hashTable->capacity);
    }

    unsigned int index = hash(key, hashTable->capacity);

    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->field = strdup(field);
    newNode->value = strdup(value);
    newNode->next = hashTable->buckets[index];
    hashTable->buckets[index] = newNode;

    hashTable->size++;
}

void hdel(HashTable *hashTable, const char *key, const char *field) {
    unsigned int index = hash(key, hashTable->capacity);
    HashNode *current = hashTable->buckets[index];
    HashNode *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0 && strcmp(current->field, field) == 0) {
            if (prev == NULL) {
                // If the matching node is the first node in the bucket
                hashTable->buckets[index] = current->next;
            } else {
                // If the matching node is not the first node in the bucket
                prev->next = current->next;
            }

            // Free the memory of the deleted node
            free(current->key);
            free(current->field);
            free(current->value);
            free(current);

            // Decrease the size of the hash table
            hashTable->size--;

            return;  // Node found and deleted
        }
        prev = current;
        current = current->next;
    }
}

char *hget(HashTable *hashTable, const char *key, const char *field) {
    unsigned int index = hash(key, hashTable->capacity);
    HashNode *current = hashTable->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0 && strcmp(current->field, field) == 0) {
            return current->value;  // Return the value of the node
        }
        current = current->next;
    }

    return NULL;  // Node not found
}

void cleanupHashTable(HashTable *hashTable) {
    for (size_t i = 0; i < hashTable->capacity; ++i) {
        HashNode *current = hashTable->buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free(current->key);
            free(current->field);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(hashTable->buckets);
    free(hashTable);
}

void INITIALIZE_SORTED_SET(SORTED_SET *set, const char *setName) {
    strcpy(set->setName, setName);
    set->members = NULL;
    set->size = 0;
}

void ZADD(SORTED_SET *set, const char *member, double score) {
    set->members = realloc(set->members, (set->size + 1) * sizeof(SORTED_SET_MEMBER));
    set->members[set->size].member = strdup(member);
    set->members[set->size].score = score;
    set->size++;
    for(int i=set->size;i--;i>0){
        if(set->members[i].score < set->members[i-1].score){
            // Swap scores
            double tempScore = set->members[i].score;
            set->members[i].score = set->members[i - 1].score;
            set->members[i - 1].score = tempScore;

            // Swap member names
            char *tempMember = set->members[i].member;
            set->members[i].member = set->members[i - 1].member;
            set->members[i - 1].member = tempMember;
            }
    }
}

size_t ZCARD(const SORTED_SET *set) {
    return set->size;
}

size_t ZCOUNT(const SORTED_SET *set, double minScore, double maxScore) {
    size_t count = 0;
    for (size_t i = 0; i < set->size; ++i) {
        if (set->members[i].score >= minScore && set->members[i].score <= maxScore) {
            count++;
        }
    }
    return count;
}

void ZRANGE(const SORTED_SET *set, size_t start, size_t stop) {
    if (start >= set->size || stop >= set->size || start > stop) {
        printf("Invalid range.\n");
        return;
    }

    printf("Members in set %s, range [%zu, %zu]:\n", set->setName, start, stop);
    for (size_t i = start; i <= stop; ++i) {
        printf("Member: %s, Score: %f\n", set->members[i].member, set->members[i].score);
    }
}

SORTED_SET ZINTERSTORE(const SORTED_SET *set1, const SORTED_SET *set2) {
    SORTED_SET result;
    INITIALIZE_SORTED_SET(&result, "INTER_RESULT");

    for (size_t i = 0; i < set1->size; ++i) {
        for (size_t j = 0; j < set2->size; ++j) {
            if (strcmp(set1->members[i].member, set2->members[j].member) == 0) {
                ZADD(&result, set1->members[i].member, set1->members[i].score);
                break;
            }
        }
    }

    return result;
}

SORTED_SET ZUNIONSTORE(const SORTED_SET *set1, const SORTED_SET *set2) {
    SORTED_SET result;
    INITIALIZE_SORTED_SET(&result, "UNION_RESULT");

    for (size_t i = 0; i < set1->size; ++i) {
        ZADD(&result, set1->members[i].member, set1->members[i].score);
    }

    for (size_t i = 0; i < set2->size; ++i) {
        size_t j;
        for (j = 0; j < result.size; ++j) {
            if (strcmp(set2->members[i].member, result.members[j].member) == 0) {
                result.members[j].score = set2->members[i].score;
                break;
            }
        }

        if (j == result.size) {
            ZADD(&result, set2->members[i].member, set2->members[i].score);
        }
    }

    return result;
}

void ZRANGEBYSCORE(const SORTED_SET *set, double min, double max) {
    for (size_t i = 0; i < set->size; ++i) {
        if (set->members[i].score >= min && set->members[i].score <= max) {
            printf("%s ", set->members[i].member);
        }
    }
    printf("\n");
}

ssize_t ZRANK(const SORTED_SET *set, const char *member) {
    for (size_t i = 0; i < set->size; ++i) {
        if (strcmp(set->members[i].member, member) == 0) {
            return i;
        }
    }
    return -1;
}

void ZREM(SORTED_SET *set, const char *member) {
    for (size_t i = 0; i < set->size; ++i) {
        if (strcmp(set->members[i].member, member) == 0) {
            free(set->members[i].member);
            memmove(set->members + i, set->members + i + 1, (set->size - i - 1) * sizeof(SORTED_SET_MEMBER));
            set->size--;
            set->members = realloc(set->members, set->size * sizeof(SORTED_SET_MEMBER));
            break;
        }
    }
}

// Function to remove members in a specified score range
void ZREMRANGE(SORTED_SET *set, double minScore, double maxScore) {
    // Create a new array to store members after removal
    SORTED_SET_MEMBER *newMembers = (SORTED_SET_MEMBER *)malloc(set->size * sizeof(SORTED_SET_MEMBER));
    size_t newSize = 0;

    // Iterate through the existing members and copy those not in the specified range
    for (size_t i = 0; i < set->size; ++i) {
        if (set->members[i].score < minScore || set->members[i].score > maxScore) {
            // Copy the member to the new array
            newMembers[newSize].member = strdup(set->members[i].member);
            newMembers[newSize].score = set->members[i].score;
            newSize++;
        } else {
            // Free memory for members within the specified range
            free(set->members[i].member);
        }
    }

    // Free the old members array
    free(set->members);

    // Update set with the new array and size
    set->members = newMembers;
    set->size = newSize;
}

void ZREMRANGEBYSCORE(SORTED_SET *set, double min, double max) {
    size_t i = 0;
    while (i < set->size) {
        if (set->members[i].score >= min && set->members[i].score <= max) {
            free(set->members[i].member);
            memmove(set->members + i, set->members + i + 1, (set->size - i - 1) * sizeof(SORTED_SET_MEMBER));
            set->size--;
            set->members = realloc(set->members, set->size * sizeof(SORTED_SET_MEMBER));
        } else {
            i++;
        }
    }
}

void FREE_SORTED_SET(SORTED_SET *set) {
    for (size_t i = 0; i < set->size; ++i) {
        free(set->members[i].member);
    }
    free(set->members);
    set->size=0;
}

cJSON *getNewJsonValue() {
    char jsonString[lenforjsonvalue];
    cJSON *newValue = NULL;

    printf("Enter value in JSON format (type 'exit' to stop):\n");
    
    while (1) {
        printf("Example JSON format: {\"value1\":\"string1\",\"value2\":42,\"value3\":[1, 2, 3]}\n");
        printf("Enter value: ");
        scanf(" %29[^\n]%*c", jsonString);

        if (strcmp(jsonString, "exit") == 0) {
            break;  // 用戶輸入 'exit'，退出迴圈
        }

        newValue = cJSON_Parse(jsonString);
        if (newValue != NULL) {
            break;  // 輸入的 JSON 字串有效，退出迴圈
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
        // 比較鍵值以決定往左或往右插入
        int cmpResult = strcmp(newNode->key, (*root)->key);

        if (cmpResult < 0) {
            insertNodeToTree(&(*root)->left, newNode);
        } else if (cmpResult > 0) {
            insertNodeToTree(&(*root)->right, newNode);
        } else {
            // 鍵已存在，更新值
            cJSON_Delete((*root)->value);

            // 檢查新節點的值是否為NULL
            if (newNode->value != NULL) {
                // 將新節點的值複製到樹中
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

            free(newNode);  // 釋放新創建的節點，因為它不再需要
        }
    }
}

// 新增樹到樹陣列
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

        // 將新節點插入樹中
        insertNodeToTree(&newTree, newNode);
    }

    // 將新樹添加到樹陣列
    trees[*numTrees] = newTree;
    (*numTrees)++;
}

// 列印所有樹
void printAllTrees(NODE **trees, int numTrees) {
    for (int i = 0; i < numTrees; i++) {
        printf("Tree %d:\n", i + 1);
        printTree(trees[i]);
        printf("\n");
    }
}

// 列印樹（中序遍歷）
void printTree(NODE *root) {
    if (root != NULL) {
        printf("Key: %s, Value: %s\n", root->key, cJSON_Print(root->value));
        printTree(root->left);
        printTree(root->right);
    }
}

// 新增節點到指定樹
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

// 由用戶輸入創建新節點
NODE *createNodeFromUserInput() {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    printf("Enter key: ");
    scanf("%s", newNode->key);
    newNode->value = getNewJsonValue();
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 在所有樹中搜索節點
NODE *searchNodeInAllTrees(NODE **trees, int numTrees, char *key) {
    NODE *foundNode = NULL;
    for (int i = 0; i < numTrees; i++) {
        foundNode = searchNodeInTree(trees[i], key);
        if (foundNode != NULL) {
            printf("Found in %d. tree",i+1);
            break;  // 如果在任何樹中找到節點，退出循環
        }
    }
    return foundNode;
}

// 在樹中搜索節點
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

// 刪除指定鍵的節點
NODE* deleteNode(NODE* root, char* key) {
    if (root == NULL) {
        return root;
    }

    // 遞歸地搜索節點
    if (strcmp(key, root->key) < 0) {
        root->left = deleteNode(root->left, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = deleteNode(root->right, key);
    } else {
        // 找到要刪除的節點
        // 情況1: 沒有子節點或只有一個子節點
        if (root->left == NULL) {
            NODE* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            NODE* temp = root->left;
            free(root);
            return temp;
        }

        // 情況2: 有兩個子節點
        // 找到右子樹中的最小節點（或左子樹中的最大節點）
        NODE* temp = minValueNode(root->right);

        // 複製最小節點的值到當前節點
        strcpy(root->key, temp->key);
        cJSON_Delete(root->value);
        root->value = cJSON_Duplicate(temp->value, 1);

        // 刪除最小節點
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

// 找到最小值節點
NODE* minValueNode(NODE* node) {
    NODE* current = node;

    // 循環找到最左邊的節點
    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

// 刪除整棵樹
void deleteTree(NODE* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        cJSON_Delete(root->value);
        free(root);
    }
}

// 釋放樹分配的內存
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
