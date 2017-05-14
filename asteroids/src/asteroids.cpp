/* Asteroids
    Semester 2 -- Small Embedded Systems - CM0506
    Dr Alun Moon
		by
		Jacob Joyce w15039859
		Jan Labro	w15010510
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
bool startGame = true;
bool gameOver = false;
bool restart = false;
bool inPlay = false;
struct ship player;
struct shield player_shield;

float Dt = 0.01f;

Ticker model, view, controller;

bool paused = false;
/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);
int main()
{

    init_DBuffer();
	
		view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
	
}
