#include "bullet.h"

void bullet_init(bullet Bullets[], int size)
{
    for (int i = 0; i < size; i++)
    {
        Bullets[i].state = 0;

        Bullets[i].pos_x = 100;
        Bullets[i].pos_y = 100;

        Bullets[i].width = 10;
        Bullets[i].height = 15;
    }
}