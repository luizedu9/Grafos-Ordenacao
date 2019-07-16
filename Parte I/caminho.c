/*
// * File:      caminho.c
// * Nome:      Luiz Eduardo Pereira
// * Matrícula: 0021619
// * Data:      19/06/2016
*/

#include "caminho.h"
#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

#define INF 999999 /*INF = Inteiro muito grande;*/

void Gcaminho(Grafo G, float *pesos, int a, int b) {
    
/*Bloco de Declaração de Variaveis:*/
    /*Todos os vetores dessa função indexidado de 1;*/
    int *pai; 
    float *custo;
    int i, v, menorV, aux1, aux2;
    boolean *flag;
    v = GInumeroVertices(G);
    pai = (int*) malloc ((v + 1) * sizeof(int)); /*Armazena o caminho;*/
    custo = (float*) malloc((v + 1) * sizeof(float));
    flag = (boolean*) malloc((v + 1) * sizeof(boolean)); /*Marca os vertices visitados;*/
    for (i = 1; i <= v; i++) {
        pai[i] = 0;
        custo[i] = INF;
        flag[i] = FALSE;      
    }
    
/*Bloco de Custos:*/
    custo[a] = 0;
    while (flag[b] != TRUE) { /*Enquanto B não for o escolhido;*/
        aux1 = INF;
        for (i = 1; i <= v; i++) { /*Pega vertice com menor custo;*/
            if ((custo[i] < aux1) && (flag[i] == FALSE)) { /*Se custo de vertice for igual a INF, ou ele ainda não pode ser*/
                aux1 = custo[i];                           /*selecionado ou não possui ligação com a rede principal;*/
                menorV = i; /*Vertice com menor custo;*/
            }
        }
        if ((custo[b] == aux1) && (aux1 == INF)) { /*Se aux1 = INF, não existe mais vertice disponivel para ser selecionado*/
            printf("%d", a);                       /*e se b = aux1, b não possui ligação com com a rede principal;*/
            return; /*Imprime o vertice inicial e abandona a função;*/
        }
        flag[menorV] = TRUE; /*menorV é marcado como visitado;*/
        aux2 = GAprimaSaida(G, menorV); /*Primeira saida do vertice menorV;*/
        while(aux2 != 0) { /*aux2 = 0, não existe mais arestas no menorV;*/
            if (custo[menorV] + pesos[aux2] < custo[GVvizinho(G, aux2, menorV)]) { /*Se o custo do menorV + Peso < Vizinho de menorV;*/
                custo[GVvizinho(G, aux2, menorV)] = custo[menorV] + pesos[aux2];      
            }
            aux2 = GAproxSaida(G, menorV, aux2); /*Proximo vertice;*/
        }
    }   
    
/*Bloco de Caminho Mínimo:*/
    menorV = b;
    while (menorV != a) { /*Enquanto não alcançar a;*/
        aux2 = GAprimaEntrada(G, menorV);
        while(aux2 != 0) {
            if (custo[GVvizinho(G, aux2, menorV)] + pesos[aux2] <= custo[menorV]) { /*Se custo do Vizinho + Peso <= custo de menorV;*/
                pai[GVvizinho(G, aux2, menorV)] = menorV; /*Pai recebe o vertice de seu filho;*/
                menorV = GVvizinho(G, aux2, menorV); 
            }
            aux2 = GAproxEntrada(G, menorV, aux2); /*Proximo vertice;*/
        }
    }
 
/*Bloco de imprenssão:*/
    i = a; /*i recebe o vertice final;*/
    printf("%d", a);
    while (i != b) {
        printf(" -> %d", pai[i]);
        i = pai[i];
    }      
    
    free(pai);
    free(custo);
    free(flag);
    
    return;
}