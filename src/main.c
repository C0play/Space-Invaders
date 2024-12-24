/*******************************************************************************************
 *   To compile, just press F5.
 ********************************************************************************************/
#include "raylib.h"


// Defines--------------------------------------------------------------------------

//----------------------------------------------------------------------------------

// Local Variables Definition (local to this module)--------------------------------
const int screenWidth = 640;
const int screenHeight = 640;
const char *title = "Test";
//----------------------------------------------------------------------------------

// Local Functions Declaration------------------------------------------------------

//----------------------------------------------------------------------------------

// Main entry point--------------------------------------------------------------------------
int main()
{
    // --------------------------------------- START ----------------------------------------
    InitWindow(screenWidth, screenHeight, title);

    SetTraceLogLevel(LOG_ALL); // For debugging purposes
    SetTargetFPS(144); // Set our game to run at 144 frames-per-second

    // Main game loop------------------------------------------------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
    }

    CloseWindow(); // Close window and OpenGL context
    // ----------------------------------------END-------------------------------------------

    return 0;
}