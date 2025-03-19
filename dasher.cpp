#include "raylib.h";

int main()
{
    // Window dimensions
    const int windowWidth{1280};
    const int windowHeight{720};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    const int gravity{1};

    // Rectangle dimensions and properties
    const int rectangleWidth{50};
    const int rectangleHeight{80};

    int rectangleX{windowWidth / 2 - rectangleWidth};
    int rectangleY{windowHeight - rectangleHeight};
    int rectangleVelocity{0};

    bool isInAir{false};

    const int jumpVelocity{22};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawRectangle(rectangleX, rectangleY, rectangleWidth, rectangleHeight, BLUE);

        if (rectangleY >= windowHeight - rectangleHeight)
        {
            // Rectangle on the ground
            isInAir = false;

            rectangleVelocity = 0;
        }
        else
        {
            // Rectangle in the air
            isInAir = true;

            rectangleVelocity += gravity;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            rectangleVelocity -= jumpVelocity;
        }

        rectangleY += rectangleVelocity;

        EndDrawing();
    }
    CloseWindow();
}