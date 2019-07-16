/*
 * File:      ordenacao.c
 * Nome:      Luiz Eduardo Pereira
 * Matrícula: 0021619
 * Data:      09/07/2016
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

/*Para compilar via terminal é necessario utilizar o comando -lm para incluir a biblioteca math.h;*/

#ifndef TAMANHO
    #define TAMANHO 100000
#endif

#ifndef METODO
    #define METODO 4
#endif

#ifndef EXECUCAO
    #define EXECUCAO 3
#endif

typedef enum {
    FALSE, TRUE
}boolean;

/*Guarda todos os dados estatisticos do programa;*/
struct estatistica {
    double NComparacao[EXECUCAO], NTroca[EXECUCAO];
    double mediaComparacao, mediaTroca;
    uint64_t tempo[EXECUCAO], mediaTempo;
    int contador; /*Contador determina em qual posição do vetor o dado será inserido;*/
};
typedef struct estatistica *Estatistica;

/*Quando a execução do programa é muito rapida a função CLOCK_PER_SEC não foi capaz de dar um resultado preciso, pois retornava 0 milisegundos
  A solução foi usar um codigo que encontrei na internet que retorna o tempo em Nanossegundos;*/
int64_t calculaTempo(struct timespec *timeA_p, struct timespec *timeB_p) {
    return (((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) - ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec));
}

int* criaVetor(void) {
    int *vetor;
    int i;
    vetor = (int*) malloc(TAMANHO * sizeof(int));
    for (i = 0; i < TAMANHO; i++)
        vetor[i] = rand() % 999;
    return(vetor);
}

void imprimeArquivo(char *metodo, Estatistica valor) {
    FILE *file;
    file = fopen("Ordenação.txt", "a"); /*Reescrever no arquivo sem apagar dados anteriores;*/
    if (file == NULL)
        return;
    fprintf(file, "%s: \n", metodo);
    fprintf(file, "%.0f comparações; \n", valor->NComparacao[valor->contador]);
    fprintf(file, "%.0f trocas; \n", valor->NTroca[valor->contador]);
    fprintf(file, "%.4f milissegundos; \n", ((double)valor->tempo[valor->contador] / 1000000));
    fclose(file);
    return;
}

void imprimeEstatistica(Estatistica valor) {
    FILE *file;
    double DComparacao = 0, DTroca = 0, DTempo = 0;
    int i;
    file = fopen("Ordenação.txt", "a");
    if (file == NULL)
        return;
    
    valor->mediaComparacao /= EXECUCAO;
    valor->mediaTroca /= EXECUCAO;
    valor->mediaTempo /= EXECUCAO;
    
    for (i = 0; i < EXECUCAO; i++) {
        DComparacao = DComparacao + pow((valor->NComparacao[i] - valor->mediaComparacao), 2);
        DTroca = DTroca + pow((valor->NTroca[i] - valor->mediaTroca), 2);
        DTempo = DTempo + pow((((double)valor->tempo[i] / 1000000) - ((double)valor->mediaTempo / 1000000)), 2);
    }
    DComparacao /= EXECUCAO;
    DTroca /= EXECUCAO;
    DTempo /= EXECUCAO;
    DComparacao = sqrt(DComparacao);
    DTroca = sqrt(DTroca);
    DTempo = sqrt(DTempo);
    
    fprintf(file, "\nEstatisticas da Execução: \n");
    fprintf(file, "%.0f comparações - S = %.2f; \n", valor->mediaComparacao, DComparacao);
    fprintf(file, "%.0f trocas - S = %.2f; \n", valor->mediaTroca, DTroca);
    fprintf(file, "%.4f milissegundos - S = %.2f; \n\n", ((double)valor->mediaTempo / 1000000), DTempo);
    fclose(file);
    return;
}

boolean compara(int maior, int menor, Estatistica valor) {
    valor->NComparacao[valor->contador]++;
    if (maior > menor)
        return(TRUE);
    return(FALSE);
}

void troca(int *vetor, int pos1, int pos2, Estatistica valor) {
    int aux;
    valor->NTroca[valor->contador]++;
    aux = vetor[pos1];
    vetor[pos1] = vetor[pos2];
    vetor[pos2] = aux;
    return;
}

int mediana(int *vetor, int i, int j) {
    int x, y, z, X, Y, Z;
    x = (rand() % (j - i)) + i;
    y = (rand() % (j - i)) + i;
    z = (rand() % (j - i)) + i;
    X = vetor[x];
    Y = vetor[y];
    Z = vetor[z];
    if (((X >= Y) && (X <= Z)) || ((X >= Z) && (X <= Y)))
        return(x);
    if (((Y >= X) && (Y <= Z)) || ((Y >= Z) && (Y <= X)))
        return(y);
    if (((Z >= Y) && (Z <= X)) || ((Z >= X) && (Z <= Y)))
        return(z);
    return(0);
}

void bubblesort(int *vetor, Estatistica valor) {
    int i = 0, j;
    double NTrocalocal = 0;
    struct timespec inicio, final;
    
    valor->NComparacao[valor->contador] = 0;
    valor->NTroca[valor->contador] = 0;
    valor->tempo[valor->contador] = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    while ((i < TAMANHO) && (NTrocalocal != 0)) {
        NTrocalocal = 0; /*Se não ouver nenhuma troca local, vetor ja está ordenado;*/
        for (j = 0; j < (TAMANHO - i); j++) {          
            if (compara(vetor[j], vetor[j + 1], valor)) {
                troca(vetor, j, (j + 1), valor);
                NTrocalocal++;
            }
        }
        i++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &final);
    valor->tempo[valor->contador] = calculaTempo(&final, &inicio);
    imprimeArquivo("Bubble Sort", valor);
    valor->mediaComparacao += valor->NComparacao[valor->contador];
    valor->mediaTroca += valor->NTroca[valor->contador];
    valor->mediaTempo += valor->tempo[valor->contador];
    return;
}

/*O Insertion Sort não troca posições no vetor, apenas faz atribuições, então não utilizei a funçao troca;*/
void insertionsort(int *vetor, Estatistica valor) {
    int i, j, x;
    struct timespec inicio, final;

    valor->NComparacao[valor->contador] = 0;
    valor->NTroca[valor->contador] = 0;
    valor->tempo[valor->contador] = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
	
    for (i = 1; i <= (TAMANHO - 1); i++) { /*Inicia em i = 1, pois um vetor de uma unica posição ja está ordenado;*/
        x = vetor[i];
        j = i - 1;
        while ((!compara(x, vetor[j], valor)) && (j >= 0)) {
            vetor[j + 1] = vetor[j];
            valor->NTroca[valor->contador]++;
            j--;
        }
        vetor[j + 1] = x;
        valor->NTroca[valor->contador]++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &final);
    valor->tempo[valor->contador] = calculaTempo(&final, &inicio);
    imprimeArquivo("Insertion Sort", valor);
    valor->mediaComparacao += valor->NComparacao[valor->contador];
    valor->mediaTroca += valor->NTroca[valor->contador];
    valor->mediaTempo += valor->tempo[valor->contador];
    return;
}

void ordenaQuicksort(int *vetor, int inicioV, int finalV, Estatistica valor) {
    int pivo, i, j;
    i = inicioV;
    j = finalV;
    pivo = vetor[mediana(vetor, i, j)];
    while (i < j) {
        while (compara(pivo, vetor[i], valor))
            i++;
        while (compara(vetor[j], pivo, valor))
            j--;
        if (i <= j) {
            troca(vetor, i, j, valor);
            i++;
            j--;
        }
    }
    if (j > inicioV)
        ordenaQuicksort(vetor, inicioV, j, valor);
    if (i < finalV)
        ordenaQuicksort(vetor, i, finalV, valor);
    return;
}

void quicksort(int *vetor, Estatistica valor) {
    struct timespec inicio, final;
    
    valor->NComparacao[valor->contador] = 0;
    valor->NTroca[valor->contador] = 0;
    valor->tempo[valor->contador] = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
	
    ordenaQuicksort(vetor, 0, (TAMANHO - 1), valor);
	
    clock_gettime(CLOCK_MONOTONIC, &final);
    valor->tempo[valor->contador] = calculaTempo(&final, &inicio);
    imprimeArquivo("Quick Sort", valor);
    valor->mediaComparacao += valor->NComparacao[valor->contador];
    valor->mediaTroca += valor->NTroca[valor->contador];
    valor->mediaTempo += valor->tempo[valor->contador];
    return;
}

void ordenaQuicksortTurbinado(int *vetor, int inicioV, int finalV, Estatistica valor) {
    int pivo, i, j, x;
    
    if ((finalV - inicioV) < 40) {
        for (i = inicioV; i <= (finalV - 1); i++) {
            x = vetor[i];
            j = i - 1;
            while ((!compara(x, vetor[j], valor)) && (j >= 0)) {
                vetor[j + 1] = vetor[j];
                valor->NTroca[valor->contador]++;
                j--;
            }
            vetor[j + 1] = x;
            valor->NTroca[valor->contador]++;
        }
	return;
    }
    i = inicioV;
    j = finalV;
    pivo = vetor[mediana(vetor, i, j)];
    while (i < j) {
        while (compara(pivo, vetor[i], valor))
            i++;
        while (compara(vetor[j], pivo, valor))
            j--;
        if (i <= j) {
            troca(vetor, i, j, valor);
            i++;
            j--;
        }
    }
    if (j > inicioV)
        ordenaQuicksortTurbinado(vetor, inicioV, j, valor);
    if (i < finalV)
        ordenaQuicksortTurbinado(vetor, i, finalV, valor);
    return;
}

void quicksortTurbinado(int *vetor, Estatistica valor) {
    struct timespec inicio, final;
    
    valor->NComparacao[valor->contador] = 0;
    valor->NTroca[valor->contador] = 0;
    valor->tempo[valor->contador] = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    ordenaQuicksortTurbinado(vetor, 0, (TAMANHO - 1), valor);
    
    clock_gettime(CLOCK_MONOTONIC, &final);
    valor->tempo[valor->contador] = calculaTempo(&final, &inicio);
    imprimeArquivo("Quick Sort Turbinado", valor);
    valor->mediaComparacao += valor->NComparacao[valor->contador];
    valor->mediaTroca += valor->NTroca[valor->contador];
    valor->mediaTempo += valor->tempo[valor->contador];
    return;
}

int main(void) {
    int *vetor;
    int i;
    Estatistica valor;

    srand(time(NULL));
    valor = (Estatistica) malloc(sizeof(struct estatistica));
    valor->mediaComparacao = 0;
    valor->mediaTempo = 0;
    valor->mediaTroca = 0;
    valor->contador = 0;

    switch (METODO) { /*Escolhe metodo;*/
        case 1: {
            for (i = 0; i < EXECUCAO; i++) {
                vetor = criaVetor();
                bubblesort(vetor, valor);
                valor->contador++;
            }
            break;
        }
        case 2: {
            for (i = 0; i < EXECUCAO; i++) {
                vetor = criaVetor();
                insertionsort(vetor, valor);
                valor->contador++;
            }
            break;
        }
        case 3: {
            for (i = 0; i < EXECUCAO; i++) {
                vetor = criaVetor();
                quicksort(vetor, valor);
                valor->contador++;
            }
            break;
        }
        case 4: {
            for (i = 0; i < EXECUCAO; i++) {
                vetor = criaVetor();
                quicksortTurbinado(vetor, valor);
                valor->contador++;
            }
            break;
        }
    }

    imprimeEstatistica(valor);
    free(vetor);
    free(valor);
    return(0);
}

/*Referencia para a funçao tempo: http://stackoverflow.com/questions/361363/how-to-measure-time-in-milliseconds-using-ansi-c - (Acesso em 26/07/2016);*/