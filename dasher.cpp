#include "raylib.h";

struct AnimationData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
    int loop = 1;
};

int main()
{
    // Window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    const int gravity{2'000}; // (pixels / second) / second

    // Obstacle parameters
    Texture2D obstacle = LoadTexture("textures/obstacle.png");
    int obstacleVelocity{-200};

    AnimationData obstaclesData[2]{
        // Obstacle 1
        {
            {0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8},            // Rectangle rectangle
            {windowDimensions[0], windowDimensions[1] - obstacle.height / 8}, // Vector2 position
            0,                                                                // int frame
            1.0f / 12.0f,                                                     // float updateTime
            0.0f                                                              // float runningTime
        },
        // Obstacle 2
        {
            {0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8},                  // Rectangle rectangle
            {windowDimensions[0] + 300, windowDimensions[1] - obstacle.height / 8}, // Vector2 position
            0,                                                                      // int frame
            1.0f / 16.0f,                                                           // float updateTime
            0.0f                                                                    // float runningTime
        }};

    // Hero parameters
    Texture2D hero = LoadTexture("textures/hero.png");
    AnimationData heroData;
    heroData.rectangle.width = hero.width / 6;
    heroData.rectangle.height = hero.height;
    heroData.rectangle.x = 0;
    heroData.rectangle.y = 0;
    heroData.position.x = windowDimensions[0] / 2 - heroData.rectangle.width / 2;
    heroData.position.y = windowDimensions[1] - heroData.rectangle.height;
    heroData.frame = 0;
    heroData.updateTime = 1.0f / 12.0f;
    heroData.runningTime = 0.0f;
    int heroVelocity{0};

    bool isInAir{false};

    const int jumpVelocity{1'000}; // pixels / second

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float deltaTime{GetFrameTime()};

        // Obstacle animation
        obstaclesData[0].runningTime += deltaTime;
        if (obstaclesData[0].runningTime >= obstaclesData[0].updateTime)
        {
            obstaclesData[0].runningTime = 0;

            // Update animation frame
            obstaclesData[0].rectangle.x = obstaclesData[0].frame * obstaclesData[0].rectangle.width;
            obstaclesData[0].frame += obstaclesData[0].loop;
            if (obstaclesData[0].frame >= 7 || obstaclesData[0].frame <= 0)
            {
                obstaclesData[0].loop *= -1;
            }
        }

        // Obstacle 2 animation
        obstaclesData[1].runningTime += deltaTime;
        if (obstaclesData[1].runningTime >= obstaclesData[1].updateTime)
        {
            obstaclesData[1].runningTime = 0;

            // Update animation frame
            obstaclesData[1].rectangle.x = obstaclesData[1].frame * obstaclesData[1].rectangle.width;
            obstaclesData[1].frame += obstaclesData[1].loop;
            if (obstaclesData[1].frame >= 7 || obstaclesData[1].frame <= 0)
            {
                obstaclesData[1].loop *= -1;
            }
        }

        DrawTextureRec(obstacle, obstaclesData[0].rectangle, obstaclesData[0].position, WHITE);
        DrawTextureRec(obstacle, obstaclesData[1].rectangle, obstaclesData[1].position, RED);

        // Hero animation
        if (!isInAir)
        {
            heroData.runningTime += deltaTime;
            if (heroData.runningTime >= heroData.updateTime)
            {
                heroData.runningTime = 0;

                // Update animation frame
                heroData.rectangle.x = heroData.frame * heroData.rectangle.width;
                heroData.frame++;
                if (heroData.frame > 5)
                {
                    heroData.frame = 0;
                }
            }
        }
        else
        {
            heroData.rectangle.x = 0 * heroData.rectangle.width;
        }

        DrawTextureRec(hero, heroData.rectangle, heroData.position, WHITE);

        // Hero movement
        if (heroData.position.y >= windowDimensions[1] - heroData.rectangle.height)
        {
            // Rectangle on the ground
            isInAir = false;

            heroVelocity = 0;
        }
        else
        {
            // Rectangle in the air
            isInAir = true;

            heroVelocity += gravity * deltaTime;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            heroVelocity -= jumpVelocity;
        }

        // Update obstacle position
        obstaclesData[0].position.x += obstacleVelocity * deltaTime;
        // Update obstacle 2 position
        obstaclesData[1].position.x += obstacleVelocity * deltaTime;

        // Update hero position
        heroData.position.y += heroVelocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    CloseWindow();
}