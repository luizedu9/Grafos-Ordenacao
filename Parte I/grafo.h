/*/
// * File:      grafo.h
// * Nome:      Luiz Eduardo Pereira
// * Matrícula: 0021619
// * Data:      19/06/2016
*/
 
#ifndef GRAFO_H
#define GRAFO_H

typedef struct grafo *Grafo;

typedef enum {
    FALSE, TRUE
} boolean;

/*2.2.1*/
Grafo GGcriaGrafo(int V, int A);
Grafo GGdestroiGrafo(Grafo G);
int GVcriaVertice(Grafo G);
int GAcriaAresta(Grafo G, int v1, int v2);
boolean GBexisteIdVertice(Grafo G, int v);
boolean GBexisteIdAresta(Grafo G, int v);
boolean GBexisteArestaDir(Grafo G, int v1, int v2);
boolean GBexisteAresta(Grafo G, int v1, int v2);
int GApegaArestaDir(Grafo G, int v1, int v2);
int GApegaAresta(Grafo G, int v1, int v2);
int GVprimeiroVertice(Grafo G);
int GVproximoVertice(Grafo G, int v1);
int GAprimeiraAresta(Grafo G);
int GAproximaAresta(Grafo G, int a1);
int GInumeroVertices(Grafo G);
int GInumeroVerticesMax(Grafo G);
int GInumeroArestas(Grafo G);
int GInumeroVerticesMax (Grafo G);
Grafo GGcarregaGrafo(char *f);
boolean GBsalvaGrafo(Grafo G, char *f);

/*2.2.2*/
int GIpegaGrau(Grafo G, int v);
int GAprimaAresta(Grafo G, int v);
int GAproxAresta(Grafo G, int v, int a1);
int GAprimaEntrada(Grafo G, int v);
int GAproxEntrada(Grafo G, int v, int a1);
int GAprimaSaida(Grafo G, int v);
int GAproxSaida(Grafo G, int v, int a1);

/*2.2.3*/
boolean GBarestaLaco(Grafo G, int a);
int GValfa(Grafo G, int a);
int GVomega(Grafo G, int a);
int GVvizinho(Grafo G, int a, int v1);

#endif /* GRAFO_H */

