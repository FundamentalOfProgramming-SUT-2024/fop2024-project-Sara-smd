#include"mybasics.h"

void set_gold(Level *level)
{
    int cnt = rand() % 10;
    level->cnt_gold = cnt;
    for(int i = 0; i < cnt; i++)
    {
        int rm = rand() % 6;
        point pnt;
        do
        {
            pnt = random_point(level->map.room[rm], 1);
            generalize_point(level->map.room[rm], &pnt);
        }
        while(!check_available(*level, rm, pnt));

        level->gold[i].place = rm;
        level->gold[i].x = pnt.x;
        level->gold[i].y = pnt.y;
        level->gold[i].used = 0;
        int rn = rand() % 10;
        if(rn == 0)
            level->gold[i].value = 3;
        else
            level->gold[i].value = 1;
    }
}

void set_food(Level *level, int i)
{
    int rm = rand() % 6;
    point pnt;
    do
    {
        pnt = random_point(level->map.room[rm], 1);
        generalize_point(level->map.room[rm], &pnt);
    }
    while(!check_available(*level, rm, pnt));

    level->food[i].place = rm;
    level->food[i].x = pnt.x;
    level->food[i].y = pnt.y;
}

void capture_item(Game *game)
{
    int r = game->hero.x;
    int c = game->hero.y;
    for(int i = 0; i < game->level[game->cur_level].cnt_gold; i++)
        if(game->level[game->cur_level].gold[i].x == r &&
           game->level[game->cur_level].gold[i].y == c)
        {
           game->level[game->cur_level].gold[i].used = 1;
           game->score += game->level[game->cur_level].gold[i].value;
           ///add to gold
        }

    for(int i = 0; i < 5; i++)
        if(game->level[game->cur_level].food[i].x == r &&
           game->level[game->cur_level].food[i].y == c)
        {
            set_food(&game->level[game->cur_level], i);
            game->food_save++;
            ///eat
        }
}

