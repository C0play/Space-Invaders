#ifndef BULLET_H
#define BULLET_H

typedef struct _bullet
{
    int state;

    int pos_x;
    int pos_y;

    int width;
    int height;

} bullet;

void bullet_init(bullet Bullets[], int size);

#endif