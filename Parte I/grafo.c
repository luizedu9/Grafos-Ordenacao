/*
 * File:      grafo.c
 * Nome:      Luiz Eduardo Pereira
 * Matrícula: 0021619
 * Data:      19/06/2016
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct aresta {
    int v1;
    int v2;
}Aresta;

struct grafo {
    int *vertice; /*Vetor de Vertices;*/
    Aresta *aresta; /*Vetor de Arestas [V1 | V2 | Peso];*/
    int *aux; /*Vetor com informações auxiliares;*/
};


/*2.2.1*/

Grafo GGcriaGrafo(int V, int A) {
    if (V == 0 || A == 0)
        return(NULL);
    int i;
    Grafo G; /*G - ponteiro para Grafo;*/
    G = (struct grafo*) malloc(sizeof(struct grafo));
    G->vertice = (int*) malloc(V * sizeof(int));
    G->aresta = (Aresta*) malloc(A * sizeof(Aresta));
    G->aux = (int*) malloc(2 * sizeof(int));
    G->aux[0] = V; /*Número maximo de vertices;*/
    G->aux[1] = A; /*Número maximo de arestas;*/
    for (i = 0; i < V; i++)
        G->vertice[i] = 0;
    for (i = 0; i < A; i++) {
        G->aresta[i].v1 = 0;
        G->aresta[i].v2 = 0;
    }
    return(G);
}

Grafo GGdestroiGrafo(Grafo G) {
    free(G->aresta);
    free(G->vertice);
    free(G->aux);
    free(G);
    G = NULL;
    return(G);
}

int GVcriaVertice(Grafo G) {
    int i;
    for (i = 0; i < G->aux[0]; i++) { /*Enquanto não estourar o máximo de vertices;*/
        if (G->vertice[i] == 0) {     /*E a posição escolhida for fazia, crie um(apenas) novo vertice;*/
            G->vertice[i] = i + 1; /*Coloca número ao Vertice; (i = 0, Vertice = 1);*/
            return(i + 1);
        }
    }
    return(0);
}

int GAcriaAresta(Grafo G, int v1, int v2) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {  
        if (G->aresta[i].v1 == 0) {         
            G->aresta[i].v1 = v1;
            G->aresta[i].v2 = v2;
            return(i + 1);
        }
    }
    return(0);
}

boolean GBexisteIdVertice(Grafo G, int v) {
    if ((G->vertice[v - 1] > 0) && (v > 0) && (v < G->aux[0])) { /*Verifica se existe o Id v na posição v - 1;*/
        return(TRUE);
    }
    return(FALSE);
}

boolean GBexisteIdAresta(Grafo G, int a) {
    if ((G->aresta[a - 1].v1 != 0) && (a > 0) && (a < G->aux[1])) { 
        return(TRUE);
    }
    return(FALSE);
}

boolean GBexisteArestaDir(Grafo G, int v1, int v2) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if ((G->aresta[i].v1 == v1) && (G->aresta[i].v2 == v2)) { /*Busca por aresta igual a (v1,v2);*/
            return(TRUE);
        }
    }
    return(FALSE);
}

boolean GBexisteAresta(Grafo G, int v1, int v2) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if ((((G->aresta[i].v1 == v1) && (G->aresta[i].v2 == v2)) || ((G->aresta[i].v1 == v2) && (G->aresta[i].v2 == v1))) && ((v1 > 0) && (v2 > 0))) {
                return(TRUE);
        }
    }
    return(FALSE);
}

int GApegaArestaDir(Grafo G, int v1, int v2) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if ((G->aresta[i].v1 == v1) && (G->aresta[i].v2 == v2)) {
            return(i + 1); /*Id da aresta é igual a posição do vetor + 1;*/
        }
    }
    return(0);
}

int GApegaAresta(Grafo G, int v1, int v2) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if (((G->aresta[i].v1 == v1) && (G->aresta[i].v2 == v2)) || ((G->aresta[i].v1 == v2) && (G->aresta[i].v2 == v1))) {
            return(i + 1);
        }
    }
    return(0);
}

int GVprimeiroVertice(Grafo G) {
    int i;
    for (i = 0; i < G->aux[0]; i++) {
        if (G->vertice[i] != 0)
            return(G->vertice[i]);
    }
    return(0);
}

int GVproximoVertice(Grafo G, int v1) {
    int i;
    if (v1 > 0) {
        for (i = v1; i < G->aux[0]; i++) { /*Passa proximo Vertice, mesmo se vertice[v1] = 0;*/
            if (G->vertice[i] != 0)
                  return(G->vertice[i]);
        }
    }
    return(0);
}

int GAprimeiraAresta(Grafo G) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if (G->aresta[i].v1 != 0)
            return(i + 1);
    }
    return(0);
}

int GAproximaAresta(Grafo G, int a1) {
    int i;
    if (a1 > 0) {
        for (i = a1; i < G->aux[1]; i++) { /*Retorna proxima Aresta, mesmo se aresta[a1] = 0;*/
            if (G->aresta[i].v1 != 0)
                return(i + 1);
        }
    }
    return(0);
}

int GInumeroVertices(Grafo G) {
    int nv = 0, i;
    for (i = 0; i < G->aux[0]; i++){
        if (G->vertice[i] != 0) 
            nv++;
    }
    return(nv);        
}

int GInumeroVerticesMax(Grafo G) {
    return(G->aux[0]);
}

int GInumeroArestas(Grafo G) {
    int nv = 0, i;
    for (i = 0; i < G->aux[1]; i++){
        if (G->aresta[i].v1 != 0) 
            nv++;
    }
    return(nv);        
}

int GInumeroArestasMax(Grafo G) {
    return(G->aux[1]);
}

Grafo GGcarregaGrafo(char *f) {
    Grafo G;
    int i;
    FILE *file;
    file = fopen(f, "r"); /*Aberto em modo leitura*/
    if (file == NULL)
        return(0);
    int V = 0, A = 0;
    fscanf(file, "%d%d", &V, &A); /*Pega número de vertices e arestas*/
    G = GGcriaGrafo(V, A);
    for (i = 0; i < A; i++) {
        fscanf(file, "%d%d", &G->aresta[i].v1, &G->aresta[i].v2);
    }
    for (i = 0; i < V; i++) {
        G->vertice[i] = i + 1;
    }
    G->aux[0] = V;
    G->aux[1] = A;
    return(G);
}

boolean GBsalvaGrafo(Grafo G, char *f) {
    FILE *file;
    int V, A, Au,i;
    file = fopen(f, "wt");
    V = GInumeroVerticesMax(G);
    A = GInumeroArestasMax(G);
    Au = GInumeroArestas(G); /*Atestas em uso*/
    fprintf(file, "%d %d\n", V, A); /*Quantidade de vertice e aresta;*/
    for (i = 0; i < Au; i++) {
        if (G->aresta[i].v1 != 0)
            fprintf(file, "%d %d\n", G->aresta[i].v1, G->aresta[i].v2); /*Imprime Relaçoes;*/
    }
    if (file == NULL) /*Verifica se não foi salvo com sucesso;*/
        return(FALSE);
    return(TRUE);    
}


/*2.2.2*/

int GIpegaGrau(Grafo G, int v) {
    int Grau = 0, i;
    boolean j;
    j = GBexisteIdVertice(G, v);
    if (j == TRUE) {
        i = GAprimaEntrada(G, v);
        while (i != 0) { /*Enquanto existir arestas;*/
            Grau++;
            i = GAproxEntrada(G, v, i);
        }
	i = GAprimaSaida(G,v);
	while (i != 0) {
            Grau++;
            i = GAproxSaida(G, v, i);
        }
    }
    return(Grau);
}

int GAprimaAresta(Grafo G, int v) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if (G->aresta[i].v1 == v || G->aresta[i].v2 == v)
            return(i + 1);
    }
    return(0);
}

int GAproxAresta(Grafo G, int v, int a1) {
    int i;
    if ((G->aresta[a1 - 1].v1 == v) || (G->aresta[a1 - 1].v2 == v)) { /*Se aresta escolhida pertence ao vertice;*/
        for (i = a1; i < G->aux[1]; i++) {
            if (G->aresta[i].v1 == v || G->aresta[i].v2 == v)
                return(i + 1);
        }  
    }
    return(0);
}

int GAprimaEntrada(Grafo G, int v) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if (G->aresta[i].v2 == v)
            return(i + 1);
    }
    return(0);
}

int GAproxEntrada(Grafo G, int v, int a1) {
    int i;
    if ((G->aresta[a1 - 1].v1 == v) || (G->aresta[a1 - 1].v2 == v)) {
        for (i = a1; i < G->aux[1]; i++) {
            if (G->aresta[i].v2 == v)
                return(i + 1);
        }
    }
    return(0);
}

int GAprimaSaida(Grafo G, int v) {
    int i;
    for (i = 0; i < G->aux[1]; i++) {
        if (G->aresta[i].v1 == v)
            return(i + 1);
    }
    return(0);
}

int GAproxSaida(Grafo G, int v, int a1) {
    int i;
    if ((G->aresta[a1 - 1].v1 == v) || (G->aresta[a1 - 1].v2 == v)) {
        for (i = a1; i < G->aux[1]; i++) {
            if (G->aresta[i].v1 == v)
                return(i + 1);
        }
    }
    return(0);
}


/*2.2.3*/

boolean GBarestaLaco(Grafo G, int a) {
    if (G->aresta[a - 1].v1 == G->aresta[a - 1].v2)
        return(TRUE);
    return(FALSE);
}

int GValfa(Grafo G, int a) { /*Vertice de partida;*/
    if ((G->aresta[a - 1].v1 != 0) && (a > 0))
        return(G->aresta[a - 1].v1);
    return(0);
}

int GVomega(Grafo G, int a) { /*Vertice de destino*/
    if ((G->aresta[a - 1].v1 != 0) && (a > 0))
        return(G->aresta[a - 1].v2);
    return(0);
}

int GVvizinho(Grafo G, int a, int v1) {
    if ((G->aresta[a - 1].v1 != 0) && ((a > 0) && (v1 > 0))) {
        if (G->aresta[a - 1].v1 == v1)
            return(G->aresta[a - 1].v2);
        if (G->aresta[a - 1].v2 == v1)
            return(G->aresta[a - 1].v1);
    }
    return(0);
}
