#include "raylib.h";

int main()
{
    // Window dimensions
    const int windowWidth{1280};
    const int windowHeight{720};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    const int gravity{2'000}; // (pixels / second) / second

    // Obstacle parameters
    Texture2D obstacle = LoadTexture("textures/obstacle.png");
    Rectangle obstacleRectangle{0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8};
    Vector2 obstaclePosition{windowWidth, windowHeight - obstacleRectangle.height};
    int obstacleVelocity{-200};
    int obstacleAnimationFrame{0};
    int obstacleAnimationLoop{1};
    float obstacleRunningTime{0};
    const float obstacleUpdateTime{1.0f / 12.0f};

    
    Rectangle obstacle2Rectangle{0.0f, 0.0f, obstacle.width / 8, obstacle.height / 8};
    Vector2 obstacle2Position{windowWidth + 300, windowHeight - obstacleRectangle.height};
    int obstacle2AnimationFrame{0};
    int obstacle2AnimationLoop{1};
    float obstacle2RunningTime{0};
    const float obstacle2UpdateTime{1.0f / 16.0f};

    // Hero parameters
    Texture2D hero = LoadTexture("textures/hero.png");
    Rectangle heroRectangle;
    heroRectangle.width = hero.width / 6;
    heroRectangle.height = hero.height;
    heroRectangle.x = 0;
    heroRectangle.y = 0;
    Vector2 heroPosition;
    heroPosition.x = windowWidth / 2 - heroRectangle.width / 2;
    heroPosition.y = windowHeight - heroRectangle.height;
    int heroVelocity{0};
    int heroAnimationFrame{0};
    float heroRunningTime{0};
    const float heroUpdateTime{1.0f / 12.0f};

    bool isInAir{false};

    const int jumpVelocity{1'000}; // pixels / second

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float deltaTime{GetFrameTime()};

        //Obstacle animation
        obstacleRunningTime += deltaTime;
        if (obstacleRunningTime >= obstacleUpdateTime)
        {
            obstacleRunningTime = 0;

            // Update animation frame
            obstacleRectangle.x = obstacleAnimationFrame * obstacleRectangle.width;
            obstacleAnimationFrame += obstacleAnimationLoop;
            if (obstacleAnimationFrame >= 7 || obstacleAnimationFrame <= 0)
            {
                obstacleAnimationLoop *= -1;
            }
        }

        //Obstacle 2 animation
        obstacle2RunningTime += deltaTime;
        if (obstacle2RunningTime >= obstacle2UpdateTime)
        {
            obstacle2RunningTime = 0;

            // Update animation frame
            obstacle2Rectangle.x = obstacle2AnimationFrame * obstacle2Rectangle.width;
            obstacle2AnimationFrame += obstacle2AnimationLoop;
            if (obstacle2AnimationFrame >= 7 || obstacle2AnimationFrame <= 0)
            {
                obstacle2AnimationLoop *= -1;
            }
        }

        DrawTextureRec(obstacle, obstacleRectangle, obstaclePosition, WHITE);        
        DrawTextureRec(obstacle, obstacle2Rectangle, obstacle2Position, RED);

        // Hero animation
        if (!isInAir)
        {
            heroRunningTime += deltaTime;
            if (heroRunningTime >= heroUpdateTime)
            {
                heroRunningTime = 0;

                // Update animation frame
                heroRectangle.x = heroAnimationFrame * heroRectangle.width;
                heroAnimationFrame++;
                if (heroAnimationFrame > 5)
                {
                    heroAnimationFrame = 0;
                }
            }
        }
        else
        {
            heroRectangle.x = 0 * heroRectangle.width;
        }

        DrawTextureRec(hero, heroRectangle, heroPosition, WHITE);

        // Hero movement
        if (heroPosition.y >= windowHeight - heroRectangle.height)
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
        obstaclePosition.x += obstacleVelocity * deltaTime;        
        // Update obstacle 2 position
        obstacle2Position.x += obstacleVelocity * deltaTime;

        // Update hero position
        heroPosition.y += heroVelocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    CloseWindow();
}