#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <time.h>

int main(int argc, char *argv[])
{
    FILE *fp;
long lSize, opsize;
char *buffer, *opbuffer;

int *opdone = (int*)calloc(1,(argc-1)*sizeof(int));

long lSize2[argc-1],opsize2[argc-1];
FILE *fp2[argc-1];
char *buffer2[argc-1], *opbuffer2[argc-1];

for(int j=0; j<argc-1;j++)
opsize2[j] = 0;


for(int j=1; j<argc;j++)
{
/*
fp = fopen ( argv[j] , "rb" );
if( !fp ) perror("blah.txt"),exit(1);

fseek( fp , 0L , SEEK_END);
lSize = ftell( fp );
rewind( fp );
buffer = (char*) calloc(1,(lSize+1)*sizeof(char));
fread( buffer , lSize, 1 , fp);
fclose(fp);
printf("Size of input file is %ld\n",lSize);

//Synchronous compression

printf("Time before call is ");
time_t time1 = time(0);
printf(ctime(&time1));
sync_compress(buffer,lSize,opbuffer,&opsize);
time_t time1a = time(0);
printf("Time after call is ");
printf(ctime(&time1a));

printf("opsize is %ld\n",opsize);

char* opname;
sprintf(opname,"output_%d.txt",j);

fp = fopen(opname,"w");
for(int i=0; i<opsize;i++)
    fprintf(fp,"%c",*(opbuffer+i));
fclose(fp);
*/
//Asynchronous compression

printf("j is %d\n",j);
fp2[j-1] = fopen ( argv[j] , "rb" );
if( !fp2[j-1] ) perror("blah.txt"),exit(1);

fseek( fp2[j-1] , 0L , SEEK_END);
lSize2[j-1] = ftell( fp2[j-1] );
rewind( fp2[j-1] );
buffer2[j-1] = (char*) calloc(1,(lSize2[j-1]+1)*sizeof(char));
fread( buffer2[j-1] , lSize2[j], 1 , fp2[j-1]);
fclose(fp2[j-1]);
printf("Size of input file is %ld\n",lSize2[j-1]);


printf("Time before call is ");
time_t time2 = time(0);
printf(ctime(&time2));
async_compress(buffer2[j-1],lSize2[j-1],opbuffer2[j-1],&opsize2[j-1],j,&opdone[j-1]);
time_t time2a = time(0);
printf("Time after call is ");
printf(ctime(&time2a));
sleep(1);
/*
printf("opsize is %ld\n",opsize);
while(!opsize);
time_t time2b = time(0);
printf("Time after compression is ");
printf(ctime(&time2b));
printf("after compress, opsize is %ld\n",opsize);
*/
//free(buffer2[j-1]);
}

for(int j=0;j<argc-1;j++)
   while(opdone[j]==0);

     exit(0);
}
