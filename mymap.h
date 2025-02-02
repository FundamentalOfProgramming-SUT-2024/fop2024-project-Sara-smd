#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#ifndef POINT
#define POINT
typedef struct
{
    int x;
    int y;
    char contain;
    int visible;
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

typedef struct
{
    Room room[6];
    Corridor corridor[7];
    int is[7];
    int trans;
    point staircase;
} Map;

int absolute(int);
int contain_stair(Room, point);
int check_conflict(Corridor *, int, int);
int conflict(Corridor *, int *, int );
void build_pillar(Room *, int);
point random_point(Room, int);

void build_room(Room *room, int r)
{
    room[r].floor[0][0].x = room[r].corner.x;
    room[r].floor[0][0].y = room[r].corner.y;
    room[r].floor[0][0].contain = ' ';
    room[r].floor[0][0].visible = 0;

    room[r].floor[0][room[r].width - 1].x = room[r].corner.x;
    room[r].floor[0][room[r].width - 1].y = room[r].corner.y + room[r].width - 1;
    room[r].floor[0][room[r].width - 1].contain = ' ';
    room[r].floor[0][room[r].width - 1].visible = 0;

    for(int j = 1; j < room[r].width - 1; j++)
    {
        room[r].floor[0][j].x = room[r].corner.x;
        room[r].floor[0][j].y = room[r].corner.y + j;
        room[r].floor[0][j].contain = '_';
        room[r].floor[0][j].visible = 0;

        room[r].floor[room[r].height - 1][j].x = room[r].corner.x + room[r].height - 1;
        room[r].floor[room[r].height - 1][j].y = room[r].corner.y + j;
        room[r].floor[room[r].height - 1][j].contain = '_';
        room[r].floor[room[r].height - 1][j].visible = 0;
    }
    for(int i = 1; i < room[r].height; i++)
    {
        room[r].floor[i][0].x = room[r].corner.x + i;
        room[r].floor[i][0].y = room[r].corner.y;
        room[r].floor[i][0].contain = '|';

        room[r].floor[i][room[r].width - 1].x = room[r].corner.x + i;
        room[r].floor[i][room[r].width - 1].y = room[r].corner.y + room[r].width - 1;
        room[r].floor[i][room[r].width - 1].contain = '|';
        room[r].floor[i][room[r].width - 1].visible = 0;
    }
    for(int i = 1; i < room[r].height - 1; i++)
        for(int j = 1; j < room[r].width - 1; j++)
        {
            room[r].floor[i][j].x = room[r].corner.x + i;
            room[r].floor[i][j].y = room[r].corner.y + j;
            room[r].floor[i][j].contain = '.';
            room[r].floor[i][j].visible = 0;
        }
}

void create_room(Room *room, int m, point stair)
{
    int row[] = {4, 16};
    int clm[] = {4, 44, 84};
    int h = 10, w = 32;

    for(int i = 0; i < 6; i++)
    {
        do
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
        while(i == m && !contain_stair(room[i], stair));
    }

    for(int r = 0; r < 6; r++)
    {
        build_room(room, r);
        build_pillar(room, r);
    }
}

void build_corridor(Room *room, Corridor *corridor, int *is, int i)
{
    if(corridor[i].direction == 0)
    {
        corridor[i].start.x = rand() % (room[corridor[i].st_room].height - 2) + room[corridor[i].st_room].corner.x + 1;
        corridor[i].start.y = room[corridor[i].st_room].corner.y + room[corridor[i].st_room].width - 1;
        corridor[i].start.contain = '+';
        corridor[i].start.visible = 0;

        corridor[i].end.x = rand() % (room[corridor[i].en_room].height - 2) + room[corridor[i].en_room].corner.x + 1;
        corridor[i].end.y = room[corridor[i].en_room].corner.y;
        corridor[i].end.contain = '+';
        corridor[i].end.visible = 0;

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
            corridor[i].trail[0].visible = 0;
            for(int j = 0; j + 1 < corridor[i].horizontal; j++)
            {
                if(j > 0)
                {
                    corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x;
                    corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y + 1;
                    corridor[i].trail[j + k].contain = '#';
                    corridor[i].trail[j + k].visible = 0;
                }
                if(j == corridor[i].turn)
                {
                    while(k < corridor[i].vertical)
                    {
                        k++;
                        corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x + corridor[i].sub;
                        corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y;
                        corridor[i].trail[j + k].contain = '#';
                        corridor[i].trail[j + k].visible = 0;
                    }
                }
            }
        }
        while(conflict(corridor, is, i));
    }
    else
    {
        corridor[i].start.x = room[corridor[i].st_room].corner.x + room[corridor[i].st_room].height - 1;
        corridor[i].start.y = rand() % (room[corridor[i].st_room].width - 2) + room[corridor[i].st_room].corner.y + 1;
        corridor[i].start.contain = '+';
        corridor[i].start.visible = 0;

        corridor[i].end.x = room[corridor[i].en_room].corner.x;
        corridor[i].end.y = rand() % (room[corridor[i].en_room].width - 2) + room[corridor[i].en_room].corner.y + 1;
        corridor[i].end.contain = '+';
        corridor[i].end.visible = 0;

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
            corridor[i].trail[0].visible = 0;
            for(int j = 0; j + 1 < corridor[i].vertical; j++)
            {
                if(j > 0)
                {
                    corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x + 1;
                    corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y;
                    corridor[i].trail[j + k].contain = '#';
                    corridor[i].trail[j + k].visible = 0;
                }
                if(j == corridor[i].turn)
                {
                    while(k < corridor[i].horizontal)
                    {
                        k++;
                        corridor[i].trail[j + k].x = corridor[i].trail[j + k - 1].x;
                        corridor[i].trail[j + k].y = corridor[i].trail[j + k - 1].y + corridor[i].sub;
                        corridor[i].trail[j + k].contain = '#';
                        corridor[i].trail[j + k].visible = 0;
                    }
                }
            }
        }
        while(conflict(corridor, is, i));

    }
    room[corridor[i].st_room].floor[corridor[i].start.x - room[corridor[i].st_room].corner.x][corridor[i].start.y - room[corridor[i].st_room].corner.y].contain = '+';
    room[corridor[i].en_room].floor[corridor[i].end.x - room[corridor[i].en_room].corner.x][corridor[i].end.y - room[corridor[i].en_room].corner.y].contain = '+';
}

void create_corridor(Room *room, Corridor *corridor, int *is)
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
            build_corridor(room, corridor, is, i);
}

void build_pillar(Room *room, int k)
{
    int cnt = rand() % 3;
    for(int i = 0; i < cnt; i++)
    {
        point tmp = random_point(room[k], 2);
        tmp.x -= room[k].corner.x;
        tmp.y -= room[k].corner.y;
        room[k].floor[tmp.x][tmp.y].contain = 'O';
    }
}

void draw(Map map)
{
    Room *room = map.room;
    Corridor *corridor = map.corridor;
    int *is = map.is;
    for(int r = 0; r < 6; r++)
    {
        for(int i = 0; i < room[r].height; i++)
        {
            for(int j = 0; j < room[r].width; j++)
            {
                move(room[r].floor[i][j].x, room[r].floor[i][j].y);
                if(room[r].floor[i][j].visible)
                    printw("%c", room[r].floor[i][j].contain);
            }
        }
    }

    for(int i = 0; i < 7; i++)
        if(is[i])
        {
            for(int j = 0; j < corridor[i].length; j++)
            {
                move(corridor[i].trail[j].x, corridor[i].trail[j].y);
                if(corridor[i].trail[j].visible)
                    printw("%c", corridor[i].trail[j].contain);
            }
        }
    return;
}

int absolute(int x)
{
    if(x < 0)
        return -x;
    return x;
}

int contain_stair(Room room, point stair)
{
    if(stair.x <= room.corner.x || stair.x + 1 >= room.corner.x + room.height)
        return 0;
    if(stair.y <= room.corner.y || stair.y + 1 >= room.corner.y + room.width)
        return 0;
    return 1;
}

int check_conflict(Corridor *corridor, int a, int b)
{
    for(int i = 0; i < corridor[a].length; i++)
        for(int j = 0; j < corridor[b].length; j++)
            if(corridor[a].trail[i].x == corridor[b].trail[j].x && corridor[a].trail[i].y == corridor[b].trail[j].y)
                return 1;
    return 0;
}

int conflict(Corridor *corridor, int *is, int c)
{
    for(int i = 0; i < c; i++)
        if(is[i] && check_conflict(corridor, c, i))
            return 1;
    return 0;
}

point random_point(Room room, int d)
{
    int r = rand() % (room.height - 2 * d) + d;
    int c = rand() % (room.width - 2 * d) + d;
    r += room.corner.x;
    c += room.corner.y;
    point res;
    res.x = r;
    res.y = c;
    return res;
}
#endif // POINT
