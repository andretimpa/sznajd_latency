#ifndef _BIBLIOTECA_H_
#define _BIBLIOTECA_H_

struct aresta {
  int k, l;
};
typedef struct aresta aresta;
typedef aresta *Aresta;

struct rede_arestas{
  int    N, E;
  Aresta conexao;
};
typedef struct rede_arestas rede_arestas;

rede_arestas  BA                     (int N, int m);
/*create BA network with N vertices and minimum connectivity m*/
void          escreve                (rede_arestas grafo, char *arq);
/*writes a graph in an edge representation to a file*/
void          sznajd_in              (const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq, double p); /*sznajd inflow simulation*/
void          sznajd_out             (const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq, double p); /*sznajd outflow simulation*/
void 	      votante                (const char *arquivo, int mudanca_opiniao, const char *saida, int step, const char *arq, double p); /*voter simulation*/
#endif

