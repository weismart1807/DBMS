#include <hiredis/hiredis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hw2.h"
#define M 100001
#define N 10

int main(){

    int kN=2*N,vN=N;
    char input[vN],key[kN],value[vN];
    char create[]="create",read[]="read",off[]="off";
    double sum = 0;
    
    //mine
    NODE *first;
    first = (NODE*)malloc(sizeof(NODE));

    //redis
    redisContext *c = redisConnect("127.0.0.1",6379);
    if(c == NULL || c -> err){
        if(c){
            printf("Error: %s\n",c->errstr);
        }
        else{
            printf("Cannot allocate redis context\n");
        }
    }
    redisReply *reply;
    int redissize=0;


    while(1){
	    setbuf(stdin,NULL);
        scanf("%s",input);
        getchar();

        //create
        if(strcmp(input,create)==0){
            
            //mine
            clock_t begin = clock();
            for(int i = 0;i < M;i++){
                if(kN>1)kN--;
                else kN=2*N;
                if(vN>1)vN--;
                else vN=N;
                genRandomString(kN, key);
                genRandomString(vN, value);

                createList(first,key,value);
            }
            clock_t end = clock();
            int memorysize = M*sizeof(NODE);//size
            sum=(double)(end-begin)/CLOCKS_PER_SEC;
            printf("'myredis' create time for all %d pairs: %f s\n",M,sum);
            printf("'myredis' create time for 1 pairs (average): %f s\n",sum/M);
            printf("'myredis' create size for all %d pairs: %d bytes\n",M,memorysize);
            printf("'myredis' create size for 1 pairs (average): %d bytes\n",memorysize/M);
            sum = 0.0;

            //redis
            begin = clock();
            for(int i=0;i<M;i++){
                if(kN>1)kN--;
                else kN=2*N;
                if(vN>1)vN--;
                else vN=N;
                genRandomString(kN, key);
                genRandomString(vN, value);
                
                reply=redisCommand(c,"SET %s %s",key,value);
                if(reply == NULL){
                    printf("ERROR: %s",c->errstr);
                    redisFree(c);
                    return 1;
                }
                freeReplyObject(reply);

                //size
                reply=redisCommand(c,"MEMORY USAGE %s",key);
                if(reply == NULL){
                    printf("ERROR: %s",c->errstr);
                    redisFree(c);
                    return 1;
                }
                redissize+=reply->integer;
            }
            end = clock();
            sum = (double)(end-begin)/CLOCKS_PER_SEC;
            printf("'redis' create time for all %d pairs: %f s\n",M,sum);
            printf("'redis' create time for 1 pairs (average): %f s\n",sum/M);
            printf("'redis' create size for all %d pairs: %d bytes\n",M,redissize);
            printf("'redis' create size for 1 pairs (average): %d bytes\n",redissize/M);
        }

        //read
        else if(strcmp(input,read)==0){
            //mine
            clock_t begin = clock();
            printList(first);
            clock_t end = clock();
            sum = (double)(end-begin)/CLOCKS_PER_SEC;
            printf("'myredis' read time for all %d pairs: %f s\n",M,sum);
            printf("'myredis' read time for 1 pairs: %f s\n",sum/M);
            sum = 0.0;
            freeList(first);
            
            //redis
            begin = clock();
            // Send a "KEYS *" command to get all keys in the Redis database
            reply = redisCommand(c, "KEYS *");
            if (reply == NULL) {
                printf("Error executing the command: %s\n", c->errstr);
                redisFree(c);
                return 1;
            }

            if (reply->type == REDIS_REPLY_ARRAY) {
                for (size_t i = 0; i < reply->elements; i++) {
                    printf("Key: %s\n", reply->element[i]->str);
                    // Retrieve the value for each key
                    redisReply *valueReply = redisCommand(c, "GET %s", reply->element[i]->str);
                    if (valueReply != NULL) {
                        printf("Value: %s\n", valueReply->str);
                        freeReplyObject(valueReply);
                    }
                }
            }
            freeReplyObject(reply);
            redisFree(c);
            end = clock();
            sum = (double)(end-begin)/CLOCKS_PER_SEC;
            printf("'redis' read for all %d pairs: %f s\n",M,sum);
            printf("'redis' read for 1 pairs: %f s\n",sum/M);
        }

        //end all
        else if(strcmp(input,off)==0){
            break;
        }
    }
	system("pause");
	return 0;
}