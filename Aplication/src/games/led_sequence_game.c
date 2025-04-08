/***********************************| includes| ***************************/
#include "../Aplication/inc/led_sequence_game.h"

/************************| funcoes |**************************************/
/**
 * @brief Moves a colored rectangle across an LED matrix.
 * 
 * This function animates a rectangle of specified dimensions and color
 * across an LED matrix. The rectangle moves through all possible positions
 * on the matrix, clearing and updating the display at each step.
 * 
 * @param x The width of the rectangle to be drawn. Must be within the bounds of the LED matrix.
 * @param y The height of the rectangle to be drawn. Must be within the bounds of the LED matrix.
 * @param red The intensity of the red color component for the rectangle.
 * @param green The intensity of the green color component for the rectangle.
 * @param blue The intensity of the blue color component for the rectangle.
 * 
 * @note If the rectangle dimensions are invalid (e.g., zero or exceeding the matrix bounds),
 *       the function will return without performing any operation.
 * 
 * @warning This function assumes that the LED matrix dimensions are defined by
 *          MAX_X_LED_MATRIX and MAX_Y_LED_MATRIX. Ensure these macros are properly set.
 * 
 * @warning The function uses a delay of 200 milliseconds between updates, which may
 *          block execution of other tasks in a non-multithreaded environment.
 */
void led_matrix_moving(int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue){
    if( (x == 0 && y == 0) || ((x > MAX_X_LED_MATRIX) && (y > MAX_Y_LED_MATRIX)) )
        return;

    for(int i = 0; i < MAX_X_LED_MATRIX; i++){
        for( int j = 0; j < MAX_Y_LED_MATRIX; j++){
            LedMatrix_Clear();
            LedMatrix_FillRectangle(i, j, x, y, red, green, blue);
            LedMatrix_Update();
            sleep_ms(200);
        }
    }
}

/**
 * @brief Displays a number on an LED matrix by lighting up specific pixels in a sequence.
 * 
 * This function uses a predefined mapping of pixel coordinates to represent numbers from 0 to 9
 * on an LED matrix. Each number is displayed by iterating through its corresponding pixel coordinates
 * and lighting them up with specific colors and intensities. After displaying the number, the LED matrix
 * is cleared.
 * 
 * @param numero The number to be displayed on the LED matrix (0-9). If the number is outside this range,
 *               the LED matrix will be cleared without displaying anything.
 * 
 * The function uses the following predefined color intensities:
 * - LED_MAX: Maximum intensity for the red channel.
 * - LED_WEAK: Weak intensity for the red, green, or blue channels.
 * - LED_OFF: Turns off the respective color channel.
 * 
 * The function performs the following steps:
 * 1. Selects the pixel coordinates corresponding to the input number from the `map_nunber` array.
 * 2. Iterates through the selected coordinates, lighting up the pixels with the specified color and intensity.
 * 3. Updates the LED matrix after each pixel is lit.
 * 4. Introduces a delay of 100 milliseconds between lighting up each pixel.
 * 5. Clears the LED matrix after the number is fully displayed.
 * 
 * @note The `LedMatrix_SetPixel` function is used to set the color and intensity of a specific pixel.
 *       The `LedMatrix_Update` function is called to apply the changes to the LED matrix.
 *       The `sleep_ms` function is used to introduce a delay between pixel updates.
 * 
 * Example usage:
 * @code
 * led_matrix_nuber(3); // Displays the number 3 on the LED matrix.
 * @endcode
 */
void led_matrix_nuber(int numero){

    int map_nunber[115][2] = {
        {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {2, 4}, {1, 4}, {1, 3}, {1, 2}, {1, 1},          // 0 -  12 -> ZERO
        {1, 3}, {2, 4}, {2, 3}, {2, 2}, {2, 1}, {2, 0},                                                          // 12 - 18 -> UM
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {2, 2}, {1, 2}, {1, 1}, {1, 0}, {2, 0}, {3, 0},                  // 18 - 30 -> DOIS
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 0}, {1, 0}, {1, 2}, {2, 2},                  // 30 - 41 -> TRES
        {1, 4}, {1, 3}, {1, 2}, {2, 2}, {3, 2}, {3, 3}, {3, 4}, {3, 1}, {3, 0},                                  // 41 - 50 -> QUATRO
        {3, 4}, {2, 4}, {1, 4}, {1, 3}, {1, 2}, {2, 2}, {3, 2}, {3, 1}, {3, 0}, {2, 0}, {1, 0},                  // 51 - 63 -> CINCO
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2},          // 63 - 76 -> SEIS
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0},                                                  // 76 - 84 -> SETE
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 2},  // 84 - 98 -> OITO
        {1, 4}, {2, 4}, {3, 4}, {3, 3}, {3, 2}, {3, 1}, {3, 0}, {2, 0}, {1, 0}, {1, 3}, {1, 2}, {2, 2}           // 98 - 111 -> NOVE
    };

    switch (numero){
        case 0:{
            for( int i = 0; i < 12; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_MAX, LED_OFF, LED_OFF);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 1:{
            for( int i = 12; i < 18; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_WEAK, LED_WEAK, LED_OFF);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 2:{
            for( int i = 18; i < 30; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_WEAK, LED_WEAK, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 3:{
            for( int i = 29; i < 41; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_WEAK, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 4:{
            for( int i = 40; i < 49; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_WEAK, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 5:{
            for( int i = 49; i < 63; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_WEAK, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 6:{
            for( int i = 63; i < 76; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_OFF, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 7:{
            for( int i = 76; i < 83; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_OFF, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 8:{
            for( int i = 83; i < 96; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_OFF, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        case 9:{
            for( int i = 92; i < 104; i++){
                LedMatrix_SetPixel(map_nunber[i][0], map_nunber[i][1], LED_OFF, LED_OFF, LED_WEAK);
                LedMatrix_Update();
                sleep_ms(100);
            }
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
        default:{
            LedMatrix_Clear();
            LedMatrix_Update();
        }
        break;
    } 
}

void play_led_sequence_game(JoystickState *js) {
    static int sequence[20][2]; // Stores the sequence of LEDs (x, y)
    static int sequence_length = 1; // Current sequence length
    static int cursor_x = 2, cursor_y = 2; // Cursor position (starts at center)
    int current_step = 0; // Current step in the sequence

    // Initialize the first LED in the sequence
    sequence[0][0] = rand() % 5; // Random x
    sequence[0][1] = rand() % 5; // Random y

    while (true) {
        // Display instructions on the OLED
        ssd_1306_fill(black);
        ssd_1306_set_cursor(0, 0);
        ssd_1306_write_string("Joystick: Move", Font_6x8, white);
        ssd_1306_set_cursor(0, 10);
        ssd_1306_write_string("A: Select LED", Font_6x8, white);
        ssd_1306_set_cursor(0, 20);
        ssd_1306_write_string("B: Clear LED", Font_6x8, white);
        ssd_1306_up_date_screen();

        // Read joystick state
        Joystick_Read(js);
        js->x_filtered = Joystick_LowPassFilter(js->x_raw, js->x_filtered);
        js->y_filtered = Joystick_LowPassFilter(js->y_raw, js->y_filtered);

        // Move cursor based on joystick input
        if (js->x_filtered > ADC_UPPER_THRESHOLD_X && cursor_x < 4) {
            cursor_x++;
        } else if (js->x_filtered < ADC_LOWER_THRESHOLD_X && cursor_x > 0) {
            cursor_x--;
        }
        if (js->y_filtered > ADC_UPPER_THRESHOLD_Y && cursor_y < 4) {
            cursor_y++;
        } else if (js->y_filtered < ADC_LOWER_THRESHOLD_Y && cursor_y > 0) {
            cursor_y--;
        }

        // Check if button A is pressed to select an LED
        if (_read_button_A()) {
            if (cursor_x == sequence[current_step][0] && cursor_y == sequence[current_step][1]) {
                // Correct LED: Change its color to green
                LedMatrix_SetPixel(cursor_x, cursor_y, LED_OFF, LED_STRONG, LED_OFF);
                LedMatrix_Update();
                sleep_ms(200); // Small delay for feedback

                current_step++;
                if (current_step == sequence_length) {
                    // Player completed the current sequence
                    sequence_length++;
                    if (sequence_length > 20) {
                        // Player wins
                        ssd_1306_fill(black);
                        ssd_1306_set_cursor(0, 20);
                        ssd_1306_write_string("You Win!", Font_6x8, white);
                        ssd_1306_up_date_screen();
                        sleep_ms(2000);
                        return;
                    }
                    // Add new LEDs to the sequence
                    for (int i = current_step; i < sequence_length; i++) {
                        sequence[i][0] = rand() % 5;
                        sequence[i][1] = rand() % 5;
                    }
                    current_step = 0;
                }
            } else {
                // Incorrect LED: Keep it red and end the game
                LedMatrix_SetPixel(cursor_x, cursor_y, LED_STRONG, LED_OFF, LED_OFF);
                LedMatrix_Update();
                sleep_ms(200); // Small delay for feedback

                // Display "Game Over" on the OLED
                ssd_1306_fill(black);
                ssd_1306_set_cursor(0, 20);
                ssd_1306_write_string("Game Over!", Font_6x8, white);
                ssd_1306_up_date_screen();
                sleep_ms(2000);

                // Clear the LED matrix
                LedMatrix_Clear();
                LedMatrix_Update();
                return;
            }
        }

        // Check if button B is pressed to clear the LED matrix
        if (_read_button_B()) {
            LedMatrix_Clear();
            LedMatrix_Update();
        }

        // Clear the LED matrix
        LedMatrix_Clear();

        // Display the sequence
        for (int i = 0; i < sequence_length; i++) {
            LedMatrix_SetPixel(sequence[i][0], sequence[i][1], LED_STRONG, LED_OFF, LED_OFF); // Red for sequence LEDs
        }

        // Highlight the cursor position
        LedMatrix_SetPixel(cursor_x, cursor_y, LED_WEAK, LED_WEAK, LED_WEAK); // Weak white for cursor

        // Update the LED matrix
        LedMatrix_Update();
        sleep_ms(100);
    }
}