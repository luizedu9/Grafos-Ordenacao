/*
 * File:      busca.c
 * Nome:      Luiz Eduardo Pereira
 * Matrícula: 0021619
 * Data:      09/07/2016
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct lista { /*A unica diferença entre fila e pilha é na função retira;*/
    int n;
    struct nodo *inicio;
    struct nodo *final;
}*Lista;

typedef struct nodo {
    struct nodo *proximo;
    int vertice;
}Nodo;

static Lista cria();
static void insere(Lista lista, int vertice);
static int retiraFila(Lista fila);
static int retiraPilha(Lista pilha);
static Lista destroi(Lista lista);
static boolean vazia(Lista lista);

static boolean existeCinza(Grafo G, int *vertice);
static int arestaVisitada(Grafo G, boolean *aresta, int v);
static int escolheGrau(Grafo G);
static int sementeVertice(Grafo G);

void buscaLargura(Grafo G, int partida) {
    Lista fila;
    int *cor; /*0 - Branco / 1 - Cinza / 2 - Preto;*/
    boolean *flagA;
    int i, v, a, w, quantidadeV, quantidadeA;
    
    quantidadeV = GInumeroVertices(G);
    quantidadeA = GInumeroArestas(G);
    cor = (int*) malloc((quantidadeV + 1) * sizeof(int));
    flagA = (boolean*) malloc((quantidadeA + 1) * sizeof(boolean));
    for (i = 1; i <= quantidadeV; i++)
        cor[i] = 0;
    for (i = 1; i <= quantidadeV; i++)
        flagA[i] = FALSE;
    
    fila = cria();
    v = partida;
    cor[v] = 1;
    insere(fila, v);
    
    while (existeCinza(G, cor)) {
        v = retiraFila(fila);       
        a = arestaVisitada(G, flagA, v);
        while (a != 0) {            
            flagA[a] = TRUE;
            w = GVvizinho(G, a, v);
            if (cor[w] == 0) {
                cor[w] = 1;
                insere(fila, w);
            }
            a = arestaVisitada(G, flagA, v);
        }
        cor[v] = 2;
    }
    destroi(fila);
    return;
}

void buscaProfundidade(Grafo G, int partida) {
    Lista pilha;
    int *cor; /*0 - Branco / 1 - Cinza / 2 - Preto;*/
    boolean *flagA;
    int i, v, a, w, quantidadeV, quantidadeA;
    
    quantidadeV = GInumeroVertices(G);
    quantidadeA = GInumeroArestas(G);
    cor = (int*) malloc((quantidadeV + 1) * sizeof(int));
    flagA = (boolean*) malloc((quantidadeA + 1) * sizeof(boolean));
    for (i = 1; i <= quantidadeV; i++)
        cor[i] = 0;
    for (i = 1; i <= quantidadeV; i++)
        flagA[i] = FALSE;
    
    pilha = cria();
    v = partida;
    cor[v] = 1;
    insere(pilha, v);
    
    while (existeCinza(G, cor)) {
        v = retiraPilha(pilha);    
        a = arestaVisitada(G, flagA, v);
        while (a != 0) {            
            flagA[a] = TRUE;
            w = GVvizinho(G, a, v);
            if (cor[w] == 0) {
                cor[w] = 1;
                insere(pilha, w);
            }
            a = arestaVisitada(G, flagA, v);
        }
        cor[v] = 2;
    }
    destroi(pilha);
    return;
}

void coloracaoGulosa(Grafo G, int k) {
    Grafo GCopia;
    Lista pilha;
    int *cor;
    int i, v, w, quantidadeV, corV, semente;
    boolean corDisponivel[k + 1];
    
    GBsalvaGrafo(G, "CopiaGrafo.txt");
    GCopia = GGcarregaGrafo("CopiaGrafo.txt");
    pilha = cria();
    quantidadeV = GInumeroVertices(G);
    semente = sementeVertice(G); /*Devolve o id do ultimo vertice;*/
    cor = (int*) malloc((semente + 1) * sizeof(int));
    for (i = 1; i <= quantidadeV; i++)
        cor[i] = 0;
    for (i = 1; i <= k; i++)
        corDisponivel[i] = TRUE;
    
    w = escolheGrau(GCopia);
    while (w != 0) {
        GVdestroiVertice(GCopia, w);
        insere(pilha, w);
        w = escolheGrau(GCopia);
    }
    
    while (!vazia(pilha)) {
        v = retiraPilha(pilha);
        i = GAprimaAresta(G, v); /*Verifica quais cores estão disponiveis para V;*/
        while (i != 0) {
            corV = GVvizinho(G, i, v);
            if (cor[corV] != 0)
                corDisponivel[(cor[corV])] = FALSE;
            i = GAproxAresta(G, v, i);
        }
        for (i = 1; i <= k; i++) { /*Colore com a primeira cor disponivel;*/
            if (corDisponivel[i] == TRUE) {
                cor[v] = i;               
                break;
            }
        }
        if (cor[v] == 0) {
            printf("Não existe solução.");
            return;
        }
        for (i = 1; i <= k; i++) /*Reinicia a variavel corDisponivel;*/
            corDisponivel[i] = TRUE;
    }
    
    v = GVprimeiroVertice(G);
    while(v != 0) {
        printf("%d - %d\n", v, cor[v]);
        v = GVproximoVertice(G, v);
    }
    destroi(pilha);
    return;
}

boolean existeCinza(Grafo G, int *vertice) {
    int i;
    for (i = 1; i <= GInumeroVertices(G); i++) {
        if (vertice[i] == 1)
            return(TRUE);
    }
    return(FALSE);
}

static int arestaVisitada(Grafo G, boolean *aresta, int v) {
    int i, idAresta;
    idAresta = GAprimaAresta(G, v);
    for (i = 1; i <= GIpegaGrau(G, v); i++) {
        if (aresta[idAresta] == FALSE)
            return(idAresta);
        idAresta = GAproxAresta(G, v, idAresta);
    }
    return(0);
}

static int escolheGrau(Grafo G) {
    int menor, atual, idmenor, comparacao;
    menor = INT_MAX;
    idmenor = 0;
    if (GInumeroVertices(G) == 0)
        return(idmenor);
    atual = GVprimeiroVertice(G);
    while (atual != 0) { /*Encontra o menor Grau;*/
        comparacao = GIpegaGrau(G, atual);
        if ((comparacao < menor)) {
            menor = comparacao;
            idmenor = atual;
        }
        atual = GVproximoVertice(G, atual);
    }
    return(idmenor);
}

static int sementeVertice(Grafo G) {
    int i, semente; /*Semente é id do ultimo vertice;*/
    semente = GVprimeiroVertice(G);
    for (i = 0; i < (GInumeroVertices(G) - 1); i++) {
        semente = GVproximoVertice(G, semente);
    }
    return(semente);
}

static Lista cria() {
    Lista lista;
    lista = (Lista) malloc(sizeof(struct lista));
    lista->n = 0;
    lista->inicio = NULL;
    lista->final = NULL;
    return(lista);
}

static void insere(Lista lista, int vertice) {
    Nodo *nodo;
    nodo = (Nodo*) malloc(sizeof(Nodo));
    lista->n++;
    nodo->vertice = vertice;
    nodo->proximo = NULL;
    if (lista->n == 1) { /*Caso inicial - lista vazia;*/
        lista->inicio = nodo;
        lista->final = nodo;
        return;
    }
    lista->final->proximo = nodo;
    lista->final = nodo;
    return;
}

static int retiraFila(Lista fila) {
    int id;
    Nodo *aux;
    fila->n--;
    id = fila->inicio->vertice;
    aux = fila->inicio;
    fila->inicio = fila->inicio->proximo;
    free(aux);
    return(id);
}

static int retiraPilha(Lista pilha) {
    int id;
    Nodo *aux1, *aux2;
    pilha->n--;
    id = pilha->final->vertice;
    if (pilha->n == 0) { /*Caso especiao - Pilha com um unico elemento;*/
        free(pilha->inicio);
        pilha->inicio = NULL;
        pilha->final = NULL;
        return(id);
    }
    aux1 = pilha->final;
    aux2 = pilha->inicio;
    while (aux2->proximo != aux1)
        aux2 = aux2->proximo;
    aux2->proximo = NULL;
    pilha->final = aux2;
    free(aux1);
    return(id);
}

static Lista destroi(Lista lista) {
    Nodo *aux1, *aux2;
    aux1 = lista->inicio;
    while (aux1 != NULL) {
        aux2 = aux1->proximo;
        free(aux1);
        aux1 = aux2;
    }
    free(lista);
    lista = NULL;
    return(lista);
}

static boolean vazia(Lista lista) {
    if (lista->inicio == NULL)
        return(TRUE);
    return(FALSE);
}