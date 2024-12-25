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

#define PlayerMagazineSize 30
#define EnemyMagazineSize 10
#define BulletSpeed 4

#define InvadersPerRow 14
#define Rows 5
#define InvaderWidth 35
#define InvaderHeight 20
#define BulletSpawnRate 0.5
//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
const int screenWidth = 1000;
const int screenHeight = 1000;
const char *title = "Test";

player Player = (player){200, 30, screenHeight - 50, 75, 20};

bullet PlayerBullets[PlayerMagazineSize];
bullet EnemyBullets[EnemyMagazineSize];
double lastTimeCheck = 0.0;

invader Invaders[InvadersPerRow * Rows];
int invaderCount = InvadersPerRow * Rows;
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------
void DrawFrame();
void UpdatePlayerPosition(player *Player, const int screenWidth);
void UpdatePlayerBullet(bullet PlayerBullets[], player *Player);
void UpdateInvader(invader *Invaders, bullet *PlayerBullets);
void UpdateEnemyBullet(invader *Invaders);
void CheckEnemyBulletPlayerCollisions(void);
//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    bullet_init(EnemyBullets, EnemyMagazineSize);
    bullet_init(PlayerBullets, PlayerMagazineSize);
    invader_init(Invaders, InvadersPerRow, Rows, InvaderWidth, InvaderHeight, screenWidth);

    TraceLog(LOG_INFO, "Padding: %d", (screenWidth - (InvadersPerRow * InvaderWidth)) / (InvadersPerRow + 1));

    InitWindow(screenWidth, screenHeight, title);

    SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144);         // Set our game to run at 144 frames-per-second

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        DrawFrame();
        UpdatePlayerPosition(&Player, screenWidth);
        UpdatePlayerBullet(PlayerBullets, &Player);
        UpdateInvader(Invaders, PlayerBullets);
        UpdateEnemyBullet(Invaders);
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
    else if(Player.helath <= 0) // Lose
    {
        DrawText("GAME OVER", screenWidth / 2 - 300, screenHeight / 2 - 100, 100, RED);
    }
    else if(invaderCount <= 0) // Win
    {
        DrawText("YOU WIN", screenWidth / 2 - 200, screenHeight / 2 - 100, 100, GREEN);
    }

    EndDrawing();
}

void UpdatePlayerPosition(player *Player, const int screenWidth)
{
    if (IsKeyDown(KEY_RIGHT) && Player->pos_x < screenWidth - (Player->width + 10))
    {
        Player->pos_x += PlayerSpeed;
    }

    if (IsKeyDown(KEY_LEFT) && Player->pos_x > 10)
    {
        Player->pos_x -= PlayerSpeed;
    }
}

void UpdatePlayerBullet(bullet PlayerBullets[], player *Player)
{
    // Initializing bullet on space press
    if (IsKeyPressed(KEY_SPACE))
    {
        int i = 0;
        while (PlayerBullets[i].state == 1 && i < PlayerMagazineSize)
        {
            i++;
        }

        PlayerBullets[i].pos_x = Player->pos_x + (Player->width / 2) - (PlayerBullets[i].width / 2);
        PlayerBullets[i].pos_y = Player->pos_y - Player->height - 5;
        PlayerBullets[i].state = 1;
    }
    // Checking if bullet is out of frame or updating bullet position
    for (int k = 0; k < PlayerMagazineSize; k++)
    {
        if (PlayerBullets[k].state == 1 && PlayerBullets[k].pos_y < 0 - PlayerBullets[k].height)
        {
            PlayerBullets[k].state = 0;
        }

        if (PlayerBullets[k].state == 1)
        {
            PlayerBullets[k].pos_y -= BulletSpeed;
        }
    }
}

void UpdateInvader(invader *Invaders, bullet *PlayerBullets)
{
    for (int i = 0; i < PlayerMagazineSize; i++)
    {
        for (int j = 0; j < InvadersPerRow * Rows; j++)
        {
            // Check only if both bullets and invader are active
            if (Invaders[j].state == 1 && PlayerBullets[i].state == 1)
            {
                // Check if x coordinates match
                if (PlayerBullets[i].pos_x >= Invaders[j].pos_x - PlayerBullets[i].width && PlayerBullets[i].pos_x <= Invaders[j].pos_x + Invaders[j].width)
                {
                    // Check if y coordinates match
                    if (PlayerBullets[i].pos_y >= Invaders[j].pos_y && PlayerBullets[i].pos_y <= Invaders[j].pos_y + Invaders[j].height)
                    {
                        invaderCount--;
                        Invaders[j].state = 0;
                        PlayerBullets[i].state = 0;
                    }
                }
            }
        }
    }
}

void UpdateEnemyBullet(invader *Invaders)
{
    int randomInvaderIndex = GetRandomValue(0, (Rows * InvadersPerRow) - 1);

    double elapsedTime = GetTime();

    // Initializing bullet every 1 second
    if (elapsedTime - lastTimeCheck >= BulletSpawnRate && Invaders[randomInvaderIndex].state == 1)
    {
        lastTimeCheck = elapsedTime;

        int i = 0;
        while (EnemyBullets[i].state == 1 && i < EnemyMagazineSize)
        {
            i++;
        }

        EnemyBullets[i].pos_x = Invaders[randomInvaderIndex].pos_x + (InvaderWidth / 2) - (EnemyBullets[i].width / 2);
        EnemyBullets[i].pos_y = (Invaders[randomInvaderIndex].pos_y + InvaderHeight);
        EnemyBullets[i].state = 1;
    }
    // Checking if bullet is out of frame or updating bullet position
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

void CheckEnemyBulletPlayerCollisions(void)
{
    for (int i = 0; i < EnemyMagazineSize; i++)
    {
        // Check only if both bullets and invader are active
        if (EnemyBullets[i].state == 1)
        {
            // Check if x coordinates match
            if (EnemyBullets[i].pos_x >= Player.pos_x - EnemyBullets[i].width && EnemyBullets[i].pos_x <= Player.pos_x + Player.width)
            {
                // Check if y coordinates match
                if (EnemyBullets[i].pos_y + EnemyBullets[i].height >= Player.pos_y && EnemyBullets[i].pos_y + EnemyBullets[i].height <= Player.pos_y + Player.height)
                {
                    EnemyBullets[i].state = 0;
                    Player.helath -= 20;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------