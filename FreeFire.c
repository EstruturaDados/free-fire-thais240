#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
    int quantidade; 
} Componente;


//Contador global de comparações
long long comparacoes = 0;

void removerItem(Componente v[], int *total);

void lerString(char *dest, int tamanho) {
    fgets(dest, tamanho, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}


/*
Mostra todos os componentes da mochila alinhados.
*/
void mostrarComponentes(Componente v[], int n) {
    printf("\n========== MOCHILA DE SOBREVIVÊNCIA ==========\n");
    for (int i = 0; i < n; i++) {
        printf("%d. Nome: %-20s | Tipo: %-20s | Quantidade: %-20d | Prioridade: %-10d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
    }
    printf("\n==============================================\n");
}

//ordena todos os itens da mochila pela prioridade que o jogador atribuiu.
void selectionSortPrioridade(Componente v[], int n) {
    comparacoes = 0;

    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade < v[menor].prioridade) {
                menor = j;
            }
        }

        if (menor != i) {
            Componente temp = v[i];
            v[i] = v[menor];
            v[menor] = temp;
        }
    }
}

//ordena pelo tipo do item, do tipo com mais itens ao que tem menos.
void insertionSortTipo(Componente v[], int n) {
    comparacoes = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else break;
        }

        v[j + 1] = chave;
    }
}

/*busca pelo nome que o jogador adicionou, não funciona se houver
algum caractere diferente do nome adicionado.
*/
int buscaBinariaPorNome(Componente v[], int n, char alvo[]) {
    comparacoes = 0;
    int ini = 0, fim = n - 1;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comparacoes++;

        int cmp = strcmp(alvo, v[meio].nome);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            fim = meio - 1;
        else
            ini = meio + 1;
    }
    return -1;
}


//mostra o tempo que levou para a busca ser feita
double medirTempo(void (*alg)(Componente[], int), Componente v[], int n) {
    clock_t inicio = clock();
    alg(v, n);
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}


/*
Menu com as opções de organização, o jogador pode escolher como quer organizar os itens
ou se quer voltar ao menu principal.
*/
void menuOrganizar(Componente v[], int total) {
    int opc;

    do {
        printf("\n======= ESTRATÉGIA DE ORGANIZAÇÃO =======\n");
        printf("1. Ordenar por Prioridade\n");
        printf("2. Ordenar por Tipo\n");
        printf("0. Voltar\n ");
        printf("\n==================================\n");
        printf("\nEscolha o critério:\n");
        scanf("%d", &opc);
        getchar();

        if (opc == 1) {
            double tempo = medirTempo(selectionSortPrioridade, v, total);
            printf("\nMochila ordenada por prioridade!\nComparações: %lld\nTempo: %.6f s\n", comparacoes, tempo);
            mostrarComponentes(v, total);
        }

        else if (opc == 2) {
            double tempo = medirTempo(insertionSortTipo, v, total);
            printf("\nMochila ordenada por tipo!\nComparações: %lld\nTempo: %.6f s\n", comparacoes, tempo);
            mostrarComponentes(v, total);
        }

    } while (opc != 0);
}
//remove um item pelo número que ele foi adicionado.
void removerItem(Componente v[], int *total) {
    if (*total == 0) {
        printf("A mochila está vazia!\n");
        return;
    }

    mostrarComponentes(v, *total);

    int pos;
    printf("Digite o número do item que deseja remover: ");
    scanf("%d", &pos);
    getchar();

    if (pos < 1 || pos > *total) {
        printf("Posição inválida!\n");
        return;
    }

    pos--;

    //Avança o item para frente. O índice 2 vira o 1 e assim por diante.
    for (int i = pos; i < *total - 1; i++) {
        v[i] = v[i + 1];
    }

    (*total)--;

    printf("Item removido com sucesso!\n");
}
//tabela principal onde as escolhas serão feitas.
int main() {
    Componente mochila[10];
    int total = 0, opc;

    do {
        printf("\n========== MOCHILA DE SOBREVIVÊNCIA ==========\n");
        printf("Itens na mochila: %d/10\n\n", total);
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Mostrar mochila\n");
        printf("3. Organizar mochila\n");
        printf("4. Buscar por nome (Busca Binária)\n");
        printf("5. Remover item\n");
        printf("0. Sair\n");
        printf("\n==============================================\n");
        printf("\nEscolha uma opção:");
        scanf("%d", &opc);
        getchar();

        //adiciona um novo item com o nome, tipo, quantidade e prioridade informados pelo jogador.
        if (opc == 1) {
            if (total >= 10) {
                printf("A mochila está cheia, descarte um item para liberar espaço!\n");
                continue;
            }

            printf("Nome: ");
            lerString(mochila[total].nome, 30);

            printf("Tipo: ");
            lerString(mochila[total].tipo, 20);
            
            printf("Quantidade: ");        
            scanf("%d", &mochila[total].quantidade);
            getchar();

            printf("Prioridade: ");
            scanf("%d", &mochila[total].prioridade);
            getchar();

            

            total++;
        }
        //mostra todos os itens adicionados até o momento na mochila.
        else if (opc == 2) {
            mostrarComponentes(mochila, total);
        }
        //entra no menu para escolher qual organização o jogador prefere.
        else if (opc == 3) {
            menuOrganizar(mochila, total);
        }
        //busca o item pelo nome e retorna suas informações.
        else if (opc == 4) {
            char nomeBusca[30];

            printf("\nDigite o nome a buscar: ");
            lerString(nomeBusca, 30);

            int pos = buscaBinariaPorNome(mochila, total, nomeBusca);

            printf("\nComparações da busca: %lld\n", comparacoes);
            

      

            if (pos >= 0) {
                printf("\nItem encontrado!\n");

printf("\n%-20s | %-20s | %-20s | %-10s\n",
       "Nome", "Tipo", "Quantidade", "Prioridade");
printf("--------------------------------------------------------------\n");

printf("%-20s | %-20s | %-20d | %-10d\n",
       mochila[pos].nome,
       mochila[pos].tipo,
       mochila[pos].quantidade,
       mochila[pos].prioridade);
printf("--------------------------------------------------------------\n");
            } else {
                printf("Item não encontrado.\n");
            }
        }
        //remove o item pelo índice
        else if (opc == 5) {
        removerItem(mochila, &total);
        }

    } while (opc != 0);

    return 0;
}