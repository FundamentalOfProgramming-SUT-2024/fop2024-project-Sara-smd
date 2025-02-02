#include "mybasics.h"

void handle_input(Game *, int, int *);
void prepro(Game *, Sett);
void end_game(Game *);

void new_game(Sett st)
{
    noecho();
    timeout(0);
    Game game;
    prepro(&game, st);

    int sh = 0;
    while(true)
    {
        draw(&game, sh);
        int ch = getch();
        if(ch == 10)
            break;
        handle_input(&game, ch, &sh);
        usleep(DELAY);
        clear();
    }
    //end_game(&game);
    clear();
    return;
}

void prepro(Game *game, Sett st)
{
    game->cur_level = 0;
    game->cur_corr = -1;
    game->on_door = 0;
    game->cur_room = rand() % 6;
    game->last_check = time(NULL);
    game->last_update = time(NULL);
    game->last_message = time(NULL);
    game->food_save = 0;
    game->score = 0;
    game->health = 12;
    game->hunger = 0;
    game->message = DEFAULT_MESSAGE;
    game->settings = st;

    create_levels(game->level, game->cur_room);

    point tmp = random_point(game->level[game->cur_level].map.room[game->cur_room], 1);
    generalize_point(game->level[game->cur_level].map.room[game->cur_room], &tmp);
    game->hero.x = tmp.x;
    game->hero.y = tmp.y;
    reveal_room(game);
}

void handle_input(Game *game, int ch, int *sh)
{
    if(ch == 'M')
        *sh ^= 1;
    if(ch == 'E')
        show_food_menu(game);
    if(ch == 32)
    {
        change_level(game);
        capture_item(game);
    }
    else
        handle_movement(game, ch);
}

void end_game(Game *game)
{
//    for(int i = 0; i < 4; i++)
//        free(game->level[i]);
    return;
}
