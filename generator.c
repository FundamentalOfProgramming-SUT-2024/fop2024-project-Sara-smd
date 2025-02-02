#include "mybasics.h"

int absolute(int x)
{
    if(x < 0)
        return -x;
    return x;
}

point random_point(Room room, int d)
{
    int r = rand() % (room.height - 2 * d) + d;
    int c = rand() % (room.width - 2 * d) + d;
    point res;
    res.x = r;
    res.y = c;
    return res;
}

void localize_point(Room room, point *pnt)
{
    pnt->x -= room.corner.x;
    pnt->y -= room.corner.y;
}

void generalize_point(Room room, point *pnt)
{
    pnt->x += room.corner.x;
    pnt->y += room.corner.y;
}

int check_available(Level level, int rm, point pnt)
{
    localize_point(level.map.room[rm], &pnt);
    int r = pnt.x;
    int c = pnt.y;
    if(level.map.room[rm].floor[r][c].contain != '.')
        return 0;

    generalize_point(level.map.room[rm], &pnt);
    r = pnt.x;
    c = pnt.y;
    for(int i = 0; i < level.cnt_gold; i++)
        if(level.gold[i].used == 0)
            if(level.gold[i].x == r &&
               level.gold[i].y == c)
                return 0;
    ///add extra conditions
    return 1;
}
