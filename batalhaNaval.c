#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ROWS 10         // Linhas do tabuleiro
#define COLS 10         // Colunas do tabuleiro
#define SHIP_SIZE 3     // Tamanho fixo de cada navio
#define WATER 0         // Representa água
#define SHIP 3          // Representa parte de um navio
#define EFFECT 5        // Representa área de efeito de habilidade

// Tamanho das matrizes de habilidade (usamos 5x5)
#define ABIL_SIZE 5

// Inicializa o tabuleiro com água
void initialize_board(int board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = WATER;
}

// Verifica se é possível posicionar um navio sem sair dos limites e sem sobreposição
bool can_place_ship(int board[ROWS][COLS], int sr, int sc, char o) {
    int dr = 0, dc = 0;
    switch(o) {
        case 'H': dr = 0; dc = 1; break;  // Horizontal
        case 'V': dr = 1; dc = 0; break;  // Vertical
        case 'D': dr = 1; dc = 1; break;  // Diagonal principal (down-right)
        case 'A': dr = 1; dc = -1; break; // Diagonal secundária (down-left)
        default: return false;
    }
    for (int i = 0; i < SHIP_SIZE; i++) {
        int r = sr + i*dr;
        int c = sc + i*dc;
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
        if (board[r][c] != WATER) return false;
    }
    return true;
}

// Posiciona o navio no tabuleiro
void place_ship(int board[ROWS][COLS], int sr, int sc, char o) {
    int dr = 0, dc = 0;
    switch(o) {
        case 'H': dr = 0; dc = 1; break;
        case 'V': dr = 1; dc = 0; break;
        case 'D': dr = 1; dc = 1; break;
        case 'A': dr = 1; dc = -1; break;
    }
    for (int i = 0; i < SHIP_SIZE; i++) {
        board[sr + i*dr][sc + i*dc] = SHIP;
    }
}

// Constrói as matrizes de habilidade dinamicamente
void build_abilities(int cone[ABIL_SIZE][ABIL_SIZE],
                     int cross[ABIL_SIZE][ABIL_SIZE],
                     int diamond[ABIL_SIZE][ABIL_SIZE]) {
    int center = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; i++) {
        for (int j = 0; j < ABIL_SIZE; j++) {
            // Cone apontando para baixo
            if (i <= center && abs(j - center) <= i)
                cone[i][j] = 1;
            else
                cone[i][j] = 0;
            // Cruz
            if (i == center || j == center)
                cross[i][j] = 1;
            else
                cross[i][j] = 0;
            // Octaedro (losango)
            if (abs(i - center) + abs(j - center) <= center)
                diamond[i][j] = 1;
            else
                diamond[i][j] = 0;
        }
    }
}

// Aplica uma matriz de habilidade sobre o tabuleiro
void apply_ability(int board[ROWS][COLS],
                   int ability[ABIL_SIZE][ABIL_SIZE],
                   int origin_r, int origin_c) {
    int offset = ABIL_SIZE / 2;
    for (int i = 0; i < ABIL_SIZE; i++) {
        for (int j = 0; j < ABIL_SIZE; j++) {
            if (ability[i][j] == 1) {
                int r = origin_r + (i - offset);
                int c = origin_c + (j - offset);
                if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                    board[r][c] = EFFECT;
                }
            }
        }
    }
}

// Exibe o tabuleiro com caracteres: '~'=água, 'N'=navio, '*'=área de efeito
void print_board(int board[ROWS][COLS]) {
    printf("   ");
    for (int c = 0; c < COLS; c++)
        printf(" %d ", c);
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        printf("%2d ", r);
        for (int c = 0; c < COLS; c++) {
            char symbol;
            if (board[r][c] == WATER)
                symbol = '~';   // Água
            else if (board[r][c] == SHIP)
                symbol = 'N';   // Navio
            else if (board[r][c] == EFFECT)
                symbol = '*';   // Área afetada
            else
                symbol = '?';   // Valor inesperado
            printf(" %c ", symbol);
        }
        printf("\n");
    }
}

int main() {
    int board[ROWS][COLS];
    initialize_board(board);

    // 1) Posiciona navios (exemplos)
    int ships[4][3] = {{1,2,'H'}, {4,5,'V'}, {6,1,'D'}, {2,8,'A'}};
    for (int s = 0; s < 4; s++) {
        int r = ships[s][0], c = ships[s][1];
        char o = ships[s][2];
        if (!can_place_ship(board, r, c, o)) {
            printf("Erro: navio %d inválido em (%d,%d)\n", s+1, r, c);
            return 1;
        }
        place_ship(board, r, c, o);
    }

    // 2) Monta matrizes de habilidade
    int cone[ABIL_SIZE][ABIL_SIZE];
    int cross[ABIL_SIZE][ABIL_SIZE];
    int diamond[ABIL_SIZE][ABIL_SIZE];
    build_abilities(cone, cross, diamond);

    // 3) Define origens das habilidades
    int cone_r = 3, cone_c = 4;
    int cross_r = 7, cross_c = 2;
    int dia_r = 5, dia_c = 8;

    // 4) Aplica habilidades ao tabuleiro
    apply_ability(board, cone, cone_r, cone_c);
    apply_ability(board, cross, cross_r, cross_c);
    apply_ability(board, diamond, dia_r, dia_c);

    // 5) Exibe o tabuleiro final
    print_board(board);
    return 0;
}
