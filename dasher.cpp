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

    const int sizeOfObstacles{2};
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

    float finishLine{obstaclesData[sizeOfObstacles - 1].position.x};
    bool collision{};
    bool isInAir{false};

    const int jumpVelocity{1'000}; // pixels / second

    Texture2D background = LoadTexture("textures/background.png");
    Texture2D midground = LoadTexture("textures/midground.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float backgroundX{};
    float midgroundX{};
    float foregroundX{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        const float deltaTime{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll the background textures
        backgroundX -= 20 * deltaTime;
        if (backgroundX <= -background.width * 2)
        {
            backgroundX = 0.0;
        }

        midgroundX -= 40 * deltaTime;
        if (midgroundX <= -midground.width * 2)
        {
            midgroundX = 0.0;
        }

        foregroundX -= 80 * deltaTime;
        if (foregroundX <= -foreground.width * 2)
        {
            foregroundX = 0.0;
        }

        // Draw the background
        Vector2 background1Position{backgroundX, 0.0};
        DrawTextureEx(background, background1Position, 0.0, 2.0, WHITE);
        Vector2 background2Position{backgroundX + background.width * 2, 0.0};
        DrawTextureEx(background, background2Position, 0.0, 2.0, WHITE);

        Vector2 midground1Position{midgroundX, 0.0};
        DrawTextureEx(midground, midground1Position, 0.0, 2.0, WHITE);
        Vector2 midground2Position{midgroundX + midground.width * 2, 0.0};
        DrawTextureEx(midground, midground2Position, 0.0, 2.0, WHITE);

        Vector2 foreground1Position{foregroundX, 0.0};
        DrawTextureEx(foreground, foreground1Position, 0.0, 2.0, WHITE);
        Vector2 foreground2Position{foregroundX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, foreground2Position, 0.0, 2.0, WHITE);

        // Obstacles animations
        for (int i = 0; i < sizeOfObstacles; i++)
        {
            obstaclesData[i] = updateAnimationData(obstaclesData[i], deltaTime, obstacleMaxFrame);

            obstaclesData[i].position.x += obstacleVelocity * deltaTime;
        }

        for (AnimationData obstacleData : obstaclesData)
        {
            float padding{50.0f};
            Rectangle obstacleRectangle{
                obstacleData.position.x + padding,
                obstacleData.position.y + padding,
                obstacleData.rectangle.width - 2 * padding,
                obstacleData.rectangle.height - 2 * padding};
            Rectangle heroRectangle{
                heroData.position.x,
                heroData.position.y,
                heroData.rectangle.width,
                heroData.rectangle.height
            };

            if(CheckCollisionRecs(obstacleRectangle, heroRectangle)){
                collision = true;
            }
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

        // Game state
        if(collision){
            DrawText("Game Over!", windowDimensions[0]/2 - 160, windowDimensions[1]/2 - 32, 64, RED);
        }
        else if (heroData.position.x >= finishLine)
        {         
            DrawText("You Win!", windowDimensions[0]/2 - 128, windowDimensions[1]/2 - 32, 64, GREEN);
        }        
        else{
            DrawTextureRec(hero, heroData.rectangle, heroData.position, WHITE);

            for (int i = 0; i < sizeOfObstacles; i++)
            {            
                DrawTextureRec(obstacle, obstaclesData[i].rectangle, obstaclesData[i].position, WHITE);
            }       
        }

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

        finishLine += obstacleVelocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    UnloadTexture(obstacle);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}