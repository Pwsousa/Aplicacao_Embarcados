#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "Sistemas_Embarcados/display/inc/ssd_1306.h"
#include "Sistemas_Embarcados/display/inc/ssd_1306_font.h"
#include "Sistemas_Embarcados/joystick/inc/joystick.h"

// Definições da tela e do tabuleiro
#define SCREEN_WIDTH 127
#define SCREEN_HEIGHT 63

// Posições para desenhar o grid do jogo (valores experimentais)
#define VERT_LINE1 42
#define VERT_LINE2 85
#define HORZ_LINE1 22
#define HORZ_LINE2 45

// Cores (supondo que white e black estejam definidos na biblioteca)
#define white 1
#define black 0

// Definições dos pinos dos botões para navegação do menu
#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

// Definição do botão do joystick (para confirmação)
#define BUTTON_JOYSTICK 22

// Constantes de direção do joystick (retornadas pela função get_joystick_direction)
#define JOY_NONE   0
#define JOY_UP     1
#define JOY_DOWN   2
#define JOY_LEFT   3
#define JOY_RIGHT  4
#define JOY_SELECT 5

char board[3][3];  
int sel_row = 1, sel_col = 1;  
int blink_state = 0;  

// Protótipos das funções dos botões
bool read_button_A(void);  
bool read_button_B(void);  

// Protótipo para leitura dos eixos do joystick
int get_joystick_direction(void);

void draw_menu(void);
int menu_selection(void);
void clear_board(void);
void draw_board(void);
void get_cell_center(int row, int col, int *center_x, int *center_y);
void drawX(int cell_row, int cell_col);
void drawO(int cell_row, int cell_col);
void draw_moves(void);
void update_board_screen(int current_player);
int check_win(void);
int board_full(void);

// Implementação das funções dos botões utilizando gpio_get (considerando lógica invertida)
bool read_button_A(void) {
    return (!gpio_get(BUTTON_PIN_A));
}

bool read_button_B(void) {
    return (!gpio_get(BUTTON_PIN_B));
}

int get_joystick_direction(void) {
    uint16_t x, y;
    joystick_read_x(&x);
    joystick_read_y(&y);

    const uint16_t threshold_low = 1500;
    const uint16_t threshold_high = 2500;

    if (x < threshold_low) return JOY_LEFT;
    if (x > threshold_high) return JOY_RIGHT;
    if (y < threshold_low) return JOY_UP;
    if (y > threshold_high) return JOY_DOWN;
    return JOY_NONE;
}

// Desenha o menu na tela SSD_1306
void draw_menu(void) {
    ssd_1306_fill(black);
    ssd_1306_set_cursor(20, 5);
    ssd_1306_write_string("JOGO DA VELHA", Font_7x10, white);
    ssd_1306_draw_line(0, 20, 163, 20, white);  // Linha decorativa
    ssd_1306_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, white);
    
    ssd_1306_set_cursor(20, 27);
    ssd_1306_write_string("1 Jogador", Font_7x10, white);
    ssd_1306_set_cursor(20, 40);
    ssd_1306_write_string("2 Jogador", Font_7x10, white);
    ssd_1306_up_date_screen();
}

// Função de delay (usada sleep_ms da Pico SDK)
void delay_ms(int ms) {
    sleep_ms(ms);
}

// Menu de seleção usando switch-case e os botões A e B
int menu_selection(void) {
    int option = 1;  // Inicia com opção 1 selecionada
    bool confirmed = false;
    
    while (!confirmed) {
        draw_menu();
        // Destaque a opção selecionada usando inversão de retângulo
        if (option == 1) {
            ssd_1306_inverte_retangulo(18, 25, 100, 35);
        } else {
            ssd_1306_inverte_retangulo(18, 38, 100, 48);
        }
        ssd_1306_up_date_screen();
        
        delay_ms(200);  // Debounce e tempo para visualização
        
        // Lê o estado dos botões e gera um valor:
        // btn_state: 1 (apenas A), 2 (apenas B), 3 (ambos pressionados), 0 (nenhum)
        int btn_state = (read_button_A() ? 1 : 0) + (read_button_B() ? 2 : 0);
        
        switch (btn_state) {
            case 1: // Apenas botão A pressionado: seleciona opção 1
                option = 1;
                break;
            case 2: // Apenas botão B pressionado: seleciona opção 2
                option = 2;
                break;
            case 3: // Ambos pressionados: confirma a seleção
                confirmed = true;
                break;
            default:
                // Nenhum botão pressionado: nenhuma ação
                break;
        }
    }
    return option;
}

void clear_board(void) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

void draw_board(void) {
    ssd_1306_draw_line(VERT_LINE1, 0, VERT_LINE1, SCREEN_HEIGHT, white);
    ssd_1306_draw_line(VERT_LINE2, 0, VERT_LINE2, SCREEN_HEIGHT, white);
    ssd_1306_draw_line(0, HORZ_LINE1, SCREEN_WIDTH, HORZ_LINE1, white);
    ssd_1306_draw_line(0, HORZ_LINE2, SCREEN_WIDTH, HORZ_LINE2, white);
}

void get_cell_center(int row, int col, int *center_x, int *center_y) {
    int x_start, x_end, y_start, y_end;
    if (col == 0) { x_start = 0; x_end = VERT_LINE1; }
    else if (col == 1) { x_start = VERT_LINE1; x_end = VERT_LINE2; }
    else { x_start = VERT_LINE2; x_end = SCREEN_WIDTH; }
    
    if (row == 0) { y_start = 0; y_end = HORZ_LINE1; }
    else if (row == 1) { y_start = HORZ_LINE1; y_end = HORZ_LINE2; }
    else { y_start = HORZ_LINE2; y_end = SCREEN_HEIGHT; }
    
    *center_x = (x_start + x_end) / 2;
    *center_y = (y_start + y_end) / 2;
}

void drawX(int cell_row, int cell_col) {
    int x_start, x_end, y_start, y_end;
    int margin = 3;
    
    if (cell_col == 0) { x_start = 0; x_end = VERT_LINE1; }
    else if (cell_col == 1) { x_start = VERT_LINE1; x_end = VERT_LINE2; }
    else { x_start = VERT_LINE2; x_end = SCREEN_WIDTH; }
    
    if (cell_row == 0) { y_start = 0; y_end = HORZ_LINE1; }
    else if (cell_row == 1) { y_start = HORZ_LINE1; y_end = HORZ_LINE2; }
    else { y_start = HORZ_LINE2; y_end = SCREEN_HEIGHT; }
    
    ssd_1306_draw_line(x_start + margin, y_start + margin, x_end - margin, y_end - margin, white);
    ssd_1306_draw_line(x_start + margin, y_end - margin, x_end - margin, y_start + margin, white);
}

void drawO(int cell_row, int cell_col) {
    int center_x, center_y;
    get_cell_center(cell_row, cell_col, &center_x, &center_y);
    int radius = 6;
    ssd_1306_draw_circle(center_x, center_y, radius, white);
}

void draw_moves(void) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == 'X')
                drawX(i, j);
            else if (board[i][j] == 'O')
                drawO(i, j);
}

void update_board_screen(int current_player) {
    ssd_1306_fill(black);
    draw_board();
    draw_moves();
    if (board[sel_row][sel_col] == ' ') {
        if (blink_state) {
            if (current_player == 0)
                drawX(sel_row, sel_col);
            else
                drawO(sel_row, sel_col);
        }
    }
    ssd_1306_up_date_screen();
}

int check_win(void) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return 1;
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return 1;
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return 1;
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return 1;
    return 0;
}

int board_full(void) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

/******************************* MAIN *************************************/
int main(void) {
    int mode, current_player = 0;  // 0 para 'X', 1 para 'O'
    int joy;
    
    ssd_1306_init();
    joystick_init();
    
    // Seleção de modo via botões A e B com switch-case
    mode = menu_selection();
    
    // Se modo 1 for selecionado, exibe aviso e utiliza o modo 2 (exemplo)
    if (mode == 1) {
        printf("Modo 1 Jogador nao implementado. Usando modo 2 Jogador.\n");
    }
    
    clear_board();
    sel_row = 1;
    sel_col = 1;
    
    while (1) {
        blink_state = !blink_state;
        update_board_screen(current_player);
        sleep_ms(300);
        
        joy = get_joystick_direction();
        switch (joy) {
            case JOY_UP:
                if (sel_row > 0) sel_row--;
                break;
            case JOY_DOWN:
                if (sel_row < 2) sel_row++;
                break;
            case JOY_LEFT:
                if (sel_col > 0) sel_col--;
                break;
            case JOY_RIGHT:
                if (sel_col < 2) sel_col++;
                break;
            case JOY_SELECT:
                if (board[sel_row][sel_col] == ' ') {
                    board[sel_row][sel_col] = (current_player == 0) ? 'X' : 'O';
                    if (check_win()) {
                        update_board_screen(current_player);
                        printf("Jogador %c venceu!\n", (current_player == 0) ? 'X' : 'O');
                        return 0;
                    }
                    if (board_full()) {
                        update_board_screen(current_player);
                        printf("Empate!\n");
                        return 0;
                    }
                    current_player = 1 - current_player;
                    sel_row = 1;
                    sel_col = 1;
                }
                break;
            default:
                break;
        }
    }
    
    return 0;
}
