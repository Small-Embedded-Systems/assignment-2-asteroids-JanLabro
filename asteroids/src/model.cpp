/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"


/* If you want to initialise a linked list:
    You'll have to replace node_t with the right type */


const size_t MAXSize = 10;

static missile_t missile_data[MAXSize];
static missile_t *freenodes;

static asteroids_t asteroid_data[MAXSize];
static asteroids_t *freenodes_asteroids;

//initialise the missile list
missile_t *initialise_missile_list()
{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++){
        missile_data[c].next = &missile_data[c+1];
    }
    missile_data[c].next = NULL;
		freenodes = missile_data;
    return missile_data;
}

//initialise the asteroid list
asteroids_t *initialise_asteroid_list()
{
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++){
        asteroid_data[c].next = &asteroid_data[c+1];
    }
    asteroid_data[c].next = NULL;
		freenodes_asteroids = asteroid_data;
    return asteroid_data;
}

ship initialise_player()
{
		player.p.x = 240;
		player.p.y = 150;
		player.collision = 0;
	
		player.p1.x = player.p.x;
		player.p1.y = player.p.y - 8;
	
		player.p0.x = player.p1.x - 5;
		player.p0.y = player.p1.y + 16;
		
		player.p2.x = player.p1.x + 5;
		player.p2.y = player.p1.y + 16;
	
		player_shield.s.x = player.p.x;
		player_shield.s.y = player.p.y + 7;
		player_shield.size = 20;
		player_shield.quality = 3;
	
	return player;
}

//gets the next free node in the missile list
missile_t *getNode()
{
	missile_t *available = NULL;
	if (freenodes)
	{
		available = freenodes;
		freenodes = freenodes->next;
		available->next = NULL;
	}
	return available;
}

//gets the next free node in the asteroid list
asteroids_t *getNodeAsteroid()
{
	asteroids_t *available_asteroid = NULL;
	if (freenodes_asteroids)
	{
		available_asteroid = freenodes_asteroids;
		freenodes_asteroids = freenodes_asteroids->next;
		available_asteroid->next = NULL;
	}
	return available_asteroid;
}

//frees a node in the missile list
void freenode(struct missile *n)
{
    n->next = freenodes;
    freenodes = n;
}

//frees a node in the asteroids list
void freenodeAsteroid(struct asteroids *n)
{
    n->next = freenodes_asteroids;
    freenodes_asteroids = n;
}

//prunes a node in the missile list
struct missile *prunehead(struct missile *list)
{
	
	if (list)
	{
		//If out of bounds has been set to zero
		if(list->outOfBounds > 0 || list->collision > 0)
		{
			struct missile *discard = list;
			list = list->next;
			discard->next = NULL;
			freenode(discard);
		}		
	}
	return list;
}

struct missile *prune(struct missile *list)
{
  struct missile *m = prunehead(list);

	return m;
}

//prunes a node in the missile list
struct asteroids *prunehead_asteroid(struct asteroids *list)
{
	
	if (list)
	{	
		if(list->r.collision != 0)
		{
			struct asteroids *discard = list;
			list = list->next;
			discard->next = NULL;
			freenodeAsteroid(discard);	
		}
	}
	return list;
}

struct asteroids *prune_asteroid(struct asteroids *list)
{
  struct asteroids *a = prunehead_asteroid(list);

	return a;
}


ship move_ship(ship player, float Dt)
{
	//Finds the centre (x, y) coordinates of the ship
	player.p.x = (player.p0.x + player.p1.x + player.p2.x)/3;
	player.p.y = (player.p0.y + player.p1.y + player.p2.y)/3;
	
	//converts the heading from controller to radian values
	player.headingRads = radians(player.heading);
	
	//coverts the radian headings to sine, cosine values
	float s = sin(player.headingRads);
	float c = cos(player.headingRads);

	//this rotates the first point of the triangle around the centre of the triangle
	float newp0x = player.p.x + (player.p0.x - player.p.x) * c - (player.p0.y - player.p.y) * s; 
	float newp0y = player.p.y + (player.p0.x - player.p.x) * s + (player.p0.y - player.p.y) * c;
	
  player.p0.x = newp0x;
	player.p0.y = newp0y;
	
	//this rotates the second point of the triangle around the centre of the triangle
	float newp1x = player.p.x + (player.p1.x - player.p.x) * c - (player.p1.y - player.p.y) * s;
	float newp1y = player.p.y + (player.p1.x - player.p.x) * s + (player.p1.y - player.p.y) * c;

	player.p1.x = newp1x;
	player.p1.y = newp1y;
	
		//this rotates the third point of the triangle around the centre of the triangle
	float newp2x = player.p.x + (player.p2.x - player.p.x) * c - (player.p2.y - player.p.y) * s; 
	float newp2y = player.p.y + (player.p2.x - player.p.x) * s + (player.p2.y - player.p.y) * c;
	
	player.p2.x = newp2x;
	player.p2.y = newp2y;
	
	//attempt to allow the ship to move
	float newaccx = player.a.x * c * Dt;
	float newaccy = player.a.y * s * Dt;
	
	player.v.x += newaccx;
	player.v.y += newaccy;
	
	//updates the player position based on it's velocity
	player.p0.x = player.p0.x + player.v.x;// * Dt;
	player.p0.y = player.p0.y + player.v.y;// * Dt;
	
	player.p1.x = player.p1.x + player.v.x;// * Dt;
	player.p1.y = player.p1.y + player.v.y;// * Dt;
	
	player.p2.x = player.p2.x + player.v.x;// * Dt;
	player.p2.y = player.p2.y + player.v.y;// * Dt;
		
	
	//wraps the ship around to the other edge of the screen if it would go out of the play area
	player = wrap_around(player);
	
	return player;
}

ship wrap_around(ship player)
{
	//If the ship has passed the right boundary, move it to the left
	if((player.p0.x > 480) || (player.p1.x > 480) || (player.p2.x > 480))
	{
		//player.p.x = 0;
		player.p0.x -= 469;
		player.p1.x -= 469;
		player.p2.x -= 469;
	}
	//Else if the ship has passed the left boundary, move it to the right
	else if((player.p0.x < 0) || (player.p1.x < 0) || (player.p2.x < 0))
	{
		//player.p.x = 480;
		player.p0.x += 469;
		player.p1.x += 469;
		player.p2.x += 469;
	}
	
	
	//If the ship has passed the bottom boundary, move it to the top
	if((player.p0.y > 275) || (player.p1.y > 275) || (player.p2.y > 275))
	{
		//player.p.y = 15;
		player.p0.y -= 249;
		player.p1.y -= 249;
		player.p2.y -= 249;
	}
	//Else if the ship has passed the top boundary, move it to the bottom
	else if((player.p0.y < 10) || (player.p1.y < 10) || (player.p2.y < 10))
	{
		//player.p.y = 250;
		player.p0.y += 249;
		player.p1.y += 249;
		player.p2.y += 249;
	}
	
	return player;
}

shield move_shield(ship player, shield player_shield, float Dt)
{
	
		player_shield.s.x = player.p.x;
		player_shield.s.y = player.p.y;
	
	return player_shield;
}

void create_missile(struct missile *pew)
{
	//missile originates at the same point as the ship with the same velocity
	pew->p.x = player.p1.x;
	pew->p.y = player.p1.y;
	pew->v.x = 5* -(player.p.x - player.p1.x);
	pew->v.y = 5* -(player.p.y - player.p1.y);
	pew->outOfBounds = 0;
	pew->collision = 0;
	
	//missileX = 22;
}

asteroids_t create_asteroid(struct asteroids *rock)
{
	//creates a new asteroid at random position, with a random size and a random velocity
	rock->r.p.x = randrange(30,450);
	rock->r.p.y = randrange(30,250);
	rock->r.v.x = randrange(-25, 25);
	rock->r.v.y = randrange(-25, 25);
	rock->r.size = randrange(2, 20);
	rock->r.collision = 0;
	
	return *rock;
}

struct missile * update_missiles(struct missile *missile)
{
		
	struct missile *m = missile;
	
	for( ; missile; missile = missile->next)
	{
		//move the missile
		missile->p.x += missile->v.x * Dt;
		missile->p.y += missile->v.y * Dt;
		
		//check to see if the missile has hit an asteroid
		
		
		//check to see if the missle is out of bounds
		if(missile->p.x < 0 || missile->p.x > 480 || missile->p.y < 10 || missile->p.y > 275)
		{
			missile->outOfBounds = 1;
	
		}
	}
	
		//removes any missiles which are out of bounds
		missile = prune(m);
	return missile;
}

asteroids_t wrap_around_asteroid(struct asteroids *rock)
{
	
	//If the ship has passed the right boundary, move it to the left
	if(rock->r.p.x > 480)
	{
		rock->r.p.x = 0;
	}
	//Else if the ship has passed the left boundary, move it to the right
	else if(rock->r.p.x < 0)
	{
		rock->r.p.x = 480;
	}
	
	
	//If the ship has passed the bottom boundary, move it to the top
	if(rock->r.p.y > 275)
	{
		rock->r.p.y = 10;
	}
	//Else if the ship has passed the top boundary, move it to the bottom
	else if(rock->r.p.y < 10)
	{
		rock->r.p.y = 275;
	}
	
	return *rock;
}

struct asteroids * update_asteroids(struct asteroids *rock)
{
	
	struct asteroids *a = rock;
	
	for( ; rock; rock = rock->next)
	{
		//move the rock
		rock->r.p.x += rock->r.v.x * Dt;
		rock->r.p.y += rock->r.v.y * Dt;	
		
		//check to see if the asteroid is out of bounds
		//if so, wrap it around to the other edge of the screen
		*rock = wrap_around_asteroid(rock);
		
	}
	
	rock = prune_asteroid(a);
	return rock;
	
}


struct missile *active = NULL;
struct asteroids *active_asteroid = NULL;

void move_missiles()
{
	//if a new missile has been fired
	if(missile_fired)
	{
		missile_fired = false;
		//get the next free space in the missile list
		struct missile *pew = getNode();
		if(pew)
		{
			//create a missile in the free space
			pew->next = active;
			active = pew;
			create_missile(pew);
		}
	}
	
	//moves existing missiles
	active = update_missiles(active);
	
}

void move_asteroids()
{
	//get the next free space in the asteroid list
	struct asteroids *rock = getNodeAsteroid();
	if(rock)
	{
		//create a new asteroid
		rock->next = active_asteroid;
		active_asteroid = rock;
		create_asteroid(rock);
	}
	
	//move existing asteroids
	active_asteroid = update_asteroids(active_asteroid);
	
}

//Checks to see if there has been a collision between any of the missiles or asteroids
void missile_collision(struct missile *missile_list, struct asteroids *asteroid_list)
{
	//loops through all the missiles
	for( ; missile_list; missile_list = missile_list->next)
	{
		//for each asteroid in play
		for( ; asteroid_list ; asteroid_list = asteroid_list->next)
		{		
			//create local variables for missile and asteroid values
			float dx = asteroid_list->r.p.x - missile_list->p.x;
			float dy = asteroid_list->r.p.y - missile_list->p.y;			
			//calculate the distance between the missile and the asteroid
			float distance = sqrt(dx * dx + dy * dy);
			//check for a collision
			if(distance < asteroid_list->r.size + 1)
			{
				//remove the missile and asteroid from play
				missile_list->collision = 1;
				asteroid_list->r.collision = 1;
			}
		}
	}
}

bool ship_collision(struct asteroids *list)
{
	//for each asteroid in play
	for( ; list ; list = list->next)
	{
		//create local variables for the asteroid and ship values
		float dx = list->r.p.x - player.p.x;
		float dy = list->r.p.y - player.p.y;
		//calculate the distance between the asteroid and the ship
		float distance = sqrt( dx * dx + dy * dy);
		//check for a collision
		if (distance < list->r.size + 1)
		{
			return true;
		}		
	}
	return false;
}

void physics(void)
{
	if(startGame){
		lives = 5;
		elapsed_time = 0;
	}
	
	if(restart == 1) {
		initialise_missile_list();
		initialise_asteroid_list();		
		player = initialise_player();
		inPlay = true;
		restart = 0;
	}
	
	if(inPlay){
		if(!paused)
		{
			player = move_ship(player, Dt);
			player_shield = move_shield(player, player_shield, Dt);
			move_missiles();
			move_asteroids();
			
			elapsed_time = elapsed_time + 1;
			score = elapsed_time;
				
			missile_collision(active, active_asteroid);
			if (ship_collision(active_asteroid) )
			{
				if (player_shield.quality > 0)
				{
					player_shield.quality = player_shield.quality - 1;
				}
				else
				{
					if(lives > 0)
					{
						lives = lives - 1;
						player = initialise_player();
					}
					else
					{
						inPlay = 0;
						gameOver = 1;
					}
				}
			}
		}
		else
			paused = true;
		}	
}

