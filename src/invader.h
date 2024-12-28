#ifndef INVADER_H
#define INVADER_H

// Includes
#include <stdbool.h>

// Defines
#define InvadersPerRow 14
#define EnemyMagazineSize 10
#define Columns 20
#define Rows 3
#define MovementInterval 1.0
#define MovementDistance 20
#define MovementMultiplier 1.05

// Structs
typedef struct _invader
{
    bool state;

    int pos_x;
    int pos_y;

    int width;
    int height;

} invader;

// Functions
void invader_init(invader Invaders[], const int ScreenWidth);

void moveInvadersHorizontal(invader Invaders[], int index, int distance);

void moveInvadersVertical(invader Invaders[], int index, int distance);

int lowestInvaderY(invader Invaders[]);

#endif // INVADER_H