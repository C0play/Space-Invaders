#ifndef PLAYER_H
#define PLAYER_H

// Defines
#define PlayerSpeed 5
#define PlayerHealth 100
#define HealthLoss 20
#define PlayerMagazineSize 5
#define PlayerWidth 80
#define PlayerHeight 20

// Structs
typedef struct _player
{
    int helath;
    
    int pos_x;
    int pos_y;

    int width;
    int height;

} player;

// Functions

#endif