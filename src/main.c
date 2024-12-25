/********************************************************************************************
 *
 *   To compile, just press F5.
 *
 ********************************************************************************************/

// Includes ------------------------------------------------------------------------
#include "raylib.h"
#include "player.h"
#include "bullet.h"
#include "invader.h"
// ---------------------------------------------------------------------------------

// Defines--------------------------------------------------------------------------
#define NEARBLACK (Color){26, 26, 26, 255}

#define PlayerSpeed 6

#define MagazineSize 30
#define BulletSpeed 4

#define InvadersPerRow 14
#define Rows 5
#define InvaderWidth 35
#define InvaderHeight 20
//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
const int screenWidth = 1000;
const int screenHeight = 1000;
const char *title = "Test";

player Player = (player){200, 30, 50, 75, 20};
const Color Player_color = GREEN;

bullet Bullets[MagazineSize];

invader Invaders[InvadersPerRow * Rows];
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------
void DrawFrame(const int screenWidth, const int screenHeight);
void UpdatePlayerPosition(player *Player, const int screenWidth);
void UpdateBullet(bullet Bullets[], player *Player, const int screenHeight);
void UpdateInvader(invader *Invaders, bullet *Bullets);
//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    bullet_init(Bullets, MagazineSize);
    invader_init(Invaders, InvadersPerRow, Rows, InvaderWidth, InvaderHeight, screenWidth);

    TraceLog(LOG_INFO, "Padding: %d", (screenWidth - (InvadersPerRow * InvaderWidth)) / (InvadersPerRow + 1));

    InitWindow(screenWidth, screenHeight, title);

    SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144);         // Set our game to run at 144 frames-per-second

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        DrawFrame(screenWidth, screenHeight);
        UpdatePlayerPosition(&Player, screenWidth);
        UpdateBullet(Bullets, &Player, screenHeight);
        UpdateInvader(Invaders, Bullets);
        DrawFPS(screenWidth - 30, screenHeight - 35);
    }

    CloseWindow(); // Close window and OpenGL context
    // ----------------------------------------END-------------------------------------------

    return 0;
}

// Local Functions Implementation------------------------------------------------------
void DrawFrame(const int screenWidth, const int screenHeight)
{
    int unused_variable __attribute__((unused)) = screenWidth; // To get rid of unused variable warning

    BeginDrawing();

    // Drawing background
    ClearBackground(NEARBLACK);

    // Drawing player
    DrawRectangle(Player.pos_X, screenHeight - Player.pos_y, Player.width, Player.height, Player_color);

    // Drawing bullet
    for (int i = 0; i < MagazineSize; i++)
    {
        if (Bullets[i].state == 1)
            DrawRectangle(Bullets[i].pos_X, Bullets[i].pos_Y, Bullets[i].width, Bullets[i].height, RED);
    }

    // Drawing invaders
    for (int i = 0; i < InvadersPerRow * Rows; i++)
    {
        if (Invaders[i].state)
        {
            DrawRectangle(Invaders[i].pos_X, Invaders[i].pos_Y, Invaders[i].width, Invaders[i].height, ORANGE);
        }
    }

    EndDrawing();
}

void UpdatePlayerPosition(player *Player, const int screenWidth)
{
    if (IsKeyDown(KEY_RIGHT) && Player->pos_X < screenWidth - (Player->width + 10))
    {
        Player->pos_X += PlayerSpeed;
    }

    if (IsKeyDown(KEY_LEFT) && Player->pos_X > 10)
    {
        Player->pos_X -= PlayerSpeed;
    }
}

void UpdateBullet(bullet Bullets[], player *Player, const int screenHeight)
{
    // Initializing bullet on space press
    if (IsKeyPressed(KEY_SPACE))
    {
        int i = 0;
        while (Bullets[i].state == 1 && i < MagazineSize)
        {
            i++;
        }

        Bullets[i].pos_X = Player->pos_X + (Player->width / 2) - (Bullets[i].width / 2);
        Bullets[i].pos_Y = screenHeight - (Player->pos_y + Player->height + 5);
        Bullets[i].state = 1;
    }
    // Checking if bullet is out of frame or updating bullet position
    for (int k = 0; k < MagazineSize; k++)
    {
        if (Bullets[k].state == 1 && Bullets[k].pos_Y < 0 - Bullets[k].height)
        {
            Bullets[k].state = 0;
        }

        if (Bullets[k].state == 1)
        {
            Bullets[k].pos_Y -= BulletSpeed;
        }
    }
}

void UpdateInvader(invader *Invaders, bullet *Bullets)
{

    for (int i = 0; i < MagazineSize; i++)
    {
        for (int j = 0; j < InvadersPerRow * Rows; j++)
        {
            // Check only if both bullets and invader are active
            if (Invaders[j].state == 1 && Bullets[i].state == 1)
            {
                // Check if x coordinates match
                if (Bullets[i].pos_X >= Invaders[j].pos_X - Bullets[i].width && Bullets[i].pos_X <= Invaders[j].pos_X + Invaders[j].width)
                {
                    // Check if y coordinates match
                    if (Bullets[i].pos_Y >= Invaders[j].pos_Y && Bullets[i].pos_Y <= Invaders[j].pos_Y + Invaders[j].height)
                    {
                        Invaders[j].state = 0;
                        Bullets[i].state = 0;
                    }
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------