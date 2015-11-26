#include <stdio.h>
#include <stdlib.h>
#include "log.h"
void log_server(char * log)
{
   FILE *fp;
   int i = 11;

   /* open the file */
   fp = fopen("logfile","a");
   if (fp == NULL) {
      printf("I couldn't open logfile for appending.\n");
      exit(0);
   }

   /* write to the file */
   fprintf(fp, "%s\n", log);

   /* close the file */
   fclose(fp);
}