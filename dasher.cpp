#include "raylib.h";

struct AnimationData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimationData data, int windowsHeight)
{
    return data.position.y >= windowsHeight - data.rectangle.height;
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;

        // Update animation frame
        data.rectangle.x = data.frame * data.rectangle.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

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
    const int obstacleMaxFrame{7};
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
        obstaclesData[i].updateTime = 1.0f / 16.0f;
        obstaclesData[i].runningTime = 0.0;
    }

    // Hero parameters
    Texture2D hero = LoadTexture("textures/hero.png");
    const int heroMaxFrame{5};
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
            obstaclesData[i] = updateAnimationData(obstaclesData[i], deltaTime, obstacleMaxFrame);

            // Draw obstacle
            DrawTextureRec(obstacle, obstaclesData[i].rectangle, obstaclesData[i].position, WHITE);

            // Update obstacles position
            obstaclesData[i].position.x += obstacleVelocity * deltaTime;
        }

        // Hero animation
        if (!isInAir)
        {
            heroData = updateAnimationData(heroData, deltaTime, heroMaxFrame);
        }
        else
        {
            heroData.rectangle.x = 0 * heroData.rectangle.width;
        }

        DrawTextureRec(hero, heroData.rectangle, heroData.position, WHITE);

        // Hero movement
        if (isOnGround(heroData, windowDimensions[1]))
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