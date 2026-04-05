#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "biblioteca.h"

#define TRUE    1
#define randf() (((double) rand())/RAND_MAX)

struct rede_vizinhos{
  int **grafo;
  int *numero_de_vizinhos;
  int numero_de_sitios;
};
typedef struct rede_vizinhos rede_vizinhos;

/**************************************************************/

rede_vizinhos ler(const char *arq){
  FILE *txt;
  int *vizinhos;
  int *viz_colocados;
  int **grafo;
  int N, M, k, l;
  rede_vizinhos output;

  txt = fopen(arq, "r");
  if (txt == NULL) {
    printf("Erro ao abrir %s.\n", arq);
    exit(1);
  }

  fscanf(txt, "%d", &N);

  vizinhos = (int *)malloc(N * sizeof(int));
  viz_colocados = (int *)malloc(N * sizeof(int));
  grafo = (int **)malloc(N * sizeof(int *));  

  // Alocação de 0 em viz_colocados
  for (int i = 0; i < N; i++) {
    viz_colocados[i] = 0;
  }


  for (int i = 0; i < N; i++) {
    fscanf(txt, "%d", &vizinhos[i]);

    grafo[i] = (int *)malloc(vizinhos[i] * sizeof(int));
  }

  fscanf(txt, "%d", &M);
  for (int j = 0; j < M; j++) {
    fscanf(txt, "%d", &k);
    fscanf(txt, "%d", &l);
    grafo[k][viz_colocados[k]] = l;
    viz_colocados[k] ++;
    grafo[l][viz_colocados[l]] = k;
    viz_colocados[l] ++;
  }
  fclose(txt);
  output.grafo = grafo;
  output.numero_de_vizinhos = vizinhos;
  output.numero_de_sitios = N;
  return output;
}

/**************************************************************/

rede_arestas BA(int N, int m){
  int novas_arestas, aresta_atual, i, j, k, aresta_aux, connect;
  int sorteioNBA, sorteio2;
  int A = novas_arestas = (N-m)*m + m*(m-1)/2;
  Aresta lista_de_arestas = (Aresta)malloc(novas_arestas * sizeof(aresta));
  rede_arestas grafo;
  int *sorteados;
  
  srand(time(NULL));
  
  sorteados = (int *)malloc(m*sizeof(int));
  for(aresta_atual = 0, i = 0; i <= m; i ++){
    for(j = i + 1; j <= m; j ++){
      lista_de_arestas[aresta_atual].k = i;
      lista_de_arestas[aresta_atual].l = j;
      aresta_atual ++;
    }
  } /*cria o nucleo da rede*/
  
  for(i = m+1; i < N; i++){ /*adicionando o i-esimo sitio a rede*/
    aresta_aux = aresta_atual;
    for(j = 0; j < m; ){  /*j-esima conexao desse novo sitio*/
      sorteioNBA = rand() % aresta_aux; 
      sorteio2 = rand() % 2;  /*sorteio via arestas*/

      if(sorteio2 == 0)
        connect = lista_de_arestas[sorteioNBA].k;
      else
        connect = lista_de_arestas[sorteioNBA].l;
      /*tenta conectar com connect*/
      for(k = 0; k < j; k ++){
        if(connect == sorteados[k]){
          break;
        }
      } /*checa se a conexao eh repetida*/
      if(k == j){
        lista_de_arestas[aresta_atual].k = i;
        lista_de_arestas[aresta_atual].l = connect;
        sorteados[j] = connect;
        aresta_atual ++;
        j ++;
      } /*se nao for procede*/
    }
  }
  grafo.N = N;
  grafo.E = A;
  grafo.conexao = lista_de_arestas;
  return grafo;
}

/**************************************************************/

void escreve (rede_arestas grafo, char *arq){
  FILE *saida;
  int *vizinhos;
  int k, l;
  int numero_de_sitios  = grafo.N;
  int numero_de_arestas = grafo.E;
  Aresta rede           = grafo.conexao;
  
  saida    = fopen(arq, "w");
  vizinhos = (int *) malloc (numero_de_sitios*sizeof(int));
  for(int i = 0; i < numero_de_sitios; i ++)
    vizinhos[i] = 0;
  for(int i = 0; i < numero_de_arestas; i ++){
    k = rede[i].k;
    l = rede[i].l;
    vizinhos[k] ++;
    vizinhos[l] ++;
  }
  fprintf(saida, "%d\n\n", numero_de_sitios);
  for(int i = 0; i < numero_de_sitios; i ++)
    fprintf(saida, "%d\t", vizinhos[i]);
  fprintf(saida, "\n\n%d\n\n", numero_de_arestas);
  for(int i = 0; i < numero_de_arestas; i ++){
    fprintf(saida, "%d\t%d\n", rede[i].k, rede[i].l);
  }
  fclose(saida);
  free(vizinhos);
  return;
}

/**************************************************************/

void sznajd_in(const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq,  double p) {
    int *vizinhos;
    int i, j, k, l;
    int *opiniao;
    int n_opinioes; 
    double *probabilidades, rn;
    rede_vizinhos rede = ler(arquivo);
    int numero_de_sitios  = rede.numero_de_sitios;
    int *ativo = (int *)malloc(numero_de_sitios * sizeof(int));
    srand(time(NULL));

    vizinhos = rede.numero_de_vizinhos;
    int **grafo = rede.grafo;

    FILE* porcentagem;
    porcentagem = fopen(arq, "r");
    if (porcentagem == NULL) {
        printf("Erro ao abrir %s.\n", arq);
        exit(1);
    }
    fscanf(porcentagem, "%d", &n_opinioes);
    probabilidades = (double*)calloc(n_opinioes, sizeof(double));
    opiniao = (int *)malloc(numero_de_sitios * sizeof(int));

    for (int i = 0; i < n_opinioes; i++) {
        fscanf(porcentagem, "%lf", &probabilidades[i]);
    }
    fclose(porcentagem);
    
    FILE *nova_saida = fopen(saida, "w");
    if (nova_saida == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
  
    int *contagem_ativo, *contagem_inativo;
    contagem_ativo   = (int*)calloc(n_opinioes, sizeof(int));
    contagem_inativo = (int*)calloc(n_opinioes, sizeof(int));
    
    for (int i = 0; i < numero_de_sitios; i++) {
        ativo[i] = 1;
        rn = randf(); 
        double conta_porcentagem = 0;

        for (int j = 0; j < n_opinioes; j++) {
            conta_porcentagem += probabilidades[j]; 
            if (rn <= conta_porcentagem) {
                opiniao[i] = j; 
                break;
            }
        }
    }
    
    for (int i = 0; i < mudanca_opiniao; i++) {
        int a = rand() % numero_de_sitios;
        if (ativo[a]) {
            int vizinho = rand() % vizinhos[a];
            int b = grafo[a][vizinho];
            vizinho = rand() % vizinhos[b];
            int c = grafo[b][vizinho];
            /*sorteia os sitios*/
            if (opiniao[b] == opiniao[c] && opiniao[a] != opiniao[b]) {
                opiniao[a] = opiniao[c];
                ativo[a] = 0;
            }
        }
        else{
            if (rand() < p*RAND_MAX){
                ativo[a] = 1;
            }
        }
        /*passo do sznajd*/
        if(i % step == 0){
            fprintf(nova_saida,"%d:\t ", i);
            for (int j = 0; j < n_opinioes; j++){
                contagem_ativo[j] = contagem_inativo[j] = 0;
            }
            for (int j = 0; j < numero_de_sitios; j++){
                if(ativo[j])
                    contagem_ativo[opiniao[j]] ++;
                else
                    contagem_inativo[opiniao[j]] ++;
            }
            for (int j = 0; j < n_opinioes; j++){
                fprintf(nova_saida,"%d\t %d\t ", contagem_ativo[j], contagem_inativo[j]); 
            }
            fprintf(nova_saida,"\n");
        }
    }
    fclose(nova_saida);
    
    for (i = 0; i < numero_de_sitios; i++) {
        free(grafo[i]);
    }
    free(contagem_ativo);
    free(contagem_inativo);
    free(vizinhos);
    free(opiniao);
}

/**************************************************************/

void sznajd_out(const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq,  double p) {
    int *vizinhos;
    int i, j, k, l;
    int *opiniao;
    int n_opinioes; 
    double *probabilidades, rn;
    rede_vizinhos rede = ler(arquivo);
    int numero_de_sitios  = rede.numero_de_sitios;
    int *ativo = (int *)malloc(numero_de_sitios * sizeof(int));
    srand(time(NULL));

    vizinhos = rede.numero_de_vizinhos;
    int **grafo = rede.grafo;

    FILE* porcentagem;
    porcentagem = fopen(arq, "r");
    if (porcentagem == NULL) {
        printf("Erro ao abrir %s.\n", arq);
        exit(1);
    }
    fscanf(porcentagem, "%d", &n_opinioes);
    probabilidades = (double*)calloc(n_opinioes, sizeof(double));
    opiniao = (int *)malloc(numero_de_sitios * sizeof(int));

    for (int i = 0; i < n_opinioes; i++) {
        fscanf(porcentagem, "%lf", &probabilidades[i]);
    }
    fclose(porcentagem);
    
    FILE *nova_saida = fopen(saida, "w");
    if (nova_saida == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
  
    int *contagem_ativo, *contagem_inativo;
    contagem_ativo   = (int*)calloc(n_opinioes, sizeof(int));
    contagem_inativo = (int*)calloc(n_opinioes, sizeof(int));
    
    for (int i = 0; i < numero_de_sitios; i++) {
        ativo[i] = 1;
        rn = randf(); 
        double conta_porcentagem = 0;

        for (int j = 0; j < n_opinioes; j++) {
            conta_porcentagem += probabilidades[j]; 
            if (rn <= conta_porcentagem) {
                opiniao[i] = j; 
                break;
            }
        }
    }
    
    for (int i = 0; i < mudanca_opiniao; i++) {
        int a = rand() % numero_de_sitios;
        if (rand() < p*RAND_MAX) {
            ativo[a] = 1;
        }
        else{
            int vizinho = rand() % vizinhos[a];
            int b = grafo[a][vizinho];
            vizinho = rand() % vizinhos[b];
            int c = grafo[b][vizinho];
            /*sorteia os sitios*/
            if (opiniao[a] == opiniao[b] && opiniao[c] != opiniao[a] && ativo[c] == 1) {
                opiniao[c] = opiniao[a];
                ativo[c] = 0;
            }
        }
        /*passo do sznajd*/
        if(i % step == 0){
            fprintf(nova_saida,"%d:\t ", i);
            for (int j = 0; j < n_opinioes; j++){
                contagem_ativo[j] = contagem_inativo[j] = 0;
            }
            for (int j = 0; j < numero_de_sitios; j++){
                if(ativo[j])
                    contagem_ativo[opiniao[j]] ++;
                else
                    contagem_inativo[opiniao[j]] ++;
            }
            for (int j = 0; j < n_opinioes; j++){
                fprintf(nova_saida,"%d\t %d\t ", contagem_ativo[j], contagem_inativo[j]); 
            }
            fprintf(nova_saida,"\n");
        }
    }
    fclose(nova_saida);
    
    for (i = 0; i < numero_de_sitios; i++) {
        free(grafo[i]);
    }
    free(vizinhos);
    free(opiniao);
}

/**************************************************************/

void votante(const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq, double p) {
    int *vizinhos;
    int i, j, k, l;
    int *opiniao;
    int n_opinioes; 
    double *probabilidades, rn;
    rede_vizinhos rede = ler(arquivo);
    int numero_de_sitios  = rede.numero_de_sitios;
    int *ativo = (int *)malloc(numero_de_sitios * sizeof(int));
    srand(time(NULL));

    vizinhos = rede.numero_de_vizinhos;
    int **grafo = rede.grafo;

    FILE* porcentagem;
    porcentagem = fopen(arq, "r");
    if (porcentagem == NULL) {
        printf("Erro ao abrir %s.\n", arq);
        exit(1);
    }
    fscanf(porcentagem, "%d", &n_opinioes);
    probabilidades = (double*)calloc(n_opinioes, sizeof(double));
    opiniao = (int *)malloc(numero_de_sitios * sizeof(int));

    for (int i = 0; i < n_opinioes; i++) {
        fscanf(porcentagem, "%lf", &probabilidades[i]);
    }
    fclose(porcentagem);
    
    FILE *nova_saida = fopen(saida, "w");
    if (nova_saida == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
  
    int *contagem_ativo, *contagem_inativo;
    contagem_ativo   = (int*)calloc(n_opinioes, sizeof(int));
    contagem_inativo = (int*)calloc(n_opinioes, sizeof(int));

    for (int i = 0; i < numero_de_sitios; i++) {
	ativo[i] = 1;
        rn = randf(); 
        double conta_porcentagem = 0;

        for (int j = 0; j < n_opinioes; j++) {
            conta_porcentagem += probabilidades[j]; 
            if (rn <= conta_porcentagem) {
                opiniao[i] = j; 
                break;
            }
        }
    }


    for (int i = 0; i < mudanca_opiniao; i++) {
        int a = rand() % numero_de_sitios;
	if (ativo[a]) {
        	    int vizinho = rand() % vizinhos[a];
	    int b = grafo[a][vizinho];
	    if (opiniao[a] != opiniao[b]) {
		opiniao[a] = opiniao[b];
		ativo[a] = 0;
	    }
	}
	else{
      	    if (rand() < p*RAND_MAX){
       		ativo[a] = 1;
	    }
   	}
        /*passo do votante*/
        if(i % step == 0){
            fprintf(nova_saida,"%d:\t ", i);
            for (int j = 0; j < n_opinioes; j++){
                contagem_ativo[j] = contagem_inativo[j] = 0;
            }
            for (int j = 0; j < numero_de_sitios; j++){
                if(ativo[j])
                    contagem_ativo[opiniao[j]] ++;
                else
                    contagem_inativo[opiniao[j]] ++;
            }
            for (int j = 0; j < n_opinioes; j++){
                fprintf(nova_saida,"%d\t %d\t ", contagem_ativo[j], contagem_inativo[j]); 
            }
            fprintf(nova_saida,"\n");
        }
    }
    fclose(nova_saida);
    
    for (i = 0; i < numero_de_sitios; i++) {
        free(grafo[i]);
    }
    free(contagem_ativo);
    free(contagem_inativo);
    free(vizinhos);
    free(opiniao);
}


