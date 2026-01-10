#include <stdio.h>
#include <stdlib.h>

#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[1;93m"
#define WHITE "\033[1;97m"
#define RESET "\033[0m"


/* Estrutura do Heap Dinâmico */
typedef struct {
    int *dados;
    int tamanho;
    int capacidade;
} Heap;


/* Protótipos das funções */
void exibirHeap(Heap *h);


/* Limpar tela */
void limparTela() {
#ifdef _WIN32
    system("cls");      // Windows
#else
    system("clear");    // Linux / macOS
#endif
}


/* Inicializa o heap */
void inicializarHeap(Heap *h, int capacidadeInicial) {
    h->tamanho = 0;
    h->capacidade = capacidadeInicial;
    h->dados = (int *) malloc(capacidadeInicial * sizeof(int));

    if (h->dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
}


/* Troca dois elementos */
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


/* Subir no Heap (Heapify Up) */
void heapifyUp(Heap *h, int indice) {
    int pai = (indice - 1) / 2;

    if (indice > 0 && h->dados[indice] < h->dados[pai]) {
        printf(WHITE "UP - Troca %d com %d\n" RESET, h->dados[indice], h->dados[pai]);
        trocar(&h->dados[indice], &h->dados[pai]);
        heapifyUp(h, pai);
    }
}


/* Descer no Heap (Heapify Down) */
void heapifyDown(Heap *h, int indice) {
    int menor = indice;
    int esquerdo = 2 * indice + 1;
    int direito  = 2 * indice + 2;

    if (esquerdo < h->tamanho && h->dados[esquerdo] < h->dados[menor]) {
        menor = esquerdo;
    }

    if (direito < h->tamanho && h->dados[direito] < h->dados[menor]) {
        menor = direito;
    }

    if (menor != indice) {
        printf(WHITE "DOWN - Troca %d com %d\n" RESET, h->dados[indice], h->dados[menor]);
        trocar(&h->dados[indice], &h->dados[menor]);
        heapifyDown(h, menor);
    }
}


/* Aumenta a capacidade do heap dinamicamente */
void aumentarCapacidade(Heap *h) {
    h->capacidade *= 2;
    h->dados = (int *) realloc(h->dados, h->capacidade * sizeof(int));

    if (h->dados == NULL) {
        printf("Erro ao realocar memoria.\n");
        exit(1);
    }
}


/* Inserir no Heap */
void inserir(Heap *h, int valor) {
    if (h->tamanho == h->capacidade) {
        aumentarCapacidade(h);
    }

    limparTela();
    exibirHeap(h);
    printf("\n");

    printf(BLUE "Elemento inserido com sucesso.\n" RESET);
    printf("\n");

    h->dados[h->tamanho] = valor;
    heapifyUp(h, h->tamanho);
    h->tamanho++;

    printf("\n");
    printf(GREEN "Novo " RESET);
    exibirHeap(h);
  
}


/* Remover o menor elemento */
void removerMenor(Heap *h) {
    if (h->tamanho == 0) {
        limparTela();
        printf(YELLOW "Heap vazia!\n" RESET);
        return;
    }

    limparTela();
    exibirHeap(h);
    printf("\n");

    printf(RED "Elemento removido: %d\n" RESET, h->dados[0]);
    printf("\n");

    h->dados[0] = h->dados[h->tamanho - 1];
    h->tamanho--;
    heapifyDown(h, 0);

    printf("\n");

    if (h->tamanho > 0)
        printf(GREEN "Novo " RESET);

    exibirHeap(h);
}


/* Exibir Heap */
void exibirHeap(Heap *h) {
    if (h->tamanho == 0) {
        printf(YELLOW "Heap vazia!\n" RESET);
        return;
    }

    printf(GREEN "Heap: [ ");
    for (int i = 0; i < h->tamanho; i++) {
        printf("%d ", h->dados[i]);
    }
    printf("]\n" RESET);
}


/* Liberar memoria */
void liberarHeap(Heap *h) {
    free(h->dados);
}


/* Menu */
void menu() {
    printf("\n===== MENU HEAP DINAMICO (MIN-HEAP) =====\n");
    printf("1 - Inserir no Heap\n");
    printf("2 - Remover o menor elemento\n");
    printf("3 - Exibir Heap\n");
    printf("0 - Sair\n");
    printf("\nEscolha uma opcao: ");
}


/* Programa principal */
int main() {
    Heap heap;
    int opcao, valor;

    inicializarHeap(&heap, 10);

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\nDigite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserir(&heap, valor);
                break;

            case 2:
                removerMenor(&heap);
                break;

            case 3:
                limparTela();
                exibirHeap(&heap);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                limparTela();
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    liberarHeap(&heap);
    return 0;
}
