#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

#define ARQ_GEO  argv[1]
#define STEPS    argv[2]
#define JUMP     argv[3]
#define ARG_P    argv[4]
#define INITIAL  argv[5]
#define OUTPUT   argv[6]
#define ARGS     7

int main(int argc, char *argv[]){
   if(argc != ARGS){
       printf("Uso: <arq_geo (str)> <steps (int)> <jump (int)> <p (float)> <arq_initial (str)> <arq_out (str)>\n\n");
       exit(0);
   }
   sznajd_out(ARQ_GEO, atoi(STEPS), OUTPUT, atoi(JUMP), INITIAL, atof(ARG_P));
   return 0;
}


