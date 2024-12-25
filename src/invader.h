#ifndef INVADER_H
#define INVADER_H

typedef struct _invader
{
    int state;

    int pos_X;
    int pos_Y;

    int width;
    int height;

} invader;

void invader_init(invader Invaders[], int InvadersPerRow, int Rows, int InvaderWidth, int Padding, const int ScreenWidth);

#endif // INVADER_H