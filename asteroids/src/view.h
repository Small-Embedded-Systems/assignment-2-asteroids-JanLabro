/* Asteroids View */

/* support double buffering */
void init_DBuffer(void);
void swap_DBuffer(void);

void draw(void);
void draw_sidebar(int, float, int);
void drawshield(shield player_shield);
void drawship(ship player);
