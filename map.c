#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>

typedef struct
{
    int x;
    int y;
    char contain;
} point;

typedef struct
{
    point corner;
    int height;
    int width;
    point floor[15][15];
} Room;

typedef struct
{
    int st_room;
    int en_room;
    int direction;
    int vertical;
    int horizontal;
    int length;
    int turn;
    int sub;
    point start;
    point end;
    point trail[60];
} Corridor;

void draw();
void create_room();
void create_corridor();

int main()
{
    initscr();
    noecho();
    srand(time(NULL));
    while(true)
    {
        draw();
        char c = getch();
        if(c == 'e')
            break;
    }
    endwin();
    return 0;
}

Room room[6];
Corridor corridor[7];
int is[7];

void draw()
{
    create_room();
    for(int r = 0; r < 6; r++)
    {
        for(int i = 0; i < room[r].height; i++)
        {
            for(int j = 0; j < room[r].width; j++)
            {
                move(room[r].floor[i][j].x, room[r].floor[i][j].y);
                printw("%c", room[r].floor[i][j].contain);
            }
        }
    }

    create_corridor();

    getch();
    clear();
    return;
}

void build_room(int r)
{
    room[r].floor[0][0].x = room[r].corner.x;
    room[r].floor[0][0].y = room[r].corner.y;
    room[r].floor[0][0].contain = ' ';

    room[r].floor[0][room[r].width - 1].x = room[r].corner.x;
    room[r].floor[0][room[r].width - 1].y = room[r].corner.y + room[r].width - 1;
    room[r].floor[0][room[r].width - 1].contain = ' ';

    for(int j = 1; j < room[r].width - 1; j++)
    {
        room[r].floor[0][j].x = room[r].corner.x;
        room[r].floor[0][j].y = room[r].corner.y + j;
        room[r].floor[0][j].contain = '_';

        room[r].floor[room[r].height - 1][j].x = room[r].corner.x + room[r].height - 1;
        room[r].floor[room[r].height - 1][j].y = room[r].corner.y + j;
        room[r].floor[room[r].height - 1][j].contain = '_';
    }
    for(int i = 1; i < room[r].height; i++)
    {
        room[r].floor[i][0].x = room[r].corner.x + i;
        room[r].floor[i][0].y = room[r].corner.y;
        room[r].floor[i][0].contain = '|';

        room[r].floor[i][room[r].width - 1].x = room[r].corner.x + i;
        room[r].floor[i][room[r].width - 1].y = room[r].corner.y + room[r].width - 1;
        room[r].floor[i][room[r].width - 1].contain = '|';
    }
    for(int i = 1; i < room[r].height - 1; i++)
        for(int j = 1; j < room[r].width - 1; j++)
        {
            room[r].floor[i][j].x = room[r].corner.x + i;
            room[r].floor[i][j].y = room[r].corner.y + j;
            room[r].floor[i][j].contain = '.';
        }
}

void create_room()
{
    int row[] = {4, 16};
    int clm[] = {4, 44, 84};
    int h = 10, w = 32;

    for(int i = 0; i < 6; i++)
    {
        int a = rand() % 5 + 6;
        int b = rand() % 5 + 10;
        room[i].height = a;
        room[i].width = b;
        int c = rand() % (h - a + 1);
        int d = rand() % (w - b + 1);
        room[i].corner.x = c + row[i / 3];
        room[i].corner.y = d + clm[i % 3];
        room[i].corner.contain = ' ';
    }

    for(int r = 0; r < 6; r++)
        build_room(r);
}

int absolute(int x)
{
    if(x < 0)
        return -x;
    return x;
}

int check_conflict(int a, int b)
{
    for(int i = 0; i < corridor[a].length; i++)
        for(int j = 0; j < corridor[b].length; j++)
            if(corridor[a].trail[i].x == corridor[b].trail[j].x && corridor[a].trail[i].y == corridor[b].trail[j].y)
                return 1;
    return 0;
}

int conflict(int c)
{
    for(int i = 0; i < c; i++)
        if(is[i] && check_conflict(c, i))
            return 1;
    return 0;
}

void create_corridor()
{
    for(int c = 0; c < 2; c++)
    {
        corridor[c].st_room = c;
        corridor[c].en_room = c + 1;
        corridor[c].direction = 0;
    }
    for(int c = 2; c < 4; c++)
    {
        corridor[c].st_room = c + 1;
        corridor[c].en_room = c + 2;
        corridor[c].direction = 0;

    }
    for(int c = 4; c < 7; c++)
    {
        corridor[c].st_room = c - 4;
        corridor[c].en_room = c - 1;
        corridor[c].direction = 1;
    }
    int pos[] = {127, 63, 95, 111, 119, 123, 125, 126, 31, 79, 87, 91, 93, 94, 47, 59, 62, 103, 115, 118, 109, 121, 124};
    int ind = rand() % 23;
    for(int c = 0; c < 7; c++)
        is[c] = (pos[ind] & (1 << c)) >> c;
    for(int i = 0; i < 7; i++)
        if(is[i])
        {
            if(corridor[i].direction == 0)
            {
                corridor[i].start.x = rand() % (room[corridor[i].st_room].height - 2) + room[corridor[i].st_room].corner.x + 1;
                corridor[i].start.y = room[corridor[i].st_room].corner.y + room[corridor[i].st_room].width - 1;
                corridor[i].start.contain = '+';

                corridor[i].end.x = rand() % (room[corridor[i].en_room].height - 2) + room[corridor[i].en_room].corner.x + 1;
                corridor[i].end.y = room[corridor[i].en_room].corner.y;
                corridor[i].end.contain = '+';

                corridor[i].vertical = absolute(corridor[i].end.x - corridor[i].start.x);
                corridor[i].horizontal = absolute(corridor[i].end.y - corridor[i].start.y);
                corridor[i].length = corridor[i].horizontal + corridor[i].vertical - 1;
                corridor[i].sub = (corridor[i].start.x - corridor[i].end.x < 0 ? 1 : -1);

                do
                {
                    int k = 0;
                    corridor[i].turn = rand() % (corridor[i].horizontal - 1);
                    corridor[i].trail[0].x = corridor[i].start.x;
                    corridor[i].trail[0].y = corridor[i].start.y + 1;
                    corridor[i].trail[0].contain = '#';
                    for(int j = 0; j + 1 < corridor[i].horizontal; j++)
                    {
                        if(j > 0)
                        {
                            corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x;
                            corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y + 1;
                            corridor[i].trail[j + k].contain = '#';
                        }
                        if(j == corridor[i].turn)
                        {
                            while(k < corridor[i].vertical)
                            {
                                k++;
                                corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x + corridor[i].sub;
                                corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y;
                                corridor[i].trail[j + k].contain = '#';
                            }
                        }
                    }
                }
                while(conflict(i));
            }
            else
            {
                corridor[i].start.x = room[corridor[i].st_room].corner.x + room[corridor[i].st_room].height - 1;
                corridor[i].start.y = rand() % (room[corridor[i].st_room].width - 2) + room[corridor[i].st_room].corner.y + 1;
                corridor[i].start.contain = '+';

                corridor[i].end.x = room[corridor[i].en_room].corner.x;
                corridor[i].end.y = rand() % (room[corridor[i].en_room].width - 2) + room[corridor[i].en_room].corner.y + 1;
                corridor[i].end.contain = '+';

                corridor[i].vertical = absolute(corridor[i].end.x - corridor[i].start.x);
                corridor[i].horizontal = absolute(corridor[i].end.y - corridor[i].start.y);
                corridor[i].length = corridor[i].horizontal + corridor[i].vertical - 1;
                corridor[i].sub = (corridor[i].end.y - corridor[i].start.y > 0 ? 1 : -1);

                do
                {
                    int k = 0;
                    corridor[i].turn = rand() % (corridor[i].vertical - 1);
                    corridor[i].trail[0].x = corridor[i].start.x + 1;
                    corridor[i].trail[0].y = corridor[i].start.y;
                    corridor[i].trail[0].contain = '#';
                    for(int j = 0; j + 1 < corridor[i].vertical; j++)
                    {
                        if(j > 0)
                        {
                            corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x + 1;
                            corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y;
                            corridor[i].trail[j + k].contain = '#';
                        }
                        if(j == corridor[i].turn)
                        {
                            while(k < corridor[i].horizontal)
                            {
                                k++;
                                corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x;
                                corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y + corridor[i].sub;
                                corridor[i].trail[j + k].contain = '#';
                            }
                        }
                    }
                }
                while(conflict(i));
            }
        }

    for(int i = 0; i < 7; i++)
        if(is[i])
        {
            move(corridor[i].start.x, corridor[i].start.y);
            printw("%c", corridor[i].start.contain);

            move(corridor[i].end.x, corridor[i].end.y);
            printw("%c", corridor[i].end.contain);

            for(int j = 0; j < corridor[i].length; j++)
            {
                move(corridor[i].trail[j].x, corridor[i].trail[j].y);
                printw("%c", corridor[i].trail[j].contain);
            }
        }
}
