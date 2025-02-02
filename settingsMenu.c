#include "mybasics.h"

void settings_menu()
{
    const char *colors[] = {"Red", "Green", "Blue"};
    const char *difficult[] = {"Easy", "Medium", "Hard"};

    int clr = 0;
    int dif = 0;
    while(true)
    {
        mvprintw(LINES / 3, 2 * COLS / 5 + 8, "Settings");
        mvprintw(LINES / 3 + 3, 2 * COLS / 5, "Color : ");
        attron(A_REVERSE);
        mvprintw(LINES / 3 + 3, 2 * COLS / 5 + 17, "%s", colors[clr]);
        attroff(A_REVERSE);
        mvprintw(LINES / 3 + 5, 2 * COLS / 5, "Difficulty : ");
        mvprintw(LINES / 3 + 5, 2 * COLS / 5 + 17, "%s", difficult[dif]);

        int ch = getch();
        if(ch == 10)
            break;
        if(ch == KEY_UP)
            clr = (clr + 2) % 3;
        if(ch == KEY_DOWN)
            clr = (clr + 1) % 3;
        clear();
    }

    while(true)
    {
        mvprintw(LINES / 3, 2 * COLS / 5 + 8, "Settings");
        mvprintw(LINES / 3 + 3, 2 * COLS / 5, "Color : ");
        mvprintw(LINES / 3 + 3, 2 * COLS / 5 + 17, "%s", colors[clr]);
        mvprintw(LINES / 3 + 5, 2 * COLS / 5, "Difficulty : ");
        attron(A_REVERSE);
        mvprintw(LINES / 3 + 5, 2 * COLS / 5 + 17, "%s", difficult[dif]);
        attroff(A_REVERSE);

        int ch = getch();
        if(ch == 10)
            break;
        if(ch == KEY_UP)
            dif = (dif + 2) % 3;
        if(ch == KEY_DOWN)
            dif = (dif + 1) % 3;
        clear();
    }

    clear();
}
