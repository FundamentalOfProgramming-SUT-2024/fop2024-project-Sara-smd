#include "mybasics.h"

void PreGame()
{
    noecho();

    const char *option[] = {"New Game", "Resume Game", "Settings", "Scoreboard"};
    int choice = 0;

    while(true)
    {
        for(int i = 0; i < 4; i++)
        {
            if(choice == i)
                attron(A_REVERSE);
            mvprintw(LINES / 3 + 2 * i, 2 * COLS / 5, "%s", option[i]);
            if(choice == i)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if(ch == KEY_UP)
            choice = (choice + 3) % 4;
        else if(ch == KEY_DOWN)
            choice = (choice + 1) % 4;
        else if(ch == 10)
            break;
    }
    clear();

    if(choice == 0)
        new_game();
    else if(choice == 2)
        settings_menu();
    clear();
    return;
}
