#include "snappy.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
   struct snappy_env *snappy_var;
   snappy_var = (struct snappy_env*)malloc(sizeof(struct snappy_env));

   

   FILE *fp;
   long lSize;
   char *buffer;
   printf("Mounika\n");
   fp = fopen ( "sample.txt" , "rb" );
   if( !fp ) perror("textfile.txt"),exit(1);
 
   fseek( fp , 0L , SEEK_END);
   lSize = ftell( fp );
   rewind( fp );

/* allocate memory for entire content */
   buffer = calloc( 1, lSize+1 );
   if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
   if( 1!=fread( buffer , lSize, 1 , fp) )
  fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

/* do your work here, buffer is a string contains the whole text */

  fclose(fp);
  
  char *outbuffer;
  long outlen;
  printf("Mounika2\n");
  int t1 = snappy_init_env(snappy_var);
   if (t1 ==0)
     {printf("Mounika3\n");
       int temp = snappy_compress(snappy_var, buffer, lSize,  outbuffer, &outlen);
       printf("Mounika4\n");
int i=0;
       while(i<outlen) printf("%c",outbuffer[i++]);
       if (temp == 0)
         {
           printf("\ninput len, Compressed length: %ld, %ld\n", lSize, outlen);
         }

snappy_free_env(snappy_var);

     }

   free(buffer);

    
long outlen2;
    snappy_uncompressed_length(outbuffer,outlen,&outlen2);
    printf("Mounikaaaaaaaaaaaaaaa %ld\n",outlen2);
char *ob2;
    //int temp2 = snappy_uncompress(outbuffer, outlen,  ob2);
int i2=0;
    //while(i2<lSize) printf("%c",outbuffer2[i2++]);


}
