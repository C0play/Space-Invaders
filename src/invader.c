#include "invader.h"

void invader_init(invader Invaders[], const int ScreenWidth)
{
    int cellSize = ScreenWidth / Columns;
    int invaderSize = cellSize / 2;
    int startingX = (Columns - InvadersPerRow) / 2 * cellSize;
    int startingY = 60; // Same as Player.pos_y from the bottom of the window

    for (int y = 0; y < Rows; y++)
    {
        for (int x = 0; x < InvadersPerRow; x++)
        {
            int index = y * InvadersPerRow + x;
            Invaders[index].pos_x = startingX + x * cellSize + (cellSize - invaderSize) / 2;
            Invaders[index].pos_y = startingY + y * cellSize + (cellSize - invaderSize) / 2;

            Invaders[index].state = 1;
            Invaders[index].height = invaderSize;
            Invaders[index].width = invaderSize;
        }
    }
}

void moveInvadersHorizontal(invader Invaders[], int index, int distance)
{
    Invaders[index].pos_x += distance;
}

void moveInvadersVertical(invader Invaders[], int index, int distance)
{
    Invaders[index].pos_y += distance;
}

// Return row number in which and invader is active or -1 if none are
int lowestInvaderY(invader Invaders[])
{
    int lowestY = -1;
    for (int i = InvadersPerRow * Rows - 1; i >= 0; i--)
    {
        if (Invaders[i].state == 1) // Only consider active invaders
        {
            lowestY = Invaders[i].pos_y;
            break;
        }
    }
    return lowestY;
}

// |  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20
// | ___ ___ ___ aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa ___ ___ ___ |
// | ___ ___ ___ aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa ___ ___ ___ |
// | ___ ___ ___ aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa aaa ___ ___ ___ |