/************************| includs |**************************************/
#include "../Aplication/inc/snake_game.h"

/************************| struct |***************************************/
typedef struct {
    int x;
    int y;
} SnakeSegment;

/************************| funcoes |*************************************/
void play_snake_game(JoystickState *js, int difficulty) {
    while (1) {  // Outer loop for replay
        // Configura a velocidade com base na dificuldade
        int speed;
        switch (difficulty) {
            case 0: speed = SNAKE_SPEED_EASY; break;
            case 1: speed = SNAKE_SPEED_MEDIUM; break;
            case 2: speed = SNAKE_SPEED_HARD; break;
            default: speed = SNAKE_SPEED_EASY; break;
        }

        // Inicializa a cobrinha
        SnakeSegment snake[100]; // Máximo de 100 segmentos
        int snake_length = 3;    // Comprimento inicial da cobrinha
        snake[0] = (SnakeSegment){8, 4}; // Cabeça da cobrinha no centro da área permitida
        snake[1] = (SnakeSegment){7, 4};
        snake[2] = (SnakeSegment){6, 4};

        // Inicializa o fruto
        SnakeSegment fruit = {rand() % 16, rand() % 8}; // Coordenadas do fruto (16x8 grid no OLED)

        // Direção inicial da cobrinha
        int dx = 1, dy = 0; // Movendo para a direita inicialmente

        // Define os limites do retângulo seguro (margens)
        int min_x = 1, max_x = 14; 
        int min_y = 1, max_y = 6;

        // Main game loop
        while (1) {
            // Atualiza o OLED com as instruções
            ssd_1306_fill(black);
            ssd_1306_set_cursor(0, 0);
            ssd_1306_write_string("Cobrinha!", Font_6x8, white);

            // Lê o estado do joystick e aplica filtro nos eixos
            Joystick_Read(js);
            js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
            js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

            // Atualiza a direção com base no joystick, evitando reversão 180°
            if (js->x_filtered > ADC_UPPER_THRESHOLD_X && dx != -1) {
                dx = 1;
                dy = 0;
            } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && dx != 1) {
                dx = -1;
                dy = 0;
            } else if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && dy != -1) {
                dx = 0;
                dy = 1;
            } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && dy != 1) {
                dx = 0;
                dy = -1;
            }

            // Move a cobrinha: desloca os segmentos e atualiza a cabeça
            for (int i = snake_length - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }
            snake[0].x += dx;
            snake[0].y += dy;

            // Verifica colisão com as bordas do retângulo permitido
            if (snake[0].x < min_x || snake[0].x > max_x || 
                snake[0].y < min_y || snake[0].y > max_y) {
                break; // Entra no processo de "Game Over"
            }

            // Verifica colisão com o próprio corpo
            for (int i = 1; i < snake_length; i++) {
                if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                    // Reduz o tamanho da cobrinha em 1 segmento
                    snake_length = (snake_length > 1) ? (snake_length - 1) : 1;
                    break;
                }
            }

            // Verifica se a cobrinha comeu o fruto
            if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                snake_length++; // Aumenta o tamanho da cobrinha
                fruit.x = rand() % 16;
                fruit.y = rand() % 8;
            }

            // Desenha a cobrinha no OLED
            for (int i = 0; i < snake_length; i++) {
                ssd_1306_set_cursor(snake[i].x * 6, snake[i].y * 8); // Cada célula é 6x8 pixels
                ssd_1306_write_string("O", Font_6x8, white);
            }

            // Desenha o fruto no OLED
            ssd_1306_set_cursor(fruit.x * 6, fruit.y * 8);
            ssd_1306_write_string("X", Font_6x8, white);

            // Atualiza o display
            ssd_1306_up_date_screen();

            // Aguarda antes do próximo movimento
            sleep_ms(speed);
        }  // Fim do loop do jogo (Game Over)

        // Exibe "Game Over!" e o prompt para jogar novamente
        ssd_1306_fill(black);
        ssd_1306_set_cursor(0, 10);
        ssd_1306_write_string("Game Over!", Font_6x8, white);
        
        int option = 0; // 0 = Sim, 1 = Nao
        while (1) {
            ssd_1306_fill(black);
            ssd_1306_set_cursor(0, 10);
            ssd_1306_write_string("Jogar novamente?", Font_6x8, white);
            if (option == 0) {
                ssd_1306_set_cursor(0, 30);
                ssd_1306_write_string("> Sim", Font_6x8, white);
                ssd_1306_set_cursor(0, 40);
                ssd_1306_write_string("  Nao", Font_6x8, white);
            } else {
                ssd_1306_set_cursor(0, 30);
                ssd_1306_write_string("  Sim", Font_6x8, white);
                ssd_1306_set_cursor(0, 40);
                ssd_1306_write_string("> Nao", Font_6x8, white);
            }
            ssd_1306_up_date_screen();

            // Lê o estado do joystick para alteração na seleção
            Joystick_Read(js);
            // Alterar seleção pelo eixo Y
            if (js->y_raw < (AVERAGE_VALUE - DEADZONE_THRESHOLD)) {
                option = 0;
            } else if (js->y_raw > (AVERAGE_VALUE + DEADZONE_THRESHOLD)) {
                option = 1;
            }

            // Confirmação da seleção ao pressionar o botão do joystick
            if (_joystick_read_button()) {
                break;
            }
            sleep_ms(150);
        }

        // Se o usuário escolher "Nao", retorna para a tela principal
        if (option == 1) {
            break;
        }
        // Caso contrário, o jogo reinicia (continua o loop externo)
    }
}