#ifndef INVADER_H
#define INVADER_H

typedef struct _invader
{
    int state;

    int pos_x;
    int pos_y;

    int width;
    int height;

} invader;

void invader_init(invader Invaders[], int InvadersPerRow, int Rows, int InvaderWidth, int Padding, const int ScreenWidth);

#endif // INVADER_H