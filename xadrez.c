#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8

typedef enum { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING } PieceType;
typedef enum { NONE, WHITE, BLACK } Color;

typedef struct {
    PieceType type;
    Color color;
} Piece;

Piece board[BOARD_SIZE][BOARD_SIZE];

// Inicializa o tabuleiro
void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = (Piece){EMPTY, NONE};

    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = (Piece){PAWN, BLACK};
        board[6][i] = (Piece){PAWN, WHITE};
    }

    board[0][0] = board[0][7] = (Piece){ROOK, BLACK};
    board[7][0] = board[7][7] = (Piece){ROOK, WHITE};
    board[0][1] = board[0][6] = (Piece){KNIGHT, BLACK};
    board[7][1] = board[7][6] = (Piece){KNIGHT, WHITE};
    board[0][2] = board[0][5] = (Piece){BISHOP, BLACK};
    board[7][2] = board[7][5] = (Piece){BISHOP, WHITE};
    board[0][3] = (Piece){QUEEN, BLACK};
    board[7][3] = (Piece){QUEEN, WHITE};
    board[0][4] = (Piece){KING, BLACK};
    board[7][4] = (Piece){KING, WHITE};
}

char getPieceChar(Piece piece) {
    if (piece.color == WHITE) {
        switch (piece.type) {
            case PAWN: return 'P';
            case ROOK: return 'R';
            case KNIGHT: return 'N';
            case BISHOP: return 'B';
            case QUEEN: return 'Q';
            case KING: return 'K';
            default: return '.';
        }
    } else if (piece.color == BLACK) {
        switch (piece.type) {
            case PAWN: return 'p';
            case ROOK: return 'r';
            case KNIGHT: return 'n';
            case BISHOP: return 'b';
            case QUEEN: return 'q';
            case KING: return 'k';
            default: return '.';
        }
    }
    return '.';
}

void printBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", getPieceChar(board[i][j]));
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

// Converte "e2" -> linha=6, coluna=4
int parsePosition(char *pos, int *row, int *col) {
    if (strlen(pos) != 2) return 0;
    *col = pos[0] - 'a';
    *row = 8 - (pos[1] - '0');
    return (*col >= 0 && *col < 8 && *row >= 0 && *row < 8);
}

int movePiece(int fromRow, int fromCol, int toRow, int toCol, Color turn) {
    Piece src = board[fromRow][fromCol];
    if (src.type == EMPTY || src.color != turn) {
        printf("Movimento inválido. Tente novamente.\n");
        return 0;
    }

    board[toRow][toCol] = src;
    board[fromRow][fromCol] = (Piece){EMPTY, NONE};
    return 1;
}

int main() {
    char from[3], to[3];
    int fromRow, fromCol, toRow, toCol;
    Color turn = WHITE;

    initBoard();

    while (1) {
        printBoard();
        printf("Vez do %s. Digite o movimento (ex: e2 e4): ", turn == WHITE ? "Branco" : "Preto");
        scanf("%2s %2s", from, to);

        if (!parsePosition(from, &fromRow, &fromCol) || !parsePosition(to, &toRow, &toCol)) {
            printf("Posição inválida.\n");
            continue;
        }

        if (movePiece(fromRow, fromCol, toRow, toCol, turn)) {
            // Alterna o turno
            turn = (turn == WHITE) ? BLACK : WHITE;
        }
    }

    return 0;
}
