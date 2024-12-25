#ifndef BULLET_H
#define BULLET_H

typedef struct _bullet
{
    int state;

    int pos_X;
    int pos_Y;

    int width;
    int height;

} bullet;

void bullet_init(bullet Bullets[], int size);

#endif