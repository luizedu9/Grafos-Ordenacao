/*
 * File:      grafo.c
 * Nome:      Luiz Eduardo Pereira
 * Matrícula: 0021619
 * Data:      09/07/2016
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct vertice {
    int id;
    struct estrela *estrela;
    struct vertice *proximo;
}Vertice;

typedef struct aresta {
    int id;
    struct vertice *alfa; /*Vertice de partida;*/
    struct vertice *omega; /*Vertice de chegada;*/
    struct aresta *proximo;
}Aresta;    

typedef struct estrela {
    struct aresta *aresta;
    struct estrela *proximo;
}Estrela;

struct grafo {
    int nvertices, narestas; /*Número de V/A;*/
    int svertices, sarestas; /*Semente de V/A;*/
    Vertice *vinicio, *vatual; /*Aponta para primeiro e ultimo vertice;*/
    Aresta *ainicio, *aatual; /*Aponta para primeira e ultima aresta;*/
};

/*Funções auxiliares que não consta na especificação;*/
static Vertice* GPVencontraVertice(Grafo G, int v);
static Aresta* GPAencontraAresta(Grafo G, int a);
static void GEcriaEstrela(Grafo G, Vertice *vertice, Aresta *aresta);
static void GEdestroiEstrela(Grafo G, Vertice *vertice, int a);

/*2.2.1*/

Grafo GGcriaGrafo(void) {
    Grafo G; /*G - ponteiro para Grafo;*/
    G = (struct grafo*) malloc(sizeof(struct grafo));
    G->nvertices = 0;
    G->narestas = 0;
    G->svertices = 0;
    G->sarestas = 0;
    G->ainicio = NULL;
    G->vinicio = NULL;
    G->aatual = NULL;
    G->vatual = NULL;
    return(G);
}

Grafo GGdestroiGrafo(Grafo G) {
    if (G->vinicio != NULL)
        GVdestroiVertice(G, G->vinicio->id); /*A função destroiVertice também destroi as arestas;*/
    free(G);
    G = NULL;
    return(G);
}

int GVcriaVertice(Grafo G) {
    Vertice *nodo;
    if (G->nvertices == INT_MAX)
        return(0);
    nodo = (Vertice*) malloc(sizeof(Vertice));
    G->nvertices++; /*Número guarda a quantidade de vertices*/
    G->svertices++; /*Seed guarda o maior id de vertice*/
    nodo->estrela = NULL; /*Nenhum novo vertice possui aresta no inicio;*/
    nodo->proximo = NULL; /*Nenhum novo vertice aponta para alguem no inicio*/
    nodo->id = G->svertices;
    if (G->nvertices == 1) { /*Caso Inicial - Não existe nenhum vertice. (verifica se é igual a 1 pois ele ja foi adicionado no contador);*/
        G->vinicio = nodo;
        G->vatual = nodo;
        return(G->svertices);
    }
    G->vatual->proximo = nodo;
    G->vatual = nodo;
    return(G->svertices);
}

int GAcriaAresta(Grafo G, int v1, int v2) {
    Aresta *nodo;
    Vertice *vertice1, *vertice2;
    if (G->narestas == INT_MAX)
        return(0);
    nodo = (Aresta*) malloc(sizeof(Aresta));
    G->narestas++;
    G->sarestas++;
    /*Apontamentos de Nodo;*/
    nodo->proximo = NULL;
    nodo->id = G->sarestas;
    vertice1 = GPVencontraVertice(G, v1);
    vertice2 = GPVencontraVertice(G, v2);
    nodo->alfa = vertice1;
    nodo->omega = vertice2;
    /*Cria Estrela;*/
    GEcriaEstrela(G, vertice1, nodo);
    if (v1 != v2) /*Se for laço, cria apenas uma estrela;*/
        GEcriaEstrela(G, vertice2, nodo);
    /*Inserção de nodo na lista;*/
    if (G->narestas == 1) { /*Caso Inicial;*/
        G->ainicio = nodo;
        G->aatual = nodo;
        return(G->sarestas);
    }
    G->aatual->proximo = nodo;
    G->aatual = nodo;
    return(G->sarestas);
}

int GVdestroiVertice(Grafo G, int v) {
    Vertice *nodo, *verticeAnterior;
    nodo = GPVencontraVertice(G, v);
    G->nvertices--;
    /*Remoção de Estrelas e Arestas;*/
    while (nodo->estrela != NULL) { /*Verifica se existe alguma Estrela;*/
        GAdestroiAresta(G, nodo->estrela->aresta->id); /*Remove Aresta que usa este vertice;*/
    }
    /*Remoção do Vertice;*/
    if (G->vinicio->proximo == NULL) { /*Caso inicial - Unico Vertice;*/
        free(nodo);
        G->vinicio = NULL;
        return(0);
    }
    if ((G->vinicio->id == v) && (G->vinicio->proximo != NULL)) { /*Caso especial - Vertice na primeira posição, mas não o unico;*/
        G->vinicio = nodo->proximo;
        free(nodo);
        return(0);
    }
    verticeAnterior = G->vinicio;
    while (verticeAnterior->proximo != nodo) /*Pega Vertice anterior a V;*/
        verticeAnterior = verticeAnterior->proximo;
    if (nodo->proximo == NULL) { /*Caso especial - Vertice na ultima posição;*/
        G->vatual = verticeAnterior;
        verticeAnterior->proximo = NULL;
        free(nodo);
        return(0);
    }
    verticeAnterior->proximo = nodo->proximo;
    free(nodo);
    return(0);
}

int GAdestroiAresta(Grafo G, int a) {   
    Aresta *nodo, *nodoAnterior;
    nodo = GPAencontraAresta(G, a);
    G->narestas--;
    /*Remoção da struct estrela;*/
    GEdestroiEstrela(G, nodo->alfa, a);
    GEdestroiEstrela(G, nodo->omega, a);
    /*Remoção de Aresta*/
    if (G->ainicio->proximo == NULL) { /*Caso inicial - Unica aresta;*/
        free(nodo);
        G->ainicio = NULL;
        return(0);
    }
    if ((G->ainicio->id == a) && (G->ainicio->proximo != NULL)) { /*Caso especial - Aresta na primeira posição, mas nao a unica;*/
        G->ainicio = nodo->proximo;
        free(nodo);
        return(0);
    }
    nodoAnterior = G->ainicio;
    while (nodoAnterior->proximo != nodo) /*Pega aresta anterior a A;*/
        nodoAnterior = nodoAnterior->proximo;
    if (nodo->proximo == NULL) { /*Caso especial - Aresta na ultima posição;*/
        G->aatual = nodoAnterior; /*Anterior a A;*/
        nodoAnterior->proximo = NULL;
        free(nodo);
        return(0);
    }
    nodoAnterior->proximo = nodo->proximo;
    free(nodo);
    return(0);
}

boolean GBexisteIdVertice(Grafo G, int v) {
    Vertice *vertice;
    vertice = GPVencontraVertice(G, v);
    if (vertice != NULL) /*Se conseguir pegar um vertice valido, ele existe;*/
        return(TRUE);
    return(FALSE);
}

boolean GBexisteIdAresta(Grafo G, int a) {
    Aresta *aresta;
    aresta = GPAencontraAresta(G, a);
    if (aresta != NULL) /*Se conseguir pegar uma aresta valida, ela existe;*/
        return(TRUE);
    return(FALSE);
}

boolean GBexisteArestaDir(Grafo G, int v1, int v2) {
    int idAresta;
    idAresta = GApegaArestaDir(G, v1, v2);
    if (idAresta > 0)
        return(TRUE); /*Se conseguir pegar uma aresta valida, ela existe;*/
    return(FALSE);
}

boolean GBexisteAresta(Grafo G, int v1, int v2) {
    int idAresta;
    idAresta = GApegaAresta(G, v1, v2);
    if (idAresta > 0)
        return(TRUE); /*Se conseguir pegar uma aresta valida, ela existe;*/
    return(FALSE);
}

int GApegaArestaDir(Grafo G, int v1, int v2) {
    Aresta *nodo;
    nodo = G->ainicio;
    while (nodo != NULL) {
        if ((nodo->alfa->id == v1) && (nodo->omega->id == v2))
            return(nodo->id);
        nodo = nodo->proximo;
    }
    return(0);
}

int GApegaAresta(Grafo G, int v1, int v2) {
    Aresta *nodo;
    nodo = G->ainicio;
    while (nodo != NULL) {
        if (((nodo->alfa->id == v1) && (nodo->omega->id == v2)) || ((nodo->alfa->id == v2) && (nodo->omega->id == v1)))
            return(nodo->id);
        nodo = nodo->proximo;
    }
    return(0);
}

int GVprimeiroVertice(Grafo G) {
    return(G->vinicio->id);
}

int GVproximoVertice(Grafo G, int v) {
    Vertice *vertice;
    vertice = GPVencontraVertice(G, v);
    if ((vertice != NULL) && (vertice->proximo != NULL))
        return(vertice->proximo->id);
    return(0);
}

int GAprimeiraAresta(Grafo G) {
    return(G->ainicio->id);
}

int GAproximaAresta(Grafo G, int a) {
    Aresta *aresta;
    aresta = GPAencontraAresta(G, a);
    if ((aresta != NULL) && (aresta->proximo != NULL))
        return(aresta->proximo->id);
    return(0);
}

int GInumeroVertices(Grafo G) {
    return(G->nvertices);       
}

int GInumeroVerticesMax(Grafo G) {
    return(INT_MAX);
}

int GInumeroArestas(Grafo G) {
    return(G->narestas);        
}

int GInumeroArestasMax(Grafo G) {
    return(INT_MAX);
}

Grafo GGcarregaGrafo(char *nomeArquivo) {
    Grafo G;
    int i, numeroV, numeroA, v1, v2, lixo, aux, verificador;
    FILE *file;
    file = fopen(nomeArquivo, "r"); /*Aberto em modo leitura;*/
    if (file == NULL)
        return(0);
    G = GGcriaGrafo();   
    fscanf(file, "%d%d", &numeroV, &numeroA);
    fscanf(file, "%d%d", &lixo, &lixo); /*A semente do grafo será criada automaticamente por CriaVertice/Aresta, podendo ser descartada;*/
    verificador = 1;
    for (i = 1; i <= numeroV; i++) { /*Cria os vertices;*/
        fscanf(file, "%d", &aux);
        while (aux > verificador) { /*Cria e destroi os vertices que não pertencem ao grafo;*/
            lixo = GVcriaVertice(G);
            lixo = GVdestroiVertice(G, lixo);
            verificador++;
        }        
        lixo = GVcriaVertice(G);
        verificador++;
    } 
    verificador = 1;
    for (i = 1; i <= numeroA; i++) { /*Cria as arestas;*/
        fscanf(file, "%d%d%d", &aux, &v1, &v2);
        while (aux > verificador) {
            lixo = GAcriaAresta(G, v1, v2);
            lixo = GAdestroiAresta(G, lixo);
            verificador++;
        }
        lixo = GAcriaAresta(G, v1, v2);
        verificador++;
    } 
    return(G);
}

boolean GBsalvaGrafo(Grafo G, char *nomeArquivo) {
    int i, numeroV, numeroA, seedV, seedA, aux;
    Aresta *nodo;
    FILE *file;
    file = fopen(nomeArquivo, "wt");
    numeroV = G->nvertices;
    numeroA = G->narestas;
    seedV = G->svertices;
    seedA = G->sarestas;
    fprintf(file, "%d %d\n", numeroV, numeroA); /*Quantidade de vertice e aresta;*/
    fprintf(file, "%d %d\n", seedV, seedA);
    aux = GVprimeiroVertice(G);
    for (i = 0; i < numeroV; i++) {
        fprintf(file, "%d\n", aux);
        aux = GVproximoVertice(G, aux);
    }
    aux = GAprimeiraAresta(G);
    nodo = GPAencontraAresta(G, aux);
    for (i = 0; i < numeroA; i++) {
        fprintf(file, "%d %d %d\n", nodo->id, nodo->alfa->id, nodo->omega->id);
        aux = GAproximaAresta(G, aux);
        nodo = GPAencontraAresta(G, aux);
    }
    if (file == NULL) /*Verifica se foi salvo com sucesso;*/
        return(FALSE);
    fclose(file);
    return(TRUE);    
}

/*2.2.2*/

int GIpegaGrau(Grafo G, int v) {
    int Grau = 0;
    Vertice *nodo;
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    if (nodo != NULL) {
        estrela = nodo->estrela;
        while (estrela != NULL) {
            if (GBarestaLaco(G, estrela->aresta->id)) /*Se for laço, aumenta em 1 o grau;*/
                Grau++;
            Grau++;
            estrela = estrela->proximo;
        }
    }
    return(Grau);
}

int GAprimaAresta(Grafo G, int v) {
    Vertice *nodo;
    nodo = GPVencontraVertice(G, v);
    if (nodo->estrela != NULL)
        return(nodo->estrela->aresta->id);
    return(0);
}

int GAproxAresta(Grafo G, int v, int a) {
    Vertice *nodo;
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    estrela = nodo->estrela;
    while (estrela != NULL) {
        if ((estrela->aresta->id == a) && (estrela->proximo != NULL)) { /*Se encontrou a estrela que aponta para A, devolva o id da proxima;*/
            return(estrela->proximo->aresta->id);
        }
        estrela = estrela->proximo;
    }    
    return(0);
}

int GAprimaEntrada(Grafo G, int v) {
    Vertice *nodo; /*Entrada = OMEGA*/
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    estrela = nodo->estrela;
    while (estrela != NULL) {
        if (estrela->aresta->omega->id == v) /*Se a aresta ligada à estrela apontar para o vertice V pelo ponteiro omega, return;*/
            return(estrela->aresta->id);
        estrela = estrela->proximo;
    }    
    return(0);
}

int GAproxEntrada(Grafo G, int v, int a) {
    Vertice *nodo;
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    estrela = nodo->estrela;
    while ((estrela != NULL) && (estrela->aresta->id != a)) /*Encontra estrela que aponta para a aresta A;*/
        estrela = estrela->proximo;
    if (estrela != NULL) {
        estrela = estrela->proximo;
        while (estrela != NULL) {    
            if (estrela->aresta->omega->id == v)
                return(estrela->aresta->id);
            estrela = estrela->proximo;
        }
    }
    return(0);
}

int GAprimaSaida(Grafo G, int v) {
    Vertice *nodo; /*Saida = ALFA*/
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    estrela = nodo->estrela;
    while (estrela != NULL) {
        if (estrela->aresta->alfa->id == v) /*Se a aresta ligada à estrela apontar para o vertice V pelo ponteiro alfa, return;*/
            return(estrela->aresta->id);
        estrela = estrela->proximo;
    }    
    return(0);
}

int GAproxSaida(Grafo G, int v, int a) {
    Vertice *nodo;
    Estrela *estrela;
    nodo = GPVencontraVertice(G, v);
    estrela = nodo->estrela;
    while ((estrela != NULL) && (estrela->aresta->id != a)) /*Encontra estrela que aponta para a aresta A;*/
        estrela = estrela->proximo;
    if (estrela != NULL) {
        estrela = estrela->proximo;
        while (estrela != NULL) {
            if (estrela->aresta->alfa->id == v)
                return(estrela->aresta->id);
        }
    }
    return(0);
}


/*2.2.3*/

boolean GBarestaLaco(Grafo G, int a) {
    Aresta *nodo;
    nodo = GPAencontraAresta(G, a);
    if (nodo->alfa->id == nodo->omega->id) /*Se vertice de entrada e saida forem iguais, return;*/
        return(TRUE);
    return(FALSE);
}

int GValfa(Grafo G, int a) { /*Vertice de partida;*/
    Aresta *nodo;
    nodo = GPAencontraAresta(G, a);
    if (nodo != NULL)
        return(nodo->alfa->id);
    return(0);
}

int GVomega(Grafo G, int a) { /*Vertice de destino*/
    Aresta *nodo;
    nodo = GPAencontraAresta(G, a);
    if (nodo != NULL)
        return(nodo->omega->id);
    return(0);
}

int GVvizinho(Grafo G, int a, int v) {
    Aresta *nodo;
    nodo = GPAencontraAresta(G, a);
    if (nodo != NULL) {
        if (nodo->alfa->id == v) /*Recebe ALFA ou OMEGA, devolve o inverso;*/
            return(nodo->omega->id);
        return(nodo->alfa->id);
    }
    return(0);
}

/*Funções auxiliares que não consta na especificação;*/

static Vertice* GPVencontraVertice(Grafo G, int v) { /*PV - Ponteiro para Vertice;*/
    int i;
    Vertice *nodo; /*Vertice a ser encontrado;*/
    if (v <= G->svertices) {
        nodo = G->vinicio;
        for (i = 0; i < v; i++) {
            if (nodo->id == v)
                return(nodo);
            nodo = nodo->proximo;
        }
    }
    return(NULL);
}

static Aresta* GPAencontraAresta(Grafo G, int a) { /*PA - Ponteiro para Aresta;*/
    int i;
    Aresta *nodo; /*Aresta a ser encontrada;*/
    if (a <= G->sarestas) {
        nodo = G->ainicio;
        for (i = 0; i < a; i++) {
            if (nodo->id == a)
                return(nodo);
            nodo = nodo->proximo;
        }
    }
    return(NULL);
}

static void GEcriaEstrela(Grafo G, Vertice *vertice, Aresta *aresta) {
    Estrela *nodo, *aux;
    nodo = (Estrela*) malloc(sizeof(Estrela));
    nodo->proximo = NULL;
    nodo->aresta = aresta;
    if (vertice->estrela == NULL) /*ALFA - Se não existe nenhuma estrela no vertice;*/
        vertice->estrela = nodo;
    else {
        aux = vertice->estrela;
        while (aux->proximo != NULL) /*Percorre a lista estrela até achar a ultima posição;*/
            aux = aux->proximo;
        aux->proximo = nodo;
    }
    return;
}

static void GEdestroiEstrela(Grafo G, Vertice *vertice, int a) {
    Estrela *posEstrela, *estrelaAnterior;
    posEstrela = vertice->estrela;
    if (posEstrela->proximo == NULL) { /*Caso inicial - Vertice com apenas uma estrela;*/
        free(posEstrela);
        vertice->estrela = NULL;
        return;
    }
    if ((posEstrela->aresta->id == a) && (posEstrela->proximo != NULL)) { /*Caso especial - Estrela na primeira posição, mas não a unica;*/
        vertice->estrela = posEstrela->proximo;
        free(posEstrela);
        return;
    }
    while (posEstrela->aresta->id != a) { /*Encontra estrela que aponta para aresta A;*/
        estrelaAnterior = posEstrela; /*Fica sempre uma estrela atras de aux;*/
        posEstrela = posEstrela->proximo;
    }
    if (posEstrela->proximo == NULL) { /*Caso Especial - Estrela que aponta para o vertice A está na ultima posição;*/
        free(posEstrela);
        estrelaAnterior->proximo = NULL;
        return;
    }
    estrelaAnterior->proximo = posEstrela->proximo;
    free(posEstrela);
    return;
}