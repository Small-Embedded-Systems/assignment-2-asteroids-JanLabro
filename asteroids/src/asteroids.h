/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */
extern bool paused;				 /* whether or not the game is paused */
extern bool gameOver;			//when no lives remain and game is at end
extern bool startGame;		//when game is started and should display the game
extern bool restart;
extern bool inPlay;
extern bool missile_fired;
extern int numberOfMissiles;
extern int numberOfAsteroids;

extern struct ship player;
extern struct shield player_shield;

extern struct rock *asteroids; /* array of rocks / pointer to linked-list */
extern struct missile *shots;  /* array of missiles / pointer to linked-list */


extern const float Dt; /* Time step for physics, needed for consistent motion */
