#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#include"mylevel.h"

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
    //int cur_color;
} Game;

void handle_input(Game *, int);
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

int main()
{
    initscr();
    noecho();
    curs_set(false);
    srand(time(NULL));

    Game game;
    game.cur_level = 0;
    game.cur_corr = -1;
    game.on_door = 0;
    game.cur_room = rand() % 6;

    create_levels(game.level, game.cur_room);
    point tmp = random_point(game.level[game.cur_level].map.room[game.cur_room], 1);
    game.hero.x = tmp.x;
    game.hero.y = tmp.y;
    reveal_room(&game);

    while(true)
    {
        draw(game.level[game.cur_level].map);
        draw_hero(game);
        int ch = getch();
        if(ch == 10)
            break;
        handle_input(&game, ch);
        clear();
    }

    endwin();
    return 0;
}

void handle_input(Game *game, int ch)
{
    if(ch == 32)
        change_level(game);
    else
        handle_movement(game, ch);
}

void change_level(Game *game)
{
    int st = on_stair(game);
    game->cur_level += st;
    reveal_room(game);
}

int on_stair(Game *game)
{
    Room rm = game->level[game->cur_level].map.room[game->cur_room];
    int r = game->hero.x - rm.corner.x;
    int c = game->hero.y - rm.corner.y;
    if(rm.floor[r][c].contain == '<')
        return 1;
    if(rm.floor[r][c].contain == '>')
        return -1;
    return 0;
}

void handle_movement(Game *game, int ch)
{
    Game g = *game;
    point dst = destination(g.hero.x, g.hero.y, ch);
    if(g.on_door)
    {
        if(inside_room(g, dst))
        {
            game->on_door = 0;
            game->cur_corr = -1;
            move_hero(game, dst);
        }
        else if(inside_corr(game, dst))
        {
            game->on_door = 0;
            game->cur_room = -1;
            int i = game->hero.ind;
            int j = game->hero.dir;
            reveal_corr(game, i + j + j);
            move_hero(game, dst);
        }
    }
    else if(g.cur_room != -1)
    {
        if(inside_room(g, dst))
            move_hero(game, dst);
        else if(inside_door_r(g, dst))
        {
            game->on_door = 1;
            new_corr(game, dst);
            move_hero(game, dst);
        }
    }
    else
    {
        if(inside_corr(game, dst))
        {
            int i = game->hero.ind;
            int j = game->hero.dir;
            reveal_corr(game, i + j + j);
            move_hero(game, dst);
        }
        else if(inside_door_c(game, dst))
        {
            game->on_door = 1;
            new_room(game, dst);
            move_hero(game, dst);
        }
    }
}

void new_room(Game *game, point pnt)
{
    game->cur_room = find_room(*game, pnt);
    reveal_room(game);
}

void new_corr(Game *game, point pnt)
{
    game->cur_corr = find_corr(game, pnt);
    int i = game->hero.ind;
    int j = game->hero.dir;
    reveal_corr(game, i + j);
    reveal_corr(game, i + j + j);
}

int find_room(Game game, point pnt)
{
    Room *rm = game.level[game.cur_level].map.room;
    for(int i = 0; i < 6; i++)
        if(rm[i].corner.x <= pnt.x && pnt.x < rm[i].corner.x + rm[i].height)
            if(rm[i].corner.y <= pnt.y && pnt.y < rm[i].corner.y + rm[i].width)
                return i;
}

int find_corr(Game *game, point pnt)
{
    Corridor *corr = game->level[game->cur_level].map.corridor;
    int *b = game->level[game->cur_level].map.is;
    for(int i = 0; i < 7; i++)
        if(b[i])
        {
            if(corr[i].start.x == pnt.x && corr[i].start.y == pnt.y)
            {
                game->hero.ind = -1;
                game->hero.dir = 1;
                return i;
            }
            if(corr[i].end.x == pnt.x && corr[i].end.y == pnt.y)
            {
                game->hero.ind = corr[i].length;
                game->hero.dir = -1;
                return i;
            }
        }
    return -1;
}

int inside_door_r(Game game, point pnt)
{
    Room room = game.level[game.cur_level].map.room[game.cur_room];
    int r = pnt.x - room.corner.x;
    int c = pnt.y - room.corner.y;
    if(room.floor[r][c].contain == '+')
        return 1;
    return 0;
}

int inside_door_c(Game *game, point pnt)
{
    Corridor corr = game->level[game->cur_level].map.corridor[game->cur_corr];
    if(corr.start.x == pnt.x && corr.start.y == pnt.y)
    {
        game->hero.ind = -1;
        game->hero.dir = 1;
        return 1;
    }
    if(corr.end.x == pnt.x && corr.end.y == pnt.y)
    {
        game->hero.ind = corr.length;
        game->hero.dir = -1;
        return 1;
    }
    return 0;
}

int inside_room(Game game, point pnt)
{
    int r = pnt.x - game.level[game.cur_level].map.room[game.cur_room].corner.x;
    int c = pnt.y - game.level[game.cur_level].map.room[game.cur_room].corner.y;
    if(game.level[game.cur_level].map.room[game.cur_room].floor[r][c].contain == '.')
        return 1;
    if(game.level[game.cur_level].map.room[game.cur_room].floor[r][c].contain == '<')
        return 1;
    if(game.level[game.cur_level].map.room[game.cur_room].floor[r][c].contain == '>')
        return 1;
    return 0;
}

int inside_corr(Game *game, point pnt)
{
    int i = game->hero.ind;
    int j = game->hero.dir;
    point tmp;
    Corridor corr = game->level[game->cur_level].map.corridor[game->cur_corr];
    if(i + j >= 0 && i + j < corr.length)
    {
        tmp = corr.trail[i + j];
        if(tmp.x == pnt.x && tmp.y == pnt.y)
        {
            game->hero.ind = i + j;
            return 1;
        }
    }
    if(i - j >= 0 && i - j < corr.length)
    {
        tmp = corr.trail[i - j];
        if(tmp.x == pnt.x && tmp.y == pnt.y)
        {
            game->hero.ind = i - j;
            return 1;
        }
    }
    return 0;
}

void reveal_room(Game* game)
{
    Room *rm = game->level[game->cur_level].map.room;
    for(int i = 0; i < rm[game->cur_room].height; i++)
        for(int j = 0; j < rm[game->cur_room].width; j++)
            rm[game->cur_room].floor[i][j].visible = 1;
}

void reveal_corr(Game *game, int i)
{
    Corridor *corr = game->level[game->cur_level].map.corridor;
    if(i >= 0 && i < corr[game->cur_corr].length)
        corr[game->cur_corr].trail[i].visible = 1;
}

void move_hero(Game *game, point pnt)
{
    game->hero.x = pnt.x;
    game->hero.y = pnt.y;
}
void draw_hero(Game game)
{
    Charachter ch = game.hero;
    move(ch.x, ch.y);
    printw("@");
}

point destination(int r, int c, int ch)
{
    if(ch == 'j')
        c++;
    if(ch == 'h')
        c--;
    if(ch == 'n')
        r++;
    if(ch == 'u')
        r--;
    if(ch == 'y')
    {
        c--;
        r--;
    }
    if(ch == 'i')
    {
        c++;
        r--;
    }
    if(ch == 'b')
    {
        c--;
        r++;
    }
    if(ch == 'm')
    {
        c++;
        r++;
    }
    point res;
    res.x = r;
    res.y = c;
    return res;
}
