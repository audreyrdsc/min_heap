#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define AMARELO "\033[1;33m"
#define VERDE   "\033[1;32m"
#define CIANO   "\033[1;36m"
#define VERMELHO "\033[1;31m"
#define RESET   "\033[0m"

typedef struct {
    int *arrayDeElementos;
    int quantidadeAtual;
    int capacidadeMaxima;
} HeapBinario;




void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void trocarValores(int *valorA, int *valorB) {
    int temporario = *valorA;
    *valorA = *valorB;
    *valorB = temporario;
}

void desenharHeap(HeapBinario *heap) {
    printf("\n" CIANO "=== ESTADO ATUAL DO HEAP ===" RESET "\n");
    
    if (heap->quantidadeAtual == 0) {
        printf(" [ HEAP VAZIO ]\n");
        return;
    }
    

    printf("Vetor: [ ");
    for (int i = 0; i < heap->quantidadeAtual; i++) {
        printf("%d ", heap->arrayDeElementos[i]);
    }
    printf("]\n");


    printf("Visual:\n");
    for (int i = 0; i < heap->quantidadeAtual; i++) {
        
        int indiceFilhoEsquerdo = 2 * i + 1;
        int indiceFilhoDireito  = 2 * i + 2;
        
        bool existeFilhoEsquerdo = (indiceFilhoEsquerdo < heap->quantidadeAtual);
        bool existeFilhoDireito  = (indiceFilhoDireito < heap->quantidadeAtual);
        
        bool ehUmPai = (existeFilhoEsquerdo || existeFilhoDireito);
        
        if (ehUmPai) {
            printf("  Pai " AMARELO "%d" RESET " (pos %d) -> ", heap->arrayDeElementos[i], i);
            
            if (existeFilhoEsquerdo) {
                printf("Esq: " VERDE "%d" RESET " ", heap->arrayDeElementos[indiceFilhoEsquerdo]);
            }
            
            if (existeFilhoDireito) {
                printf("| Dir: " VERDE "%d" RESET "", heap->arrayDeElementos[indiceFilhoDireito]);
            }
            printf("\n");
        }
    }
    printf("----------------------------\n");
}



void heapifyUp(HeapBinario *heap, int indiceAtual) {
    if (indiceAtual > 0) {
        int indiceDoPai = (indiceAtual - 1) / 2;

        int valorDoFilho = heap->arrayDeElementos[indiceAtual];
        int valorDoPai   = heap->arrayDeElementos[indiceDoPai];

        bool filhoEhMenorQuePai = (valorDoFilho < valorDoPai);
        
        if (filhoEhMenorQuePai) {
            trocarValores(&heap->arrayDeElementos[indiceAtual], &heap->arrayDeElementos[indiceDoPai]);
            heapifyUp(heap, indiceDoPai);
        }
    }
}

void heapifyDown(HeapBinario *heap, int indiceAtual) {
    int indiceDoMenor = indiceAtual;
    
    int indiceFilhoEsquerdo = 2 * indiceAtual + 1;
    int indiceFilhoDireito  = 2 * indiceAtual + 2;

    bool existeEsquerdo = (indiceFilhoEsquerdo < heap->quantidadeAtual);

    if (existeEsquerdo) {
        int valorEsquerdo = heap->arrayDeElementos[indiceFilhoEsquerdo];
        int valorVencedorAtual = heap->arrayDeElementos[indiceDoMenor];

        bool esquerdoEhMenor = (valorEsquerdo < valorVencedorAtual);

        if (esquerdoEhMenor) {
            indiceDoMenor = indiceFilhoEsquerdo;
        }
    }

    bool existeDireito = (indiceFilhoDireito < heap->quantidadeAtual);

    if (existeDireito) {
        int valorDireito = heap->arrayDeElementos[indiceFilhoDireito];
        
        int valorVencedorAtual = heap->arrayDeElementos[indiceDoMenor]; 

        bool direitoEhMelhor = (valorDireito < valorVencedorAtual);

        if (direitoEhMelhor) {
            indiceDoMenor = indiceFilhoDireito;
        }
    }

    bool houveMudanca = (indiceDoMenor != indiceAtual);

    if (houveMudanca) {
        trocarValores(&heap->arrayDeElementos[indiceAtual], &heap->arrayDeElementos[indiceDoMenor]);
        heapifyDown(heap, indiceDoMenor);
    }
}

void inserirNovoValor(HeapBinario *heap, int valorParaInserir) {
    bool heapEstaCheio = (heap->quantidadeAtual == heap->capacidadeMaxima);

    if (heapEstaCheio) {
        heap->capacidadeMaxima *= 2; 
        heap->arrayDeElementos = (int*) realloc(heap->arrayDeElementos, heap->capacidadeMaxima * sizeof(int));
    }
    
    int indiceOndeVaiEntrar = heap->quantidadeAtual;

    heap->arrayDeElementos[indiceOndeVaiEntrar] = valorParaInserir;
    
    heap->quantidadeAtual++;
    
    printf(VERDE " -> Inserindo %d..." RESET "\n", valorParaInserir);

    heapifyUp(heap, indiceOndeVaiEntrar);
}

int removerRaiz(HeapBinario *heap) {
    if (heap->quantidadeAtual == 0) return -1;

    int indiceDaRaiz = 0;
    int valorQueSeraRemovido = heap->arrayDeElementos[indiceDaRaiz];
    
    int indiceDoUltimoElemento = heap->quantidadeAtual - 1;
    int valorDoUltimoElemento  = heap->arrayDeElementos[indiceDoUltimoElemento];
    
    heap->arrayDeElementos[indiceDaRaiz] = valorDoUltimoElemento;
    
    heap->quantidadeAtual--;
    
    printf(VERMELHO " -> Removendo %d (Raiz)..." RESET "\n", valorQueSeraRemovido);
    
    bool sobrouAlguem = (heap->quantidadeAtual > 0);
    
    if (sobrouAlguem) {
        heapifyDown(heap, indiceDaRaiz);
    }
        
    return valorQueSeraRemovido;
}



int main() {
    HeapBinario meuHeap;
    meuHeap.quantidadeAtual = 0;
    meuHeap.capacidadeMaxima = 5;
    meuHeap.arrayDeElementos = (int*) malloc(meuHeap.capacidadeMaxima * sizeof(int));
    
    int opcaoUsuario = -1;
    int valorDigitado;

    while (opcaoUsuario != 0) {
        limparTela();
        desenharHeap(&meuHeap);
        
        printf("\nMENU MIN-HEAP:\n");
        printf("1. Inserir Valor\n");
        printf("2. Remover Raiz\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcaoUsuario);

        if (opcaoUsuario == 1) {
            printf("Digite o valor: ");
            scanf("%d", &valorDigitado);
            inserirNovoValor(&meuHeap, valorDigitado);
        } 
        else if (opcaoUsuario == 2) {
            if (meuHeap.quantidadeAtual > 0) {
                removerRaiz(&meuHeap);
            } else {
                printf(VERMELHO "Heap vazio!\n" RESET);
            }
            printf("\nEnter para continuar...");
            getchar(); getchar(); 
        }
    }

    free(meuHeap.arrayDeElementos);
    return 0;
}