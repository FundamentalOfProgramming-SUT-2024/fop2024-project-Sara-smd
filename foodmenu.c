#include "mybasics.h"

void show_food_menu(Game *game)
{
    clear();
    while(true)
    {
        move(LINES / 5, 2 * COLS / 5);
        for(int i = 0; i < COLS / 5; i++)
            printw("#");
        for(int i = LINES / 5; i <= 4 * LINES / 5; i++)
        {
            mvprintw(i, 2 * COLS / 5, "#");
            mvprintw(i, 3 * COLS / 5, "#");
        }
        move(4 * LINES / 5, 2 * COLS / 5);
        for(int i = 0; i < COLS / 5; i++)
            printw("#");

        mvprintw(LINES / 5 + 2, 2 * COLS / 5 + 8, "Food Menu");
        mvprintw(LINES / 5 + 5, 2 * COLS / 5 + 2, "normal food");
        mvprintw(LINES / 5 + 5, 2 * COLS / 5 + 22, "%d", game->food_save);
        mvprintw(4 * LINES / 5 - 3, 2 * COLS / 5 + 5, "press enter to");
        mvprintw(4 * LINES / 5 - 2, 2 * COLS / 5 + 5, "resume the game");

        int ch = getch();
        if(ch == 10)
            break;
        if(ch == 'e' && game->food_save > 0)
        {
            game->food_save--;
            game->hunger -= 3;
            if(game->hunger < 0)
                game->hunger = 0;
            game->last_update = time(NULL);
        }
    }
    clear();
    game->last_check = time(NULL);
    game->last_update = time(NULL);
    return;
}
