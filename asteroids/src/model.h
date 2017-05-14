/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to use them */
struct ship {
    coordinate_t p;	
		coordinate_t p0;
		coordinate_t p1;
		coordinate_t p2;
    vector_t     v;
		vector_t		 a;
		int collision;
		float heading;
		float headingRads;
};

struct shield {
		coordinate_t s;
		int size;
		int colour;
		bool exists;
		int quality;
};


/* initial struts for building linked lists */
struct rock {
    coordinate_t p;
		vector_t     v;
		int size;
		int collision;
    struct rock *next;
};

typedef struct rock rock_t;

typedef struct asteroids {
		rock r;		
		struct asteroids *next;
} asteroids_t;


typedef struct missile {
    coordinate_t p;
		vector_t v;
		int outOfBounds;
		int collision;
    struct missile *next;
} missile_t;


void physics(void);
ship move_ship(ship, float);
shield move_shield(ship, shield, float);
ship wrap_around(ship);
missile_t *initialise_missile_list();
asteroids_t *initialise_asteroid_list();
ship initialise_player();


extern missile_t *active;
extern asteroids_t *active_asteroid;

