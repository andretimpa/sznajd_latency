#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

#define NOME_ARQ argv[1]
#define N_STR    argv[2]
#define m_STR    argv[3]
#define ARGS     4

int main(int argc, char *argv[]){
   if(argc != ARGS){
       printf("Uso: <nome_arq> <N> <m>\n\n");
       exit(0);
   }
   int N = atoi(N_STR);
   int m = atoi(m_STR);
   escreve(BA(N, m), NOME_ARQ);
   return 0;
}

