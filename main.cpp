#include <raylib.h>
#include <raymath.h>

struct Boid
{
    Vector2 position;
    Vector2 size;
    Vector2 velocity;

    float maxSpeed = 200.0;
    float minSpeed = -200.0;

    float separation = 10.0;
    float alignment = 10.0;
    float cohesion = 10.0;

    float visionRadius = 70.0;
    float protectedRadius = 20.0;

    float edgeMargin = 50.0;
    float edgeTurnSpeed = 100.0;
};

int main()
{
    InitWindow(800, 600, "Boids Improved!");

    Boid boids[50];

    // Init boids
    for (auto &boid : boids)
    {
        boid.size = (Vector2){10, 10};
        boid.position = (Vector2){
            (float)GetRandomValue(0, GetScreenWidth() - boid.size.x),
            (float)GetRandomValue(0, GetScreenHeight() - boid.size.y)
        };

        boid.velocity = (Vector2){(float)GetRandomValue(-400, 400), (float)GetRandomValue(-400, 400)};
    }

    SetTargetFPS(60);

    while (WindowShouldClose() == false) 
    {
        // Update
        for (auto &boid : boids)
        {
//            boid.position = Vector2Add(boid.position, boid.velocity);
            boid.position.x += boid.velocity.x * GetFrameTime();
            boid.position.y += boid.velocity.y * GetFrameTime();
            
            if (boid.position.x < boid.edgeMargin)
                boid.velocity.x += boid.edgeTurnSpeed;
            if (boid.position.x > GetScreenWidth() - boid.edgeMargin)
                boid.velocity.x -= boid.edgeTurnSpeed;
            if (boid.position.y < boid.edgeMargin)
                boid.velocity.y += boid.edgeTurnSpeed;
            if (boid.position.y > GetScreenHeight() - boid.edgeMargin)
                boid.velocity.y -= boid.edgeTurnSpeed;

            boid.velocity = Vector2Clamp(boid.velocity, (Vector2){boid.minSpeed, boid.minSpeed}, (Vector2){boid.maxSpeed, boid.maxSpeed});

            
        }
    
        // Draw
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for (auto &boid : boids)
        {
            DrawRectangle(
                    boid.position.x, 
                    boid.position.y, 
                    boid.size.x, 
                    boid.size.y,
                    RED
                );
        }
        
        EndDrawing();
    }

    CloseWindow();


    return 0;
}
