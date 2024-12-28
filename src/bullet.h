#ifndef BULLET_H
#define BULLET_H

// Includes
#include <stdbool.h>

// Defines
#define BulletSpawnRate 0.8
#define BulletSpeed 3

// Structs
typedef struct _bullet
{
    bool state;

    int pos_x;
    int pos_y;

    int width;
    int height;

} bullet;

// Functions
void bullet_init(bullet Bullets[], int size);

#endif