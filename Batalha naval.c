#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> // Para usar o tipo booleano (true/false)

// --- Constantes do Jogo ---
#define TAMANHO_TABULEIRO 10 // Tabuleiro 10x10
#define AGUA '~'             // Representa a água
#define NAVIO 'N'            // Representa uma parte de um navio
#define ACERTO 'X'           // Representa um acerto em um navio
#define ERRO 'O'             // Representa um tiro na água (erro)

// Estrutura para representar os tipos de navios
typedef struct {
    char id;
    int tamanho;
    int quantidade;
    int afundados; // Contador de quantos navios desse tipo foram afundados
} TipoNavio;

// Definindo os tipos de navios
TipoNavio tipos_navios[] = {
    {'P', 5, 1, 0}, // Porta-aviões (5 unidades)
    {'E', 4, 1, 0}, // Encouraçado (4 unidades)
    {'S', 3, 2, 0}, // Submarino (3 unidades, 2 unidades)
    {'C', 2, 2, 0}  // Contratorpedeiro (2 unidades, 2 unidades)
};
#define NUM_TIPOS_NAVIOS (sizeof(tipos_navios) / sizeof(TipoNavio))

// --- Protótipos de Funções ---
void inicializarTabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void exibirTabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], bool mostrarNavios);
bool posicionarNavio(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int tamanho, char orientacao, char id_navio);
void posicionarNaviosAleatoriamente(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
bool atirar(char tabuleiro_alvo[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_jogador_visual[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int *navios_restantes);
int contarNaviosRestantes(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
bool todosNaviosAfundados(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

// --- Função Principal ---
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    char tabuleiro_jogador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tabuleiro_computador[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tabuleiro_computador_visual[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]; // O que o jogador vê do tabuleiro do computador

    int navios_restantes_jogador;
    int navios_restantes_computador;

    // Inicializa os tabuleiros
    inicializarTabuleiro(tabuleiro_jogador);
    inicializarTabuleiro(tabuleiro_computador);
    inicializarTabuleiro(tabuleiro_computador_visual); // Inicializa com água

    printf("--- Bem-vindo ao Batalha Naval! ---\n");

    // --- Posicionamento dos Navios do Jogador ---
    printf("\n--- Posicione seus navios ---\n");
    exibirTabuleiro(tabuleiro_jogador, true); // Exibe seu tabuleiro limpo para posicionar

    // Loop para posicionar todos os navios do jogador
    for (int i = 0; i < NUM_TIPOS_NAVIOS; i++) {
        for (int j = 0; j < tipos_navios[i].quantidade; j++) {
            int linha, coluna;
            char orientacao;
            bool posicionado = false;
            while (!posicionado) {
                printf("Posicione %s (tamanho %d, '%c'). Navio %d de %d.\n",
                       (tipos_navios[i].id == 'P' ? "Porta-aviões" :
                        tipos_navios[i].id == 'E' ? "Encouraçado" :
                        tipos_navios[i].id == 'S' ? "Submarino" : "Contratorpedeiro"),
                       tipos_navios[i].tamanho, tipos_navios[i].id, j + 1, tipos_navios[i].quantidade);
                printf("Digite a linha (0-%d): ", TAMANHO_TABULEIRO - 1);
                scanf("%d", &linha);
                printf("Digite a coluna (0-%d): ", TAMANHO_TABULEIRO - 1);
                scanf("%d", &coluna);
                printf("Digite a orientação (H para horizontal, V para vertical): ");
                scanf(" %c", &orientacao); // Espaço antes de %c para consumir o newline anterior

                // Converte para maiúscula
                if (orientacao >= 'a' && orientacao <= 'z') {
                    orientacao -= 32;
                }

                if (linha < 0 || linha >= TAMANHO_TABULEIRO ||
                    coluna < 0 || coluna >= TAMANHO_TABULEIRO ||
                    (orientacao != 'H' && orientacao != 'V')) {
                    printf("Entrada inválida. Tente novamente.\n");
                    continue;
                }

                posicionado = posicionarNavio(tabuleiro_jogador, linha, coluna, tipos_navios[i].tamanho, orientacao, tipos_navios[i].id);
                if (!posicionado) {
                    printf("Não foi possível posicionar o navio aí. Tente novamente.\n");
                } else {
                    printf("Navio posicionado!\n");
                    exibirTabuleiro(tabuleiro_jogador, true);
                }
            }
        }
    }

    // --- Posicionamento dos Navios do Computador ---
    printf("\nPosicionando navios do computador...\n");
    posicionarNaviosAleatoriamente(tabuleiro_computador);
    printf("Navios do computador posicionados!\n");

    // Contar o número inicial de navios
    navios_restantes_jogador = contarNaviosRestantes(tabuleiro_jogador);
    navios_restantes_computador = contarNaviosRestantes(tabuleiro_computador);

    // --- Início do Jogo ---
    while (navios_restantes_jogador > 0 && navios_restantes_computador > 0) {
        printf("\n--- SEU TURNO ---\n");
        printf("Seu tabuleiro:\n");
        exibirTabuleiro(tabuleiro_jogador, true); // Exibe seu próprio tabuleiro com seus navios
        printf("\nTabuleiro do computador (seus tiros):\n");
        exibirTabuleiro(tabuleiro_computador_visual, false); // Exibe o que você vê do computador

        int linha_tiro_jogador, coluna_tiro_jogador;
        bool tiro_valido = false;
        while (!tiro_valido) {
            printf("Digite a linha para atirar (0-%d): ", TAMANHO_TABULEIRO - 1);
            scanf("%d", &linha_tiro_jogador);
            printf("Digite a coluna para atirar (0-%d): ", TAMANHO_TABULEIRO - 1);
            scanf("%d", &coluna_tiro_jogador);

            if (linha_tiro_jogador < 0 || linha_tiro_jogador >= TAMANHO_TABULEIRO ||
                coluna_tiro_jogador < 0 || coluna_tiro_jogador >= TAMANHO_TABULEIRO) {
                printf("Coordenadas inválidas. Tente novamente.\n");
            } else if (tabuleiro_computador_visual[linha_tiro_jogador][coluna_tiro_jogador] == ACERTO ||
                       tabuleiro_computador_visual[linha_tiro_jogador][coluna_tiro_jogador] == ERRO) {
                printf("Você já atirou nessas coordenadas. Tente novamente.\n");
            } else {
                tiro_valido = true;
            }
        }

        if (atirar(tabuleiro_computador, tabuleiro_computador_visual, linha_tiro_jogador, coluna_tiro_jogador, &navios_restantes_computador)) {
            printf("Acertou! Navios do computador restantes: %d\n", navios_restantes_computador);
        } else {
            printf("Errou! Navios do computador restantes: %d\n", navios_restantes_computador);
        }

        if (navios_restantes_computador == 0) {
            printf("\nParabéns! Você afundou todos os navios do computador. VOCÊ VENCEU!\n");
            break;
        }

        printf("\n--- TURNO DO COMPUTADOR ---\n");
        int linha_tiro_computador, coluna_tiro_computador;
        bool tiro_computador_valido = false;

        // Lógica simples para o computador: atira aleatoriamente e não repete tiro
        while (!tiro_computador_valido) {
            linha_tiro_computador = rand() % TAMANHO_TABULEIRO;
            coluna_tiro_computador = rand() % TAMANHO_TABULEIRO;

            if (tabuleiro_jogador[linha_tiro_computador][coluna_tiro_computador] != ACERTO &&
                tabuleiro_jogador[linha_tiro_computador][coluna_tiro_computador] != ERRO) {
                tiro_computador_valido = true;
            }
        }

        printf("Computador atirou em (%d, %d).\n", linha_tiro_computador, coluna_tiro_computador);
        if (atirar(tabuleiro_jogador, tabuleiro_jogador, linha_tiro_computador, coluna_tiro_computador, &navios_restantes_jogador)) {
            printf("O computador acertou um de seus navios! Navios seus restantes: %d\n", navios_restantes_jogador);
        } else {
            printf("O computador errou! Navios seus restantes: %d\n", navios_restantes_jogador);
        }

        if (navios_restantes_jogador == 0) {
            printf("\nQue pena! O computador afundou todos os seus navios. O COMPUTADOR VENCEU!\n");
            break;
        }
    }

    printf("\n--- Fim do Jogo ---\n");
    printf("\nSeu tabuleiro final:\n");
    exibirTabuleiro(tabuleiro_jogador, true);
    printf("\nTabuleiro final do computador (seus tiros):\n");
    exibirTabuleiro(tabuleiro_computador, true); // Revela o tabuleiro do computador no final

    return 0;
}

// --- Implementação das Funções ---

// Inicializa o tabuleiro com água
void inicializarTabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Exibe o tabuleiro
void exibirTabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], bool mostrarNavios) {
    printf("  ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j); // Colunas
    }
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d ", i); // Linhas
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (!mostrarNavios && (tabuleiro[i][j] >= 'A' && tabuleiro[i][j] <= 'Z')) {
                // Se não for para mostrar navios e for um caractere de navio, exibe água
                printf("%c ", AGUA);
            } else {
                printf("%c ", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }
}

// Tenta posicionar um navio no tabuleiro. Retorna true se conseguiu, false caso contrário.
bool posicionarNavio(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int tamanho, char orientacao, char id_navio) {
    // Verifica se o navio cabe no tabuleiro e se não há colisão
    if (orientacao == 'H') { // Horizontal
        if (coluna + tamanho > TAMANHO_TABULEIRO) return false; // Sai do tabuleiro
        for (int j = 0; j < tamanho; j++) {
            if (tabuleiro[linha][coluna + j] != AGUA) return false; // Colisão
        }
        // Se tudo ok, posiciona
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[linha][coluna + j] = id_navio;
        }
    } else { // Vertical
        if (linha + tamanho > TAMANHO_TABULEIRO) return false; // Sai do tabuleiro
        for (int i = 0; i < tamanho; i++) {
            if (tabuleiro[linha + i][coluna] != AGUA) return false; // Colisão
        }
        // Se tudo ok, posiciona
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linha + i][coluna] = id_navio;
        }
    }
    return true;
}

// Posiciona os navios do computador aleatoriamente
void posicionarNaviosAleatoriamente(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < NUM_TIPOS_NAVIOS; i++) {
        for (int j = 0; j < tipos_navios[i].quantidade; j++) {
            bool posicionado = false;
            while (!posicionado) {
                int linha = rand() % TAMANHO_TABULEIRO;
                int coluna = rand() % TAMANHO_TABULEIRO;
                char orientacao = (rand() % 2 == 0) ? 'H' : 'V'; // 0 para Horizontal, 1 para Vertical

                posicionado = posicionarNavio(tabuleiro, linha, coluna, tipos_navios[i].tamanho, orientacao, tipos_navios[i].id);
            }
        }
    }
}

// Realiza um tiro. Retorna true se acertou um navio, false se errou.
bool atirar(char tabuleiro_alvo[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], char tabuleiro_jogador_visual[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int *navios_restantes) {
    if (tabuleiro_alvo[linha][coluna] == AGUA) {
        tabuleiro_alvo[linha][coluna] = ERRO;
        tabuleiro_jogador_visual[linha][coluna] = ERRO;
        return false;
    } else if (tabuleiro_alvo[linha][coluna] == NAVIO || (tabuleiro_alvo[linha][coluna] >= 'A' && tabuleiro_alvo[linha][coluna] <= 'Z')) {
        // Se for um navio (qualquer ID de navio ou 'N')
        tabuleiro_alvo[linha][coluna] = ACERTO;
        tabuleiro_jogador_visual[linha][coluna] = ACERTO;
        // Opcional: verificar se o navio foi afundado
        *navios_restantes = contarNaviosRestantes(tabuleiro_alvo); // Reconta os navios restantes
        return true;
    }
    return false; // Caso já tenha sido atingido, etc. (não deveria acontecer com a validação)
}

// Conta o número de partes de navios ainda não atingidas no tabuleiro
int contarNaviosRestantes(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int count = 0;
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] != AGUA && tabuleiro[i][j] != ACERTO && tabuleiro[i][j] != ERRO) {
                count++;
            }
        }
    }
    return count;
}

// Verifica se todos os navios foram afundados (não usado diretamente no main, mas útil para o futuro)
bool todosNaviosAfundados(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == NAVIO || (tabuleiro[i][j] >= 'A' && tabuleiro[i][j] <= 'Z')) {
                return false; // Ainda existe uma parte de navio
            }
        }
    }
    return true; // Todos os navios foram afundados
}
