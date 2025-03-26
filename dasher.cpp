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

    const int sizeOfObstacles{10};
    AnimationData obstaclesData[sizeOfObstacles]{};
    for (int i = 0; i < sizeOfObstacles; i++)
    {
        obstaclesData[i].rectangle.x = 0.0;
        obstaclesData[i].rectangle.y = 0.0;
        obstaclesData[i].rectangle.width = obstacle.width / 8;
        obstaclesData[i].rectangle.height = obstacle.height / 8;
        obstaclesData[i].position.x = windowDimensions[0] + 300 * i;
        obstaclesData[i].position.y = windowDimensions[1] - obstacle.height / 8;
        obstaclesData[i].frame = 0.0;
        obstaclesData[i].runningTime = 0.0;
        obstaclesData[i].updateTime = 1.0f / 12.0f;
    }

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

        for (int i = 0; i < sizeOfObstacles; i++)
        {
            // Obstacle animation
            obstaclesData[i].runningTime += deltaTime;
            if (obstaclesData[i].runningTime >= obstaclesData[i].updateTime)
            {
                obstaclesData[i].runningTime = 0;

                // Update animation frame
                obstaclesData[i].rectangle.x = obstaclesData[i].frame * obstaclesData[i].rectangle.width;
                obstaclesData[i].frame += obstaclesData[i].loop;
                if (obstaclesData[i].frame >= 7 || obstaclesData[i].frame <= 0)
                {
                    obstaclesData[i].loop *= -1;
                }
            }
            
            // Draw obstacle
            DrawTextureRec(obstacle, obstaclesData[i].rectangle, obstaclesData[i].position, WHITE);

            // Update obstacles position
            obstaclesData[i].position.x += obstacleVelocity * deltaTime;
        }

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

        // Update hero position
        heroData.position.y += heroVelocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    CloseWindow();
}