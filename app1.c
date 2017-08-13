#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <time.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    long lSize, opsize;
    char *buffer, *opbuffer;

    int nooffiles = argc-1;
#ifdef ASYNC
    FILE *fp2[100];
    long lSize2[100], opsize2[100];
    char *buffer2[100], *opbuffer2[100];
    int* opdone = (int*)malloc(nooffiles*sizeof(int));
    for(int j=1; j<argc;j++) opdone[j-1]=0;
#endif
    for(int j=1; j<argc;j++)
    {
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
#ifndef ASYNC// to test ASYNC
	printf("Time before call is ");
	time_t time1 = time(0);
	printf(ctime(&time1));
	sync_compress(buffer,lSize,opbuffer,&opsize);
	time_t time1a = time(0);
	printf("Time after call is ");
	printf(ctime(&time1a));

	printf("opsize is %ld\n",opsize);

	char opname[30];
	sprintf(opname,"output_%dsync.txt",j);

	fp = fopen(opname,"w");
	for(int i=0; i<opsize;i++)
    	    fprintf(fp,"%c",*(opbuffer+i));
	    fclose(fp);
#endif
	//Asynchronous compression
#ifdef ASYNC// to test ASYNC
	opsize2[j-1] = 0;

	fp2[j-1] = fopen ( argv[j] , "rb" );
	if( !fp2[j-1] ) perror("blah.txt"),exit(1);
	printf("start1\n");
	fseek( fp2[j-1] , 0L , SEEK_END);
	lSize2[j-1] = ftell( fp2[j-1] );
	rewind( fp2[j-1] );
	buffer2[j-1] = (char*) calloc(1,(lSize2[j-1]+1)*sizeof(char));
	fread( buffer2[j-1] , lSize2[j-1], 1 , fp2[j-1]);
	fclose(fp2[j-1]);
	printf("Size of input file is %ld\n",lSize2[j-1]);

	printf("Time before call is ");
	time_t time2 = time(0);
	printf(ctime(&time2));
	async_compress(buffer2[j-1],lSize2[j-1],opbuffer2[j-1],&opsize2[j-1],j,opdone+j-1);
	time_t time2a = time(0);
	printf("Time after call is ");
	printf(ctime(&time2a));
	free(buffer2[j-1]);
#endif
    }
#ifdef ASYNC// to test ASYNC
    for(int j=1;j<argc;j++)
    {
	printf("spinning on %d\n",j-1);while(*(opdone+j-1) == 0);}
#endif
	exit(0);
    }
}
