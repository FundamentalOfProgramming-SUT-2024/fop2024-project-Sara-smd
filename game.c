#include "mybasics.h"

void handle_input(Game *, int, int *);

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
    game.last_check = time(NULL);
    game.last_update = time(NULL);
    game.health = 12;
    game.hunger = 0;

    create_levels(game.level, game.cur_room);

    point tmp = random_point(game.level[game.cur_level].map.room[game.cur_room], 1);
    generalize_point(game.level[game.cur_level].map.room[game.cur_room], &tmp);
    game.hero.x = tmp.x;
    game.hero.y = tmp.y;
    reveal_room(&game);

    int sh = 0;
    //int delay = 100000;
    while(true)
    {
        draw(&game, sh);
        int ch = getch();
        if(ch == 10)
            break;
        handle_input(&game, ch, &sh);
        //usleep(delay);
        refresh();
        clear();
    }

    endwin();
    return 0;
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

