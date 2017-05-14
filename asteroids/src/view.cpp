/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,51,102); /* Midnight Blue */

#define shipp_width 9
#define shipp_height 9
static unsigned char shipp_bits[] = {
  0x10, 0x00, 0x10, 0x00, 0x38, 0x00, 0x28, 0x00, 0x6c, 0x00, 0x44, 0x00,
  0xd6, 0x00, 0xfe, 0x00, 0xc7, 0x01 };

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

void draw_sidebar(int score, float elapsed_time, int lives)
{
		//displays the lives remaining at the top left of the screen
		graphics->setCursor(2,2);
		graphics->setTextSize(1);
		graphics->printf("Lives: %d", lives);
	
		//bitmap decoration
		int l = 0;
		int x = 60;
		while(l < lives){
			graphics->setCursor(x,2);
			graphics->drawBitmap(x, 0, shipp_bits, shipp_width, shipp_height, WHITE);
			x += 12;
			l++;
		}
		
		//displays the game title at the top centre of the screen
		graphics->setCursor(199,2);
	  graphics->printf("Time Elapsed: %f", elapsed_time);
		//displays the score at the top right of the screen
		graphics->setCursor(400,2);
		graphics->printf("Score: %d", score);
    //forms a bar seperating the information from the play area
		graphics->fillRect(0, 10, 480, 1, WHITE);
	
		//draws test values
		/*graphics->setCursor(60,90);
		graphics->printf("Player Position X: %f, ", player.p.x);
		graphics->printf("Player Position Y: %f, ", player.p.y);
		graphics->printf("\nPlayer Velocity X: %f, ", player.v.x);
		graphics->printf("Player Velocity Y: %f, ", player.v.y);
		graphics->printf("\nMissile fired: %d", missileX);
		graphics->printf("Asteroid fired: %d", asteroidX);
		graphics->printf("Prune: %d", pruneX);
		graphics->printf("Collision: %d", collisionX);
		graphics->printf("\n\np0: (%f, %f) ", player.p0.x, player.p0.y);
		graphics->printf("\np1: (%f, %f) ", player.p1.x, player.p1.y);
		graphics->printf("\np2: (%f, %f) ", player.p2.x, player.p2.y);
		graphics->printf("\nshield: (%f, %f) ", player_shield.s.x, player_shield.s.y);*/
	
}

void drawmissiles(struct missile *list)
{
	while(list)
	{
		if(list){
		graphics->fillCircle(list->p.x, list->p.y, 1, RED);
		list = list->next;
	}
}
}

void drawrocks(struct asteroids *list)
{
	while(list)
	{
		graphics->fillCircle(list->r.p.x, list->r.p.y, list->r.size, WHITE);
		list = list->next;
	}
}

void drawshield(shield player_shield)
{
	//If statements set the colour of the shield based on its quality
	if(player_shield.quality == 3)
	{
		//A quality of 3 produces a green shield
		player_shield.colour = GREEN;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 2)
	{
		//A quality of 2 produces a yellow shield
		player_shield.colour = YELLOW;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 1)
	{
		//A quality of 1 produces a red shield
		player_shield.colour = RED;
		graphics->drawCircle(player_shield.s.x, player_shield.s.y, player_shield.size, player_shield.colour);
	}
	else if(player_shield.quality == 0)
	{
		//A quality of zero produces no shield
	}
}


void drawship(ship player)
{
	graphics->drawTriangle(player.p0.x, player.p0.y, player.p1.x, player.p1.y, player.p2.x, player.p2.y, WHITE);
	graphics->fillTriangle(player.p0.x, player.p0.y, player.p1.x, player.p1.y, player.p2.x, player.p2.y, WHITE);
	
}


void draw(void)
{
    graphics->fillScreen(background);

		if (startGame == 1)
		{
			graphics->setTextSize(3);
			graphics->setCursor(165, 120);
			graphics->printf("Asteroids");
			
			graphics->setTextSize(2);
			graphics->setCursor(40, 175);
			graphics->printf("Press Centre button to start game!");
  
		}
		else if (inPlay == 1)
		{
			if(paused)
			{
				graphics->setTextSize(2);
				graphics->setCursor(220, 120);
				graphics->printf("Paused");
				
				graphics->setTextSize(1);
				graphics->setCursor(180, 150);
				graphics->printf("Press Userbutton to Resume");
			}
			else
			{
				draw_sidebar(score, elapsed_time, lives);
				drawmissiles(active);
				drawrocks(active_asteroid);
				drawshield(player_shield);
				drawship(player);
			}
		}
		else if(gameOver == 1)
		{
			graphics->setTextSize(2);
			graphics->setCursor(195, 120);
			graphics->printf("Game Over");
			
			graphics->setCursor(180, 150);
			graphics->printf("Score: %d", score);
		}  
		swap_DBuffer();
}
