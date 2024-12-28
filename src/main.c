/********************************************************************************************
 *
 *   To compile, just press F5.
 *
 ********************************************************************************************/

// Includes ------------------------------------------------------------------------
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "player.h"
#include "bullet.h"
#include "invader.h"
// ---------------------------------------------------------------------------------

// Defines--------------------------------------------------------------------------
#define NEARBLACK (Color){26, 26, 26, 255}
//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
// Screen
const int screenWidth = 960;
const int screenHeight = 960;
const char *title = "Space Invaders";

// Player
player Player = (player){PlayerHealth, 30, screenHeight - 60, PlayerWidth, PlayerHeight};

// Bullets
bullet PlayerBullets[PlayerMagazineSize];
bullet EnemyBullets[EnemyMagazineSize];
double lastTimeBulletCheck = 0.0;

// Invaders
invader Invaders[InvadersPerRow * Rows];
const int InvaderCount = InvadersPerRow * Rows;
const int BorderDistance = PlayerWidth / 2;

double lastTimeMovementCheck = 0.0;
bool shouldMoveHorizontally = 1;
bool shouldMoveVertically = 0;
int remainingInvaders = InvadersPerRow * Rows;

int invaderToMove = InvaderCount - 1;
int movementDistanceX = MovementDistance;
int movementDistanceY = MovementDistance;
double movementInterval = MovementInterval;

// Win/Lose conditiones
bool winCondition = false;
bool loseCondition = false;
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------
void DrawFrame();
void UpdatePlayerPosition();
void UpdatePlayerBullet();
void UpdateInvader();
void UpdateInvaderBullet();
void CheckEnemyBulletPlayerCollisions();
void checkConditions();
//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    invader_init(Invaders, screenWidth);
    bullet_init(PlayerBullets, PlayerMagazineSize);
    bullet_init(EnemyBullets, EnemyMagazineSize);

    SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144);         // Set our game to run at 144 frames-per-second

    InitWindow(screenWidth, screenHeight, title);

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        checkConditions();
        if (winCondition == false && loseCondition == false)
        {
            UpdatePlayerPosition();
            UpdatePlayerBullet();
            UpdateInvader();
            UpdateInvaderBullet();
            CheckEnemyBulletPlayerCollisions();
            DrawFrame();
        }
        else
        {
            TraceLog(LOG_INFO, "Conditions lose: %d win: %d", loseCondition, winCondition);
            BeginDrawing();
            ClearBackground(NEARBLACK);
            if (winCondition)
                DrawText("YOU WIN", screenWidth / 2 - 200, screenHeight / 2 - 100, 100, GREEN);
            if (loseCondition)
                DrawText("GAME OVER", screenWidth / 2 - 300, screenHeight / 2 - 100, 100, RED);
            EndDrawing();
        }
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}

// Local Functions Implementation------------------------------------------------------
void DrawFrame()
{
    BeginDrawing();

    // Drawing background
    ClearBackground(NEARBLACK);

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
                        remainingInvaders--;
                        Invaders[j].state = 0;
                        PlayerBullets[i].state = 0;
                        // TraceLog(LOG_INFO, "Remaining Invaders: %d", remainingInvaders);
                    }
                }
            }
        }
    }

    // Moving each Invader every MovementInterval seconds
    double elapsedTime = GetTime();
    if (elapsedTime - lastTimeMovementCheck >= MovementInterval)
    {
        TraceLog(LOG_INFO, "Test: %lf", movementInterval);
        if (shouldMoveVertically)
        {
            if (invaderToMove > 0)
            {
                moveInvadersVertical(Invaders, invaderToMove, 2 * abs(movementDistanceY));
                invaderToMove--;
            }
            else
            {
                shouldMoveVertically = 0;
                moveInvadersVertical(Invaders, invaderToMove, 2 * abs(movementDistanceY));
                shouldMoveVertically = 0;
                invaderToMove = InvaderCount - 1;
                if (movementInterval >= 0.4)
                    movementInterval -= 0.04;
            }
        }
        else
        {
            if (invaderToMove > 0)
            {
                moveInvadersHorizontal(Invaders, invaderToMove, movementDistanceX);
                invaderToMove--;
            }
            else if (invaderToMove == 0)
            {
                // If we're moving them right
                if (movementDistanceX > 0)
                {
                    // If there is space to move them
                    if ((Invaders[InvadersPerRow * Rows - 1].pos_x + ((screenWidth / Columns) / 3) + movementDistanceX) <= (screenWidth - BorderDistance))
                    {
                        moveInvadersHorizontal(Invaders, invaderToMove, movementDistanceX);
                        invaderToMove = InvaderCount - 1;
                    }
                    else
                    {
                        moveInvadersHorizontal(Invaders, invaderToMove, movementDistanceX);
                        movementDistanceX = -movementDistanceX;
                        shouldMoveVertically = 1;
                        invaderToMove = InvaderCount - 1;
                    }
                }
                // If we're moving them left
                else
                {
                    // If there is space to move them
                    if ((Invaders[0].pos_x + movementDistanceX) >= BorderDistance)
                    {
                        moveInvadersHorizontal(Invaders, invaderToMove, movementDistanceX);
                        invaderToMove = InvaderCount - 1;
                    }
                    else
                    {
                        moveInvadersHorizontal(Invaders, invaderToMove, movementDistanceX);
                        movementDistanceX = -movementDistanceX;
                        shouldMoveVertically = 1;
                        invaderToMove = InvaderCount - 1;
                    }
                }
            }
        }
    }
}

void UpdateInvaderBullet()
{
    // Picking random invader to shoot a bullet
    int randomInvaderIndex = GetRandomValue(0, (Rows * InvadersPerRow) - 1);
    while (Invaders[randomInvaderIndex].state == 0 && remainingInvaders > 0)
        randomInvaderIndex = GetRandomValue(0, (Rows * InvadersPerRow) - 1);

    // Initializing bullet every "BulletSpawnRate" seconds
    double elapsedTime = GetTime();
    if (elapsedTime - lastTimeBulletCheck >= BulletSpawnRate)
    {
        lastTimeBulletCheck = elapsedTime;

        // Looking for the first free bullet slot
        int i = 0;
        while (EnemyBullets[i].state == 1 && i < EnemyMagazineSize)
        {
            i++;
        }

        // Setting the bullet position to the middle of the invader rectangle and it's state to 1
        EnemyBullets[i].pos_x = Invaders[randomInvaderIndex].pos_x + (Invaders[0].height / 2) - (EnemyBullets[i].width / 2);
        EnemyBullets[i].pos_y = (Invaders[randomInvaderIndex].pos_y + Invaders[0].height);
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

void checkConditions()
{
    // TraceLog(LOG_INFO, "LOwest invader: %d", lowestInvaderY(Invaders));
    if (remainingInvaders == 0)
    {
        winCondition = true;
    }
    else
    {
        if (Player.helath <= 0 || lowestInvaderY(Invaders) + 60 >= Player.pos_y)
        {
            loseCondition = true;
        }
    }
}
//----------------------------------------------------------------------------------