/********************************************************************************************
 *
 *   To compile, just press F5.
 *
 ********************************************************************************************/

// Includes ------------------------------------------------------------------------
#include <stdlib.h>
#include "raylib.h"
#include "player.h"
#include "bullet.h"
#include "invader.h"
// ---------------------------------------------------------------------------------

// Defines--------------------------------------------------------------------------
#define NEARBLACK (Color){26, 26, 26, 255}

#define PlayerSpeed 5
#define PlayerHealth 100
#define HealthLoss 20
#define PlayerMagazineSize 5

#define InvadersPerRow 14
#define Rows 3
#define InvaderWidth 35
#define InvaderHeight 20
#define EnemyMagazineSize 10

#define BulletSpawnRate 0.8
#define BulletSpeed 3
//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
const int screenWidth = 1000;
const int screenHeight = 1000;
const char *title = "Space Invaders";

player Player = (player){PlayerHealth, 30, screenHeight - 60, 75, 20};

bullet PlayerBullets[PlayerMagazineSize];
bullet EnemyBullets[EnemyMagazineSize];
double lastTimeCheck = 0.0;

invader Invaders[InvadersPerRow * Rows];
int invaderCount = InvadersPerRow * Rows;
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------
void DrawFrame();
void UpdatePlayerPosition();
void UpdatePlayerBullet();
void UpdateInvader();
void UpdateEnemyBullet();
void CheckEnemyBulletPlayerCollisions();
//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    invader_init(Invaders, InvadersPerRow, Rows, InvaderWidth, InvaderHeight, screenWidth);
    bullet_init(PlayerBullets, PlayerMagazineSize);
    bullet_init(EnemyBullets, EnemyMagazineSize);

    // TraceLog(LOG_INFO, "Padding: %d", (screenWidth - (InvadersPerRow * InvaderWidth)) / (InvadersPerRow + 1));

    InitWindow(screenWidth, screenHeight, title);

    // SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144);         // Set our game to run at 144 frames-per-second

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        DrawFrame();
        UpdatePlayerPosition();
        UpdatePlayerBullet();
        UpdateInvader();
        UpdateEnemyBullet();
        CheckEnemyBulletPlayerCollisions();
    }

    CloseWindow(); // Close window and OpenGL context
    // ----------------------------------------END-------------------------------------------

    return 0;
}

// Local Functions Implementation------------------------------------------------------
void DrawFrame()
{
    BeginDrawing();

    // Drawing background
    ClearBackground(NEARBLACK);

    // Win/Lose conditions
    if (Player.helath > 0 && invaderCount > 0) // Game in progress
    {
        // Drawing player
        DrawRectangle(Player.pos_x, Player.pos_y, Player.width, Player.height, BLUE);

        // Drawing player bullets
        for (int i = 0; i < PlayerMagazineSize; i++)
        {
            if (PlayerBullets[i].state == 1)
                DrawRectangle(PlayerBullets[i].pos_x, PlayerBullets[i].pos_y, PlayerBullets[i].width, PlayerBullets[i].height, GREEN);
        }

        // Drawing invaders
        for (int i = 0; i < InvadersPerRow * Rows; i++)
        {
            if (Invaders[i].state)
            {
                DrawRectangle(Invaders[i].pos_x, Invaders[i].pos_y, Invaders[i].width, Invaders[i].height, ORANGE);
            }
        }

        // Drawing enemy bullets
        for (int i = 0; i < EnemyMagazineSize; i++)
        {
            if (EnemyBullets[i].state == 1)
                DrawRectangle(EnemyBullets[i].pos_x, EnemyBullets[i].pos_y, EnemyBullets[i].width, EnemyBullets[i].height, RED);
        }

        // Drawing player health
        static char buffer[20];
        itoa(Player.helath, buffer, 10);
        const char *health = buffer;
        DrawText(health, 10, screenHeight - 35, 25, GREEN);

        // Drawing FPS
        DrawFPS(screenWidth - 30, screenHeight - 35);
    }
    else if (Player.helath <= 0) // Lose
    {
        DrawText("GAME OVER", screenWidth / 2 - 300, screenHeight / 2 - 100, 100, RED);
    }
    else if (invaderCount <= 0) // Win
    {
        DrawText("YOU WIN", screenWidth / 2 - 200, screenHeight / 2 - 100, 100, GREEN);
    }

    EndDrawing();
}

void UpdatePlayerPosition()
{
    if (IsKeyDown(KEY_RIGHT) && Player.pos_x < screenWidth - (Player.width + 10))
    {
        Player.pos_x += PlayerSpeed;
    }

    if (IsKeyDown(KEY_LEFT) && Player.pos_x > 10)
    {
        Player.pos_x -= PlayerSpeed;
    }
}

void UpdatePlayerBullet()
{
    // Initializing bullet on space press
    if (IsKeyPressed(KEY_SPACE))
    {
        // Looking for the first free bullet slot
        int i = 0;
        while (PlayerBullets[i].state == 1 && i < PlayerMagazineSize)
        {
            i++;
        }

        // Setting the bullet position to the middle of the player rectangle and it's state to 1
        PlayerBullets[i].pos_x = Player.pos_x + (Player.width / 2) - (PlayerBullets[i].width / 2);
        PlayerBullets[i].pos_y = Player.pos_y - Player.height - 5;
        PlayerBullets[i].state = 1;
    }
    // Checking if  the bullet is out of frame or updating the bullet's position
    for (int k = 0; k < PlayerMagazineSize; k++)
    {
        if (PlayerBullets[k].state == 1 && PlayerBullets[k].pos_y < 0 - PlayerBullets[k].height)
        {
            PlayerBullets[k].state = 0;
        }
        else if (PlayerBullets[k].state == 1)
        {
            PlayerBullets[k].pos_y -= BulletSpeed;
        }
    }
}

void UpdateInvader()
{
    // Checking all player bullets
    for (int i = 0; i < PlayerMagazineSize; i++)
    {
        // Checkig each bullet for collisions with every invader
        for (int j = 0; j < InvadersPerRow * Rows; j++)
        {
            // Check only if both bullets and invader are active
            if (Invaders[j].state == 1 && PlayerBullets[i].state == 1)
            {
                // Check if x coordinates a match
                if (PlayerBullets[i].pos_x >= Invaders[j].pos_x - PlayerBullets[i].width && PlayerBullets[i].pos_x <= Invaders[j].pos_x + Invaders[j].width)
                {
                    // Check if y coordinates a match
                    if (PlayerBullets[i].pos_y >= Invaders[j].pos_y && PlayerBullets[i].pos_y <= Invaders[j].pos_y + Invaders[j].height)
                    {
                        // Decreasing invaderCount for win condition and deactivating the bullet and invader
                        invaderCount--;
                        Invaders[j].state = 0;
                        PlayerBullets[i].state = 0;
                    }
                }
            }
        }
    }
}

void UpdateEnemyBullet()
{
    // Picking random invader to shoot a bullet
    int randomInvaderIndex = GetRandomValue(0, (Rows * InvadersPerRow) - 1);
    while (Invaders[randomInvaderIndex].state == 0 && invaderCount > 0)
        randomInvaderIndex = GetRandomValue(0, (Rows * InvadersPerRow) - 1);

    // Initializing bullet every "BulletSpawnRate" seconds
    double elapsedTime = GetTime();
    if (elapsedTime - lastTimeCheck >= BulletSpawnRate)
    {
        lastTimeCheck = elapsedTime;

        // Looking for the first free bullet slot
        int i = 0;
        while (EnemyBullets[i].state == 1 && i < EnemyMagazineSize)
        {
            i++;
        }

        // Setting the bullet position to the middle of the invader rectangle and it's state to 1
        EnemyBullets[i].pos_x = Invaders[randomInvaderIndex].pos_x + (InvaderWidth / 2) - (EnemyBullets[i].width / 2);
        EnemyBullets[i].pos_y = (Invaders[randomInvaderIndex].pos_y + InvaderHeight);
        EnemyBullets[i].state = 1;
    }
    // Checking if  the bullet is out of frame or updating the bullet's position
    for (int k = 0; k < PlayerMagazineSize; k++)
    {
        if (EnemyBullets[k].state == 1 && EnemyBullets[k].pos_y > screenHeight + EnemyBullets[k].height)
        {
            EnemyBullets[k].state = 0;
        }

        if (EnemyBullets[k].state == 1)
        {
            EnemyBullets[k].pos_y += BulletSpeed;
        }
    }
}

void CheckEnemyBulletPlayerCollisions()
{

    // Checking every enemy bullet
    for (int i = 0; i < EnemyMagazineSize; i++)
    {
        // Check only if the bullet is active
        if (EnemyBullets[i].state == 1)
        {
            // Check if x coordinates a match
            if (EnemyBullets[i].pos_x >= Player.pos_x - EnemyBullets[i].width && EnemyBullets[i].pos_x <= Player.pos_x + Player.width)
            {
                // Check if y coordinates a match
                if (EnemyBullets[i].pos_y + EnemyBullets[i].height >= Player.pos_y && EnemyBullets[i].pos_y + EnemyBullets[i].height <= Player.pos_y + Player.height)
                {
                    // Deactivating the bullet and subtracting player health
                    EnemyBullets[i].state = 0;
                    Player.helath -= HealthLoss;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------