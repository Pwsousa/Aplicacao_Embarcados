#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

/************************| includs |**************************************/
#include "config.h"


/************************| defines |**************************************/
#define SNAKE_SPEED_EASY   1000   // Velocidade em ms (fácil)
#define SNAKE_SPEED_MEDIUM 750    // Velocidade em ms (médio, 25% mais rápido)
#define SNAKE_SPEED_HARD   250    // Velocidade em ms (difícil, 70% mais rápido)

// Função principal do jogo da cobrinha
/************************| function |**************************************/
void play_snake_game(JoystickState *js, int difficulty);

#endif