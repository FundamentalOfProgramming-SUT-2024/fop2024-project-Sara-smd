#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#include"mymap.h"

typedef struct
{
    Map map;
} Level;

void set_stair(Level *, int, int);
void change_stair(Level *);
point find_inside(int);


int main()
{
    initscr();
    noecho();
    srand(time(NULL));

    Level level[4];
    point stair;
    int r = rand() % 6;
    stair = find_inside(r);
    for(int i = 0; i < 4; i++)
    {
        create_room(level[i].map.room, r, stair);
        create_corridor(level[i].map.room, level[i].map.corridor, level[i].map.is);
        set_stair(level, i, r);
        r = level[i].map.trans;
        stair = level[i].map.staircase;
    }

    change_stair(level);
    for(int i = 0; i < 4; i++)
    {
        draw(level[i].map);
        int ch = getch();
        clear();
    }

    endwin();
    return 0;
}

void set_stair(Level *level, int i, int m)
{
    int tmp = rand() % 6;
    if(tmp == m)
        tmp = (tmp + 2) % 6;
    level[i].map.trans = tmp;
    int r = rand() % (level[i].map.room[tmp].height - 2) + level[i].map.room[tmp].corner.x + 1;
    int c = rand() % (level[i].map.room[tmp].width - 2) + level[i].map.room[tmp].corner.y + 1;
    point stair;
    stair.x = r;
    stair.y = c;
    stair.contain = '<';
    level[i].map.staircase = stair;
}

point find_inside(int i)
{
    point res;
    int row[] = {4, 16};
    int clm[] = {4, 44, 84};
    int h = 10, w = 32;
    int c = rand() % (h - 2) + 1;
    int d = rand() % (w - 2) + 1;
    res.x = c + row[i / 3];
    res.y = d + clm[i % 3];
    res.contain = '<';
    return res;
}

void change_stair(Level *level)
{
    for(int i = 0; i < 4; i++)
    {
        if(i != 0)
        {
            int r = level[i - 1].map.staircase.x - level[i].map.room[level[i - 1].map.trans].corner.x;
            int c = level[i - 1].map.staircase.y - level[i].map.room[level[i - 1].map.trans].corner.y;
            level[i].map.room[level[i - 1].map.trans].floor[r][c].contain = '>';
        }
        if(i != 3)
        {
            int r = level[i].map.staircase.x - level[i].map.room[level[i].map.trans].corner.x;
            int c = level[i].map.staircase.y - level[i].map.room[level[i].map.trans].corner.y;
            level[i].map.room[level[i].map.trans].floor[r][c].contain = '<';
        }
    }
}
