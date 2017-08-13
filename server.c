#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "snappy.h"

#define SHMSZ  1000000

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

struct req_msg_text
{
    int qid;
    long file_size;
};

struct request_msg
{
    long message_type;
    struct req_msg_text req_text;
};

    union semun { 
        int val;              /* used for SETVAL only */ 
        struct semid_ds *buf; /* for IPC_STAT and IPC_SET */ 
        ushort *array;        /* used for GETALL and SETALL */ 
    }; 

int main(int argc, char *argv[])
{
    char c;
    int shmid,server_qid,semid;
    key_t key_shm,key_sem,key_msg;
    char *shm, *s;
    struct request_msg request;
    struct reply_msg response;
    union semun arg;
    struct sembuf sb = {0, -1, 0};
    long file_size;
    size_t mem_size=SHMSZ;
    if (argc == 2) mem_size = atoi(argv[1]);
printf("Shared memory size is %ld\n",mem_size);

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key_shm = 1111;
    key_sem = 2222;
    key_msg = 3333;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key_shm, mem_size, IPC_CREAT | 0666)) < 0) {
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

    if ((server_qid = msgget (key_msg, IPC_CREAT | 0666)) == -1) 
    {
        perror ("msgget");
        exit (1);
    }

    if ((semid = semget(key_sem, 1, 0666 | IPC_CREAT)) == -1)
    { 
        perror("semget"); 
        exit(1); 
    }
    arg.val = 1; 
    if (semctl(semid, 0, SETVAL, arg) == -1)
    { 
        perror("semctl"); 
        exit(1); 
    }



    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    /*s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;*/

//////MANYU
    while(1)
    {
        
        if (msgrcv (server_qid, &request, sizeof (struct req_msg_text), 0, 0) == -1)
        {
            perror ("msgrcv");
            exit (1);
        }
        printf("Message received and file size is %ld\n",request.req_text.file_size);


// Send shared memory size
int client_qid = request.req_text.qid;
response.reply_text.flag = mem_size;
        response.message_type = 1;
        response.reply_text.qid = server_qid;


if (msgsnd (client_qid, &response, sizeof (struct reply_msg_text), 0) == -1)
        {
            perror ("msgsnd");
            exit (1);
        }


        file_size = request.req_text.file_size;
for(int i=0;i<file_size;i++) *(shm+i)='4';
        *(shm+file_size-1) = 1;
*(shm+file_size+2)='a';
*(shm+file_size) = 0;
        response.reply_text.flag = 1;
        response.message_type = 1;
        client_qid = request.req_text.qid;
        response.reply_text.qid = server_qid;
printf("sever qid is %d\n",server_qid);
        if (msgsnd (client_qid, &response, sizeof (struct reply_msg_text), 0) == -1)
        {
            perror ("msgsnd");
            exit (1);
        }
        response.reply_text.flag = 0;

        while(*(shm+file_size-1) != 0);
while(*(shm+file_size) != 1);
        /*if (semop(semid, &sb, 1) == -1) { 
            perror("semop"); 
            exit(1); 
        }*/
        
        printf("Server got sem\n");

     char *outbuffer = (char*) malloc(file_size*sizeof(char));
  long outlen = file_size;
  char* buffer = (char*) malloc(file_size*sizeof(char));
  //char* buffer = calloc( 1, file_size );
  for(int i=0;i<file_size;i++) *(buffer+i)=*(shm+i);
  
struct snappy_env snappy_var;
  int t1 = snappy_init_env(&snappy_var);
   if (t1 ==0)
     {
printf("Manyu1 %ld\n",file_size);
       int temp = snappy_compress(&snappy_var, shm, file_size, outbuffer, &outlen);
printf("Manyu2\n");
       if (temp == 0)
         {
           printf("input len, Compressed length: %ld, %ld\n", file_size, outlen);
         }

       s=shm;
       int i=0;
       for (i = 0; i< outlen; i++)
       *s++ = outbuffer[i];
     response.reply_text.flag = outlen;
     response.message_type = 1;
     response.reply_text.qid = server_qid;

//sb.sem_op = 1; /* free resource */ 
        /*if (semop(semid, &sb, 1) == -1) { 
            perror("semop"); 
            exit(1); 
        }*/
*(shm+outlen+1) = 'a';
        if (msgsnd (client_qid, &response, sizeof (struct reply_msg_text), 0) == -1)
        {
            perror ("msgsnd");
            exit (1);
        }
      

     printf("Uncompressed file length: %ld \n", outlen);

     

     }
free(buffer);
   snappy_free_env(&snappy_var);
while(*(shm+outlen+1)!='b');
while(*(shm+file_size+2)!='b');
        //for(int i=0;i<5;i++) *(shm+i)='a';
        //sleep(1);
        //*(shm+file_size)=0;
    }

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    exit(0);
return 0;
}
