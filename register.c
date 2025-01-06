#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>

typedef struct
{
    char* UserName;
    char* Password;
    char* Email;
} Player;

typedef struct
{
    Player* Users;
    int Users_Count;
} Game;

void init_game(Game*);
void IntroPage(Game*);
void LoginPage(Game*);
void RegisterPage(Game*);
int check(Game*, char*, char*);

int main()
{
    initscr();
    start_color();
    srand(time(0));
    curs_set(false);
    keypad(stdscr, true);

    Game game;
    init_game(&game);
    IntroPage(&game);

    endwin();
    return 0;
}

void init_game(Game* game)
{
    game -> Users_Count = 0;
    game -> Users = (Player*)malloc(sizeof(Player));
}

void IntroPage(Game* game)
{
    const char* option[] = {"Login", "Register"};
    int choice = 0;

    while(true)
    {
        for(int i = 0; i < 2; i++)
        {
            if(choice == i)
                attron(A_REVERSE);
            mvprintw(LINES / 2 - 4 + i, COLS / 2 - 4, "%s", option[i]);
            if(choice == i)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if(ch == KEY_UP)
            choice = 1 - choice;
        else if(ch == KEY_DOWN)
            choice = 1 - choice;
        else if (ch == 10)
            break;
    }
    clear();

    if(choice == 0)
        LoginPage(game);
    else
        RegisterPage(game);
}

void LoginPage(Game* game)
{
    mvprintw(LINES / 2 - 5, COLS / 3, "Please Enter Your Username and Password");
    char* username = (char*)malloc(50 * sizeof(char));
    char* password = (char*)malloc(50 * sizeof(char));
    mvprintw(LINES / 2 - 4, COLS / 3, "%s", "UserName: ");
    mvprintw(LINES / 2 - 3, COLS / 3, "%s", "Password: ");
    curs_set(true);
    mvgetnstr(LINES / 2 - 4, COLS / 3 + 10, username, 50);
    mvgetnstr(LINES / 2 - 3, COLS / 3 + 10, password, 50);
    curs_set(false);
    clear();
    if(check(game, username, password))
    {
        mvprintw(LINES / 2 - 4, COLS / 2 - 10, "Welcome %s!", username);
        while(true)
        {
            char c = getch();
            break;
        }
        clear();
    }
    else
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 14, "Invalid Username or Password.");
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
        while(1)
        {
            char c = getch();
            break;
        }
        clear();
        LoginPage(game);
    }
}

void RegisterPage(Game* game)
{
    clear();
}

int check(Game* game, char* username, char* password)
{
    return rand() % 2;
}
