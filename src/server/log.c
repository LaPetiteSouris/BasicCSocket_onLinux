#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include <time.h>
//Function log, store all log records into a logfile
void log_server(char  log[])
{
   FILE *fp;
   int i = 11;

   /* open the file */
   fp = fopen("logfile", "a");
   if (fp == NULL) {
      printf("I couldn't open logfile for appending.\n");
      exit(0);
   }

   /* write to the file */
   time_t rawtime;
   struct tm * timeinfo;

   time ( &rawtime );
   timeinfo = localtime ( &rawtime );
   //Append time info in the beginning of a line
   fprintf(fp, "%s", asctime(timeinfo));
   //Add lo
   fprintf(fp, "%s\n", log);

   /* close the file */
   fclose(fp);
}
