#include "invader.h"

void invader_init(invader Invaders[], int InvadersPerRow, int Rows, int InvaderWidth, int InvaderHeight, const int ScreenWidth)
{
    int InnerPadding = (ScreenWidth - (InvadersPerRow * InvaderWidth)) / (InvadersPerRow + 1);

    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < InvadersPerRow; j++)
        {
            int index = (i * InvadersPerRow) + j;

            Invaders[index].state = 1;

            Invaders[index].pos_x = InnerPadding + (j * (InvaderWidth + InnerPadding));
            Invaders[index].pos_y = InnerPadding + (i * (InvaderHeight + InnerPadding));

            Invaders[index].width = InvaderWidth;
            Invaders[index].height = InvaderHeight;
        }
    }
}

// | aaa aaa aaa aaa |