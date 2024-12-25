/********************************************************************************************
 *
 *   To compile, just press F5.
 *
 ********************************************************************************************/

// Includes ------------------------------------------------------------------------
#include "raylib.h"
#include "player.h"
#include "bullet.h"
// ---------------------------------------------------------------------------------

// Defines--------------------------------------------------------------------------
#define NEARBLACK (Color){26, 26, 26, 255}
#define MagazineSize 30
//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
const int screenWidth = 640;
const int screenHeight = 640;
const char *title = "Test";

player Player = (player){200, 30, 30, 50, 15};
const Color Player_color = GREEN;

bullet Bullets[MagazineSize];
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------
void DrawFrame(const int screenWidth, const int screenHeight);
void UpdatePlayerPosition(player *Player, const int screenWidth);
void UpdateBullet(const int screenHeight, bullet Bullets[], player *Player);
//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    bullet_init(Bullets, MagazineSize);
    InitWindow(screenWidth, screenHeight, title);

    SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144);         // Set our game to run at 144 frames-per-second

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        DrawFrame(screenWidth, screenHeight);
        UpdatePlayerPosition(&Player, screenWidth);
        UpdateBullet(screenHeight, Bullets, &Player);
        DrawFPS(0, 0);
    }

    CloseWindow(); // Close window and OpenGL context
    // ----------------------------------------END-------------------------------------------

    return 0;
}

// Local Functions Implementation------------------------------------------------------
void DrawFrame(const int screenWidth, const int screenHeight)
{
    BeginDrawing();

    ClearBackground(NEARBLACK);

    int unused_variable __attribute__((unused)) = screenWidth; // To get rid of unused variable warning

    DrawRectangle(Player.pos_X, screenHeight - Player.pos_y, Player.width, Player.height, Player_color);

    EndDrawing();
}

void UpdatePlayerPosition(player *Player, const int screenWidth)
{
    if (IsKeyDown(KEY_RIGHT) && Player->pos_X < screenWidth - (2 * Player->width))
    {
        Player->pos_X += 6;
    }

    if (IsKeyDown(KEY_LEFT) && Player->pos_X > Player->width)
    {
        Player->pos_X -= 6;
    }
}

void UpdateBullet(const int screenHeight, bullet Bullets[], player *Player)
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
            Bullets[k].pos_Y -= 2;
        }
    }

    // Drawing bullet
    for (int k = 0; k < MagazineSize; k++)
    {
        if (Bullets[k].state == 1)
            DrawRectangle(Bullets[k].pos_X, Bullets[k].pos_Y, Bullets[k].width, Bullets[k].height, RED);
    }
}
//----------------------------------------------------------------------------------