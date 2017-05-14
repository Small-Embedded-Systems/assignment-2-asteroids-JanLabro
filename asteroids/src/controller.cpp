/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch
*/
typedef enum {JLT = 0, JRT, JUP, JDN, JCR, PllUp} btnId_t;
enum position { left,down,right,up,centre };
static DigitalIn joystick[] = {P5_4, P5_0, P5_2, P5_1, P5_3, P2_10};
static bool jsPrsdAndRlsd(btnId_t b);
static bool jsPrsd(btnId_t b);

/* Definition of Joystick press capture function
 * b is one of JLEFT, JRIGHT, JUP, JDOWN - from enum, 0, 1, 2, 3
 * Returns true if this Joystick pressed then released, false otherwise.
 *
 * If the value of the button's pin is 0 then the button is being pressed,
 * just remember this in savedState.
 * If the value of the button's pin is 1 then the button is released, so
 * if the savedState of the button is 0, then the result is true, otherwise
 * the result is false. */
static bool jsPrsdAndRlsd(btnId_t b) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[6] = {1,1,1,1,1,1};
        //initially all 1s: nothing pressed
	state = joystick[b].read();
  if ((savedState[b] == 0)  && (state == 1)) {
		result = true;
	}
	savedState[b] = state;
	return result;
}

static bool jsPrsd(btnId_t b) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[6] = {1,1,1,1,1,1};
        //initially all 1s: nothing pressed
	state = joystick[b].read();
  if (savedState[b] == 0)  { 
		result = true;
	}
	savedState[b] = state;
	return result;
}

bool missile_fired = false;

void controls(void)
{

	if(startGame){
		if (jsPrsdAndRlsd(JCR)){
			startGame = false;
			restart = true;
		}
	}
		
	if(inPlay){
		if(!paused){
			if(jsPrsdAndRlsd(PllUp)){
				paused = true;
				wait_ms(200);
			}
		  else if (jsPrsd(JUP))
			{
				//up was pressed
				player.a.x = 0.5;
				player.a.y = 0.5;
			}
			else if (jsPrsd(JRT))
			{
				//right was pressed
				player.heading = 1;
			}
			else if (jsPrsd(JLT))
			{
				//left was pressed
				player.heading = -1;
			}
			else if (jsPrsd(JDN))
			{
				//down was pressed
				player.a.x = -0.25;
				player.a.y = -0.25;
			}
			else if (jsPrsd(JCR))
			{
				//Pressing centre fires a missile
				missile_fired = true;
			} else {
				player.heading = 0;
				player.v.x = 0;
				player.v.y = 0;
			}
		}
		
		if(paused){
			if(jsPrsdAndRlsd(PllUp)){
				paused = false;
				wait_ms(200);
			}
		}
	}
	
	if(gameOver){
		if (jsPrsdAndRlsd(JCR)){
			startGame = true;
			gameOver =false;
		}
	}
}
