#include "mybasics.h"

void draw(Game *game, int sh)
{
    draw_map(game->level[game->cur_level].map, sh);
    draw_gold(game->level[game->cur_level].map.room, game->level[game->cur_level].cnt_gold, game->level[game->cur_level].gold, sh);
    draw_food(game->level[game->cur_level].map.room, game->level[game->cur_level].food, sh);
    draw_health(game);
    draw_score(*game);
    draw_hero(*game);
}

void draw_map(Map map, int sh)
{
    Room *room = map.room;
    Corridor *corridor = map.corridor;
    int *is = map.is;
    for(int r = 0; r < 6; r++)
    {
        for(int i = 0; i < room[r].height; i++)
        {
            for(int j = 0; j < room[r].width; j++)
            {
                move(room[r].floor[i][j].x, room[r].floor[i][j].y);
                if(room[r].floor[i][j].visible || sh)
                    printw("%c", room[r].floor[i][j].contain);
            }
        }
    }

    for(int i = 0; i < 7; i++)
        if(is[i])
        {
            for(int j = 0; j < corridor[i].length; j++)
            {
                move(corridor[i].trail[j].x, corridor[i].trail[j].y);
                if(corridor[i].trail[j].visible || sh)
                    printw("%c", corridor[i].trail[j].contain);
            }
        }
    return;
}

void draw_gold(Room *room, int n, Gold *gold, int sh)
{
    for(int i = 0; i < n; i++)
    {
        int r = gold[i].x - room[gold[i].place].corner.x;
        int c = gold[i].y - room[gold[i].place].corner.y;
        if(gold[i].used == 0 && (room[gold[i].place].floor[r][c].visible || sh))
        {
            move(gold[i].x, gold[i].y);
            printw("*");
        }
    }
}

void draw_food(Room *room, Food *food, int sh)
{
    for(int i = 0; i < 5; i++)
    {
        int r = food[i].x - room[food[i].place].corner.x;
        int c = food[i].y - room[food[i].place].corner.y;
        if(room[food[i].place].floor[r][c].visible || sh)
        {
            move(food[i].x, food[i].y);
            printw("f");
        }
    }
}

void draw_score(Game game)
{
    mvprintw(1, 75, "score: %d", game.score);
    mvprintw(3, 75, "level: %d", game.cur_level);
}

void draw_health(Game *game)
{
    mvprintw(1, 95, "Health : ");
    for(int i = 0; i < game->health; i++)
        printw("#");
    mvprintw(3, 95, "Hunger : ");
    for(int i = 0; i < game->hunger; i++)
        printw("#");

    int update = 5;
    int check = 3;
    time_t now = time(NULL);
    if(now - game->last_update > update)
    {
        if(game->hunger < 12)
            game->hunger++;
        game->last_update = now;
    }

    if(now - game->last_check > check)
    {
        if(game->hunger < 3)
            game->health++;
        else if(game->hunger < 6)
            game->health += 0;
        else if(game->hunger < 9)
            game->health--;
        else
            game->health -= 2;

        if(game->health > 12)
            game->health = 12;
        if(game->health < 0)
            game->health = 0;
        game->last_check = now;
    }
}
