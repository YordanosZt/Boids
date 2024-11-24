#include <raylib.h>

struct Boids
{
    Vector2 position;
    Vector2 size;
    Vector2 velocity;

    float separation;
    float alignment;
    float cohesion;

    float visionRadius = 70.0;
    float protectedRadius = 20.0;

    float edgeMargin = 50.0;
};

int main()
{
    InitWindow(800, 600, "Boids Improved!");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) 
    {
        // Update
    
        // Draw
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}
