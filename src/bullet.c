#include "bullet.h"

void bullet_init(bullet Bullets[], int size)
{
    for (int i = 0; i < size; i++)
    {
        Bullets[i].state = 0;

        Bullets[i].pos_X = 100;
        Bullets[i].pos_Y = 100;

        Bullets[i].width = 5;
        Bullets[i].height = 10;
    }
}