#include "raylib.h";

int main()
{
    // Window dimensions
    const int windowWidth{1280};
    const int windowHeight{720};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    const int gravity{2'000}; // (pixels / second) / second

    Texture2D hero = LoadTexture("textures/hero.png");
    Rectangle heroRectangle;
    heroRectangle.width = hero.width/6;
    heroRectangle.height = hero.height;
    heroRectangle.x = 0;
    heroRectangle.y = 0;
    Vector2 heroPosition;
    heroPosition.x = windowWidth/2 - heroRectangle.width/2;
    heroPosition.y = windowHeight - heroRectangle.height;
    int heroAnimationFrame{0};

    int velocity{0};
    bool isInAir{false};
    float runningTime{0};
    
    const float updateTime{1.0f / 12.0f};
    const int jumpVelocity{1'000}; // pixels / second

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float deltaTime{ GetFrameTime() };

        runningTime += deltaTime;

        // Update animation frame
        if(runningTime >= updateTime){
            runningTime = 0;

            heroRectangle.x = heroAnimationFrame * heroRectangle.width;
            heroAnimationFrame++;
            if(heroAnimationFrame > 5){
                heroAnimationFrame = 0;
            }
        }
        

        DrawTextureRec(hero, heroRectangle, heroPosition, WHITE );

        if (heroPosition.y >= windowHeight - heroRectangle.height)
        {
            // Rectangle on the ground
            isInAir = false;

            velocity = 0;
        }
        else
        {
            // Rectangle in the air
            isInAir = true;

            velocity += gravity * deltaTime;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity -= jumpVelocity;
        }

        heroPosition.y += velocity * deltaTime;

        EndDrawing();
    }
    UnloadTexture(hero);
    CloseWindow();
}