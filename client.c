/*
 * shm-client - client program to demonstrate shared memory.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SHMSZ     1000000 

struct reply_msg_text
{
    int qid;
    int flag;
};

struct reply_msg
{
    long message_type;
    struct reply_msg_text reply_text;
};

struct arg
{
    long lSize;
    long* opsize;
    char* buffer;
    char* opbuffer;
    int num;
    int* opdone;
};

struct req_msg_text
{
    int qid;
    long file_size;
};

struct req_msg
{
    long message_type;
    struct req_msg_text req_text;
};

void sync_compress(char *buffer, long lSize, char* opbuffer, long* opsize)
{
    int shmid,semid;
    key_t key_shm,key_sem,key_msg;
    int server_qid,myqid;
    char *shm, *s;
    int i;
    int mem_size = SHMSZ;
    struct sembuf sb = {0, -1, 0};

    struct req_msg request;
    struct reply_msg response;
    key_shm = 1111;
    key_sem = 2222;
    key_msg = 3333;


    if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1)
    {
        perror ("msgget: myqid");
        exit (1);
    }

    if ((server_qid = msgget (key_msg, 0)) == -1)
    {
        perror ("msgget: server_qid");
        exit (1);
    }

    if ((semid = semget(key_sem, 1, 0)) == -1)
    { 
        perror("semget"); 
        exit(1); 
    } 

    request.message_type = 1;
    request.req_text.qid = myqid;
    request.req_text.file_size = lSize+1;

    if (msgsnd (server_qid, &request, sizeof (struct req_msg_text), 0) == -1)
    {
        perror ("client: msgsnd");
        exit (1);
    } 

    if (msgrcv (myqid, &response, sizeof (struct reply_msg_text), 0, 0) == -1)
    {
        perror ("client: msgrcv");
        exit (1);
    }

    mem_size = response.reply_text.flag;
    printf("Shared memory size is %ld\n",mem_size);

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key_shm, mem_size, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Client 1\n");
    response.reply_text.flag = 0;
    // read response from server
    if (msgrcv (myqid, &response, sizeof (struct reply_msg_text), 0, 0) == -1)
    {
        perror ("client: msgrcv");
        exit (1);
    }
    printf("Client 2\n");
    if(response.reply_text.flag)
        printf("Client : Server ready to take the input\n");

    printf("Client got sem\n");
    for(i=0;i<lSize+1;i++) *(shm+i)=buffer[i];
    *(shm+lSize) = 0;
    *(shm+lSize+1) = 1;
    printf("wait for server to release sem\n");
    response.reply_text.flag = 0;
    // read response from server 
    if (msgrcv (myqid, &response, sizeof (struct reply_msg_text), 0, 0) == -1)
    {
        perror ("client: msgrcv");
        exit (1);
    }

    if(response.reply_text.flag > 0)
        printf("Client : Server has placed the compressed file in the shared memory and size is %d\n",response.reply_text.flag);

    opbuffer = (char*)malloc(response.reply_text.flag*sizeof(char));
    s = shm;
    for (i = 0; i < response.reply_text.flag; i++)
      opbuffer[i]=*s++;
    *opsize = response.reply_text.flag;
    printf("Size of compressed file: %d \n", response.reply_text.flag);

    *(shm+response.reply_text.flag+1)='b';


    for(i=0;i<10;i++) printf("%c",shm[i]);
    printf("\n");

    if (msgctl (myqid, IPC_RMID, NULL) == -1) { perror ("client: msgctl"); exit (1); }
    *(shm+lSize+3)='b';
    return;
}

void *async_handler(void *v)
{
FILE *fp;
    struct arg *args = (struct arg*)v;
    printf("inside thread input size is %ld\n",args->lSize);
    sync_compress(args->buffer,args->lSize,args->opbuffer,args->opsize);
    printf("inside thread output size is %ld and number is %d\n",*(args->opsize), args->num);
    char opname[20];
    sprintf(opname,"output_%d.txt",args->num);

    fp = fopen(opname,"w");
    for(int i=0; i<*(args->opsize);i++)
    fprintf(fp,"%c",*(args->opbuffer+i));
    fclose(fp);
    *(args->opdone)=1;
    printf("inside thread done");
}

void async_compress(char *buffer, long lSize, char* opbuffer, long* opsize, int num, int* opdone)
{
    pthread_t handler_thread;
    struct arg *args = (struct arg*)malloc(sizeof(struct arg));
    args->lSize = lSize;
    args->buffer = buffer;
    args->opsize = opsize;
    args->opbuffer = opbuffer;
    args->num = num;
    args->opdone = opdone;
    printf("input size is %ld\n",lSize);
    pthread_create(&handler_thread,0,async_handler,(void *)args);
    return;
}
