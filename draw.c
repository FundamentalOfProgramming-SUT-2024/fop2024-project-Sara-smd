#include "mybasics.h"

void make_colors();

void draw(Game *game, int sh)
{
    make_colors();
    draw_map(game->level[game->cur_level].map, sh);
    draw_gold(game->level[game->cur_level].map.room, game->level[game->cur_level].cnt_gold, game->level[game->cur_level].gold, sh);
    draw_food(game->level[game->cur_level].map.room, game->level[game->cur_level].food, sh);
    draw_health(game);
    draw_score(*game);
    draw_level(*game);
    draw_hero(*game);
    draw_message(game);
}

void make_colors()
{
    init_color(3, 0, 0, 900);
    init_color(4, 700, 700, 0);
    init_color(5, 900, 600, 0);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, 3, COLOR_BLACK);
    init_pair(4, 4, COLOR_BLACK);
    init_pair(5, 5, COLOR_BLACK);
}

void draw_map(Map map, int sh)
{
    for(int r = 0; r < 6; r++)
    {
        for(int i = 0; i < map.room[r].height; i++)
        {
            for(int j = 0; j < map.room[r].width; j++)
            {
                move(map.room[r].floor[i][j].x, map.room[r].floor[i][j].y);
                if(map.room[r].floor[i][j].visible || sh)
                    printw("%c", map.room[r].floor[i][j].contain);
            }
        }
    }

    for(int i = 0; i < 7; i++)
        if(map.is[i])
        {
            for(int j = 0; j < map.corridor[i].length; j++)
            {
                move(map.corridor[i].trail[j].x, map.corridor[i].trail[j].y);
                if(map.corridor[i].trail[j].visible || sh)
                    printw("%c", map.corridor[i].trail[j].contain);
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
            attron(COLOR_PAIR(gold[i].color));
            move(gold[i].x, gold[i].y);
            printw("*");
            attroff(COLOR_PAIR(gold[i].color));
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
    mvprintw(1, 75, "score : %d", game.score);
}

void draw_level(Game game)
{
    mvprintw(3, 75, "level : %d", game.cur_level + 1);
}

void draw_health(Game *game)
{
    mvprintw(1, 95, "Health : ");
    for(int i = 0; i < game->health; i++)
        printw("#");
    mvprintw(3, 95, "Hunger : ");
    for(int i = 0; i < game->hunger; i++)
        printw("#");

    int update = 10 - 3 * game->settings.difficulty;
    int check = 5 - game->settings.difficulty;
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
        if(game->health <= 0)
            game->health = 0;
        game->last_check = now;
    }
}

void draw_message(Game *game)
{
    time_t now = time(NULL);
    if(now - game->last_message > 1)
    {
        game->message = DEFAULT_MESSAGE;
        game->last_message = now;
    }
    mvprintw(1, 4, "%s", game->message);
}
