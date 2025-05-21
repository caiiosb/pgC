#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CARTAS 7 

typedef struct {
    char nome[30];
    int populacao;
    float area;
    float PIB;
    int pontos_turisticos;
} Carta;

void inicializarCartas(Carta cartas[]) {

 
    strcpy(cartas[1].nome, "Sao Paulo");
    cartas[1].populacao = 11;
    cartas[1].area = 1.521;
    cartas[1].PIB = 2.719;
    cartas[1].pontos_turisticos = 36;

    strcpy(cartas[2].nome, "Rio de Janeiro");
    cartas[2].populacao = 6;
    cartas[2].area = 1.200;
    cartas[2].PIB = 949.301;
    cartas[2].pontos_turisticos = 58;

    strcpy(cartas[3].nome, "Minas Gerais");
    cartas[3].populacao = 20;
    cartas[3].area = 586.528;
    cartas[3].PIB = 857.593;
    cartas[3].pontos_turisticos = 24;

    strcpy(cartas[4].nome, "Santa Catarina");
    cartas[4].populacao = 7;
    cartas[4].area = 95.346;
    cartas[4].PIB = 428.571;
    cartas[4].pontos_turisticos = 32;

    strcpy(cartas[5].nome, "Piauí");
    cartas[5].populacao = 3;
    cartas[5].area = 251.529;
    cartas[5].PIB = 64.028;
    cartas[5].pontos_turisticos = 43;


    
}

int compararCartas(Carta carta1, Carta carta2, int atributo)
{
    
    if (atributo == 1) {
        printf("Comparando populacao: %s (%d) Milhoes vs %s (%d) Milhoes \n", carta1.nome, carta1.populacao, carta2.nome, carta2.populacao);
        if (carta1.populacao > carta2.populacao) {
            printf("%s ganhou!\n", carta1.nome);
            return 1;
        } else {
            printf("%s ganhou!\n", carta2.nome);
            return 2;
        }
    } else if (atributo == 2) {
        printf("Comparando area: %s (%.3f) vs %s (%.3f)\n", carta1.nome, carta1.area, carta2.nome, carta2.area);
        if (carta1.area > carta2.area) {
            printf("%s ganhou!\n", carta1.nome);
            return 1;
        } else {
            printf("%s ganhou!\n", carta2.nome);
            return 2;
        }
    } else if (atributo == 3) {
        printf("Comparando PIB: %s (%.3f) vs %s (%.3f)\n", carta1.nome, carta1.PIB, carta2.nome, carta2.PIB);
        if (carta1.PIB > carta2.PIB) {
            printf("%s ganhou!\n", carta1.nome);
            return 1;
        } else {
            printf("%s ganhou!\n", carta2.nome);
            return 2;  
        }
    } else if (atributo == 4) {
        printf("Comparando pontos turisticos: %s (%d) vs %s (%d) \n", carta1.nome, carta1.pontos_turisticos, carta2.nome, carta2.pontos_turisticos);
        if (carta1.pontos_turisticos > carta2.pontos_turisticos) {
            printf("%s ganhou!\n", carta1.nome);
            return 1;
        } else {
            printf("%s ganhou!\n", carta2.nome);
            return 2;
        }
    } else {
        printf("Atributo inválido!\n");
        return 0; // Retorna 0 para indicar erro
    }
}

int main() {
    Carta cartas[MAX_CARTAS];
    inicializarCartas(cartas);

    srand(time(0));

    int indice1 = rand() % 5 + 1;
    int indice2 = rand() % 5 + 1;

    // Garantir que os dois índices são diferentes
    while (indice2 == indice1) {
        indice2 = rand() % 5 + 1;
    }

    printf("Carta do jogador 1: %s\n", cartas[indice1].nome);
    printf("Carta do jogador 2: %s\n", cartas[indice2].nome);


    int vitoria1 = 0;
    int vitoria2 = 0;
    int atributo;
    int rodada = 1;   
    while (rodada < 4)
    {
        printf("Escolha o atributo para comparar (1: populacao, 2: area, 3: PIB, 4: pontos turisticos): ");
        scanf("%d", &atributo);
    
       int resultado = compararCartas(cartas[indice1], cartas[indice2], atributo);
       if (resultado == 1 ){
        vitoria1++;
       } else if (resultado == 2){
         vitoria2++;

       }
         
        
        

        rodada++;
    }
    
    if (vitoria1 > vitoria2 ) {
        printf("vitoria jogador 1 \n");
    } else {
        printf("vitoria jogador 2\n");
    
    }

    return 0;
}
