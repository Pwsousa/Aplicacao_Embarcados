/************************| includs |**************************************/
#include "inc/ssd_1306.h"
#include "inc/ssd_1306_conf.h"
#include "inc/ssd_1306_font.h"
#include "inc/_button.h"
#include "inc/joystick.h"
#include "inc/jogo_da_velha.h"
#include "inc/led_sequence_game.h"
#include "inc/snake_game.h"

/************************| define |*****************************************/
#define MENU_OPTIONS 3

/************************| List of game |***********************************/
const char *game_names[MENU_OPTIONS] = {
    "Jogo da Velha",
    "Sequencia Correta",
    "Snake"
};

/************************| Prototipos |*************************************/
int show_menu(JoystickState *js, const char *game_names[]);
void start_countdown();

int main() {
    /*inicializacao*/
    stdio_init_all();
    ssd_1306_init();
    button_init();
    LedMatrix_Init(&ws2812_config); 
    JoystickState js = {0};
    Joystick_Init(&js);

    int game = show_menu(&js, game_names);
    start_countdown();

    switch (game) {
        case 0: {
            play_tic_tac_toe();
        } break;
        case 1: {
            play_led_sequence_game(&js);
        } break;
        case 2:{
            int dificuldade = show_menu(&js,(const char *[]){"Facil", "Medio", "Dificil"});
            play_snake_game(&js, dificuldade);
        }break;
    }
    return 0;
}

/************************| funcoes |**************************************/
int show_menu(JoystickState *js, const char *game_names[]) {
    int selected_option = 0;
    while (1) {
        // Limpa a tela do OLED
        ssd_1306_fill(black);

        // Exibe as opções do menu
        for (int i = 0; i < MENU_OPTIONS; i++) {
            // Define o cursor para a linha apropriada (offset de 10 pixels para cada opção)
            ssd_1306_set_cursor(0, i * 10 + 10);
            if (i == selected_option) {
                // Destaca a opção selecionada
                ssd_1306_write_string("> ", Font_6x8, white);
            } else {
                ssd_1306_write_string("  ", Font_6x8, white);
            }
            ssd_1306_write_string(game_names[i], Font_6x8, white);
        }

        // Atualiza o display OLED
        ssd_1306_up_date_screen();

        // Lê o estado atual do joystick
        Joystick_Read(js);

        // Atualiza a seleção com base no eixo Y do joystick
        if (js->y_raw < (AVERAGE_VALUE - DEADZONE_THRESHOLD)) {
            if (selected_option > 0) {
                selected_option--;
                sleep_ms(200); // Delay para evitar mudanças muito rápidas
            }
        } else if (js->y_raw > (AVERAGE_VALUE + DEADZONE_THRESHOLD)) {
            if (selected_option < MENU_OPTIONS - 1) {
                selected_option++;
                sleep_ms(200);
            }
        }

        // Verifica se o botão do joystick foi pressionado para confirmar a seleção
        if (_joystick_read_button()) {
            return selected_option;
        }
    }
}

void start_countdown() {
    // Display a 10-second countdown on the OLED screen
    for (int i = 10; i > 0; i--) {
        ssd_1306_fill(black); // Clear the screen
        ssd_1306_set_cursor(0, 20);
        ssd_1306_write_string("Starting in:", Font_6x8, white);
        ssd_1306_set_cursor(50, 30);

        char countdown[3];
        snprintf(countdown, sizeof(countdown), "%d", i); // Convert the countdown number to a string
        ssd_1306_write_string(countdown, Font_6x8, white);

        ssd_1306_up_date_screen();  // Update the screen
        sleep_ms(1000);             // Wait for 1 second
    }

    // Clear the screen after the countdown
    ssd_1306_fill(black);
    ssd_1306_up_date_screen();
}