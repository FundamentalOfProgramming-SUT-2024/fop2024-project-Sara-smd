#include "mybasics.h"

void PreGame()
{
    noecho();
    //curs_set(false);
    const char *option[] = {"New Game", "Resume Game", "Settings", "Scoreboard"};
    int choice = 0;
    Sett st;
    st.color = 0;
    st.difficulty = 0;
    int exit_game = 0;

    while(true)
    {
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
            else if(ch == ' ')
            {
                exit_game = 1;
                break;
            }
            usleep(DELAY);
        }
        clear();

        if(exit_game)
            break;
        if(choice == 0)
            new_game(st);
        else if(choice == 2)
            st = settings_menu();
    }
    clear();
    return;
}
