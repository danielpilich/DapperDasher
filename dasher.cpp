#include "raylib.h";

int main()
{
    // Window dimensions
    const int windowWidth{1280};
    const int windowHeight{720};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        EndDrawing();
    }

    CloseWindow();
}