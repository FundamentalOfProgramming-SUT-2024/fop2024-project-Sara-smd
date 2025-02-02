#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>

#ifndef BASICS
#define BASICS
typedef struct
{
    int x;
    int y;
    char contain;
    int visible;
} point;

typedef struct
{
    point corner;
    int height;
    int width;
    point floor[15][15];
} Room;

typedef struct
{
    int st_room;
    int en_room;
    int direction;
    int vertical;
    int horizontal;
    int length;
    int turn;
    int sub;
    point start;
    point end;
    point trail[60];
} Corridor;

typedef struct
{
    Room room[6];
    Corridor corridor[7];
    int is[7];
    int trans;
    point staircase;
} Map;

typedef struct
{
    int x;
    int y;
    int value;
    //int color;
    int used;
    int place;
} Gold;

typedef struct
{
    int x;
    int y;
    int place;
} Food;

typedef struct
{
    int color;
    int difficulty;
} Sett;

typedef struct
{
    Map map;
    int cnt_gold;
    Gold gold[10];
    Food food[5];
} Level;

typedef struct
{
    int x;
    int y;
    int ind;
    int dir;
} Charachter;

typedef struct
{
    Level level[4];
    Charachter hero;
    int on_door;
    int cur_room;
    int cur_corr;
    int cur_level;
    Sett settings;
    int score;
    int health;
    int hunger;
    int food_save;
    time_t last_update;
    time_t last_check;
    time_t last_message;
    char *message;
} Game;
#endif // BASICS

#ifndef GENERATOR
#define GENERATOR
int absolute(int);
point random_point(Room, int);
void generalize_point(Room, point *);
void localize_point(Room, point *);
int check_available(Level, int, point);

#endif // GENERATOR

#ifndef MAP
#define MAP
void create_room(Room *, int, point);
void create_corridor(Room *, Corridor *, int *);
void build_room(Room *, int);
void build_corridor(Room *, Corridor *, int *, int);
void build_pillar(Room *, int);

int contain_stair(Room, point);
int check_conflict(Corridor *, int, int);
int conflict(Corridor *, int *, int );

#endif // MAP

#ifndef MESSAGE
#define MESSAGE

#define DELAY 100000
#define DEFAULT_MESSAGE "                          "
#define FOOD_CAPACITY "Not enough capacity!"
#define NEW_LEVEL "You entered a new level!"
#define NEW_ROOM "You entered a new room!"
#define ADD_GOLD "Added to your score!"
#define ADD_FOOD "Added to the food menu!"

#endif // MESSAGE

#ifndef DRAW
#define DRAW
void draw(Game *, int);
void draw_map(Map, int);
void draw_gold(Room *, int, Gold *, int);
void draw_food(Room *, Food *, int);
void draw_health(Game *);
void draw_score(Game);
void draw_level(Game);

void show_food_menu(Game *);

void draw_message();

#endif // DRAW

#ifndef MOVEMENT
#define MOVEMENT
void handle_movement(Game *, int);
point destination(int, int, int);
void move_hero(Game *, point);
void draw_hero(Game);
void reveal_room(Game*);
void reveal_corr(Game*, int);
int inside_room(Game, point);
int inside_corr(Game *, point);
int inside_door_r(Game, point);
int inside_door_c(Game *, point);
void new_room(Game *, point);
void new_corr(Game *, point);
int find_room(Game, point);
int find_corr(Game *, point);
void change_level(Game *);
int on_stair(Game *);

#endif // MOVEMENT

#ifndef CREATE_LEVEL
#define CREATE_LEVEL
void set_stair(Level *, int, int);
void change_stair(Level *);
point find_inside(int);
void create_levels(Level *, int);

#endif // LEVEL

#ifndef HANDLE_ITEMS
#define HANDLE_ITEMS
void set_gold(Level *);
void set_food(Level *, int);
void capture_item(Game *);

#endif // HANDLE_ITEMS

#ifndef SETTINGS
#define SETTINGS
Sett settings_menu();

#endif // SETTINGS

#ifndef NEWGAME
#define NEWGAME
void new_game(Sett);
void PreGame();

#endif // NEWGAME
