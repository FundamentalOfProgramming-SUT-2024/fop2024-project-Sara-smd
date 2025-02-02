#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#include"mypregame.h"

void init_game();
void IntroPage();
void LoginMode();
void LoginPage();
void GuestLogin();
void RegisterPage();
void SaveUser(char*, char*, char*, const char *);
int check(char*, char*);
int check_email(char*);
int check_password(char*);
int check_username(char*);

int main()
{
    initscr();
    start_color();
    srand(time(0));
    curs_set(false);
    keypad(stdscr, true);

    IntroPage();

    endwin();
    return 0;
}

void IntroPage()
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
        LoginMode();
    else
        RegisterPage();
    return;
}

void LoginMode()
{
    const char* option[] = {"Login as registered player", "Login as guest"};
    int choice = 0;

    while(true)
    {
        for(int i = 0; i < 2; i++)
        {
            if(choice == i)
                attron(A_REVERSE);
            mvprintw(LINES / 2 - 4 + i, COLS / 2 - 7, "%s", option[i]);
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
        LoginPage();
    else
        GuestLogin();
    return;
}

void LoginPage()
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
    if(check(username, password))
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 10, "Welcome %s!", username);
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
        char c = getch();
        clear();
        char *name = (char*)malloc(60 * sizeof(char));
        strcpy(name, username);
        strcat(name, ".txt");
        PreGame(name);
        return;
    }
    else
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 14, "Invalid Username or Password.");
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
        char c = getch();
        clear();
        LoginPage();
        return;
    }
}

void GuestLogin()
{
    attron(A_BOLD);
    mvprintw(LINES / 2 - 4, COLS / 2 - 10, "Welcome Guest!");
    attroff(A_BOLD);
    mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
    char c = getch();
    clear();
    PreGame("Guest.txt");
    return;
}

void RegisterPage()
{
    mvprintw(LINES / 2 - 6, COLS / 3, "Please Choose Your Username and Password");
    mvprintw(LINES / 2 - 5, COLS / 3, "and Enter Your Email Address");
    char* username = (char*)malloc(50 * sizeof(char));
    char* password = (char*)malloc(50 * sizeof(char));
    char* email = (char*)malloc(50 * sizeof(char));
    mvprintw(LINES / 2 - 4, COLS / 3, "%s", "UserName: ");
    mvprintw(LINES / 2 - 3, COLS / 3, "%s", "Password: ");
    mvprintw(LINES / 2 - 2, COLS / 3, "%s", "Email: ");
    curs_set(true);
    mvgetnstr(LINES / 2 - 4, COLS / 3 + 10, username, 50);
    mvgetnstr(LINES / 2 - 3, COLS / 3 + 10, password, 50);
    mvgetnstr(LINES / 2 - 2, COLS / 3 + 10, email, 50);
    curs_set(false);
    clear();

    if(!check_username(username))
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 24, "This Username is Already Used by Another Player!");
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
        char c = getch();
        clear();
        RegisterPage();
        return;
    }
    if(!check_password(password))
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Invalid Password!");
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 24, "your password must contain at least 7 characters;");
        mvprintw(LINES / 2 - 3, COLS / 2 - 24, "including a digit, lowercase and uppercase letter");
        mvprintw(LINES / 2 - 1, COLS / 2 - 24, "press any key to continue...");
        char c = getch();
        clear();
        RegisterPage();
        return;
    }
    if(!check_email(email))
    {
        attron(A_BOLD);
        mvprintw(LINES / 2 - 4, COLS / 2 - 14, "Invalid Email Format!");
        attroff(A_BOLD);
        mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
        char c = getch();
        clear();
        RegisterPage();
        return;
    }
    char *name = (char*)malloc(60 * sizeof(char));
    strcpy(name, username);
    strcat(name, ".txt");
    SaveUser(username, password, email, name);
    clear();
    attron(A_BOLD);
    mvprintw(LINES / 2 - 4, COLS / 2 - 12, "Registered Successfully!");
    attroff(A_BOLD);
    mvprintw(LINES / 2 - 2, COLS / 2 - 14, "press any key to continue...");
    char c = getch();
    clear();
    PreGame(name);
    return;
}

void SaveUser(char* Username, char* Password, char* Email, const char *NAME)
{
    FILE* frtp;
    frtp = fopen("number.txt", "r");
    int n;
    fscanf(frtp, "%d", &n);
    n++;
    frtp = fopen("number.txt", "w");
    fprintf(frtp, "%d\n", n);

    FILE* fptr;
    fptr = fopen("text.txt", "a");
    fprintf(fptr, "%s %s %s\n", Username, Password, Email);

    FILE *nptr;
    nptr = fopen(NAME, "w");
    fprintf(nptr, "%d\n", 0);
}

int check_username(char* Username)
{
    FILE* frtp;
    frtp = fopen("number.txt", "r");
    int n;
    fscanf(frtp, "%d", &n);
    FILE* fptr;
    fptr = fopen("text.txt", "r");

    char username[50], password[50], email[50];

    for(int i = 0; i < n; i++)
    {
        fscanf(fptr, "%s %s %s", username, password, email);
        if(strcmp(username, Username) == 0)
            return 0;
    }
    if(strcmp("Guest", Username) == 0)
        return 0;
    return 1;
}

int check_password(char* Password)
{
    if(strlen(Password) < 7)
        return 0;
    int is = 0;
    for(int i = 0; i < strlen(Password); i++)
    {
        if(Password[i] >= '0' && Password[i] <= '9')
            is |= 1;
        if(Password[i] >= 'a' && Password[i] <= 'z')
            is |= 2;
        if(Password[i] >= 'A' && Password[i] <= 'Z')
            is |= 4;
    }
    if(is == 7)
        return 1;
    return 0;
}

int check_email(char* email)
{
    int n = strlen(email);
    int i = 0;
    while(i < n && email[i] != '@')
        i++;
    if(i == 0 || i == n)
        return 0;
    i++;
    while(i < n && email[i] != '.')
    {
        if(email[i] == '@')
            return 0;
        i++;
    }
    if(email[i - 1] == '@' || i == n)
        return 0;
    i++;
    if(i == n)
        return 0;
    while(i < n)
    {
        if(email[i] == '@' || email[i] == '.')
            return 0;
        i++;
    }
    return 1;
}

int check(char* Username, char* Password)
{
    FILE* frtp;
    frtp = fopen("number.txt", "r");
    int n;
    fscanf(frtp, "%d", &n);
    FILE* fptr;
    fptr = fopen("text.txt", "r");

    char username[50], password[50], email[50];

    for(int i = 0; i < n; i++)
    {
        fscanf(fptr, "%s %s %s", username, password, email);
        if(strcmp(username, Username) == 0)
        {
            if(strcmp(password, Password) == 0)
                return 1;
            return 0;
        }
    }
    return 0;
}
