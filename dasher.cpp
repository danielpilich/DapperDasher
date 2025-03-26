#include "raylib.h";

struct AnimationData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

int main()
{
    // Window dimensions
    const int windowWidth{1280};
    const int windowHeight{720};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    const int gravity{2'000}; // (pixels / second) / second

    // Obstacle parameters
    Texture2D obstacle = LoadTexture("textures/obstacle.png");
    int obstacleVelocity{-200};

    AnimationData obstacleData{
        {0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8}, // Rectangle rectangle
        {windowWidth, windowHeight - obstacle.height / 8},     // Vector2 position
        0,                                                     // int frame
        1.0f / 12.0f,                                          // float updateTime
        0.0f                                                   // float runningTime
    };
    int obstacleAnimationLoop{1};

    AnimationData obstacle2Data{
        {0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8},   // Rectangle rectangle
        {windowWidth + 300, windowHeight - obstacle.height / 8}, // Vector2 position
        0,                                                       // int frame
        1.0f / 16.0f,                                            // float updateTime
        0.0f                                                     // float runningTime
    };
    int obstacle2AnimationLoop{1};

    // Hero parameters
    Texture2D hero = LoadTexture("textures/hero.png");
    AnimationData heroData;
    heroData.rectangle.width = hero.width / 6;
    heroData.rectangle.height = hero.height;
    heroData.rectangle.x = 0;
    heroData.rectangle.y = 0;
    heroData.position.x = windowWidth / 2 - heroData.rectangle.width / 2;
    heroData.position.y = windowHeight - heroData.rectangle.height;
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
        obstacleData.runningTime += deltaTime;
        if (obstacleData.runningTime >= obstacleData.updateTime)
        {
            obstacleData.runningTime = 0;

            // Update animation frame
            obstacleData.rectangle.x = obstacleData.frame * obstacleData.rectangle.width;
            obstacleData.frame += obstacleAnimationLoop;
            if (obstacleData.frame >= 7 || obstacleData.frame <= 0)
            {
                obstacleAnimationLoop *= -1;
            }
        }

        // Obstacle 2 animation
        obstacle2Data.runningTime += deltaTime;
        if (obstacle2Data.runningTime >= obstacle2Data.updateTime)
        {
            obstacle2Data.runningTime = 0;

            // Update animation frame
            obstacle2Data.rectangle.x = obstacle2Data.frame * obstacle2Data.rectangle.width;
            obstacle2Data.frame += obstacle2AnimationLoop;
            if (obstacle2Data.frame >= 7 || obstacle2Data.frame <= 0)
            {
                obstacle2AnimationLoop *= -1;
            }
        }

        DrawTextureRec(obstacle, obstacleData.rectangle, obstacleData.position, WHITE);
        DrawTextureRec(obstacle, obstacle2Data.rectangle, obstacle2Data.position, RED);

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
        if (heroData.position.y >= windowHeight - heroData.rectangle.height)
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
        obstacleData.position.x += obstacleVelocity * deltaTime;
        // Update obstacle 2 position
        obstacle2Data.position.x += obstacleVelocity * deltaTime;

        // Update hero position
        heroData.position.y += heroVelocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    CloseWindow();
}