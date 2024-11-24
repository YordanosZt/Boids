#include <cmath>
#include <raylib.h>
#include <raymath.h>

struct Boid
{
    int index;

    Vector2 position;
    Vector2 size;
    Vector2 velocity;

    float maxSpeed = 8.0;
    float minSpeed = 5.0;

    float separation = 0.5;
    float alignment = 0.2;
    float cohesion = 0.0005;

    float visionRadius = 50.0;
    float protectedRadius = 10.0;

    float edgeMargin = 100.0;
    float edgeTurnSpeed = 0.5;
};

int main()
{
    InitWindow(1000, 600, "Boids Improved!");

    Boid boids[300];

    bool debugLines = false;
    bool avoidWalls = true;

    // Init boids
    int index = 0;

    for (auto &boid : boids)
    {
        boid.size = (Vector2){5, 5};
        boid.position = (Vector2){
            (float)GetRandomValue(0, GetScreenWidth() - boid.size.x),
            (float)GetRandomValue(0, GetScreenHeight() - boid.size.y)
        };

        //boid.velocity = (Vector2){(float)GetRandomValue(-400, 400), (float)GetRandomValue(-400, 400)};
        boid.index = index;

        index++;
    }

    SetTargetFPS(60);

    while (WindowShouldClose() == false) 
    {
        // Update
        for (auto &boid : boids)
        { 
            // Separation
            Vector2 close = (Vector2){0, 0};
           
            // Alignment
            Vector2 avgVel = (Vector2){0, 0};
            Vector2 totalVel = (Vector2){0, 0};

            // Cohesion
            Vector2 centerOfMass = (Vector2){0, 0};
            Vector2 totalMass = (Vector2){0, 0};

            int intersectingBoids = 0;

            // Vision Radius
            for (auto &other : boids)
            {
                if (CheckCollisionCircleRec(
                            boid.position, 
                            boid.visionRadius, 
                            {other.position.x, other.position.y, other.size.x, other.size.y}
                        ))
                {
                    // Separation
                    Vector2 dxn = Vector2Subtract(boid.position, other.position);
                    float dist = Vector2Distance(boid.position, other.position);
                    
                    if (dist < boid.protectedRadius)
                    {
                        close = Vector2Add(close, dxn);
                    }

                    if (debugLines)
                    {
                        if (boid.index == 20)
                        {
                            DrawLine(
                                    boid.position.x, 
                                    boid.position.y, 
                                    boid.position.x - dxn.x, 
                                    boid.position.y - dxn.y, 
                                    RED
                                );
                        }
                    }


                    // Alignment
                    intersectingBoids += 1;
                    totalVel = Vector2Add(totalVel, other.velocity);
                    
                    // Cohesion
                    totalMass = Vector2Add(totalMass, other.position);
                }
            }

            // Separation
            boid.velocity = Vector2Add(boid.velocity, close);

            // Alignment
            if (intersectingBoids > 0)
                avgVel = (Vector2){totalVel.x / intersectingBoids, totalVel.y / intersectingBoids};

            boid.velocity = Vector2Add(
                        boid.velocity, 
                        (Vector2){
                            (avgVel.x - boid.velocity.x) * boid.alignment, 
                            (avgVel.y - boid.velocity.y) * boid.alignment
                        }
                    );

            // Cohesion
            if (intersectingBoids > 0)
                centerOfMass = (Vector2){totalMass.x / intersectingBoids, totalMass.y / intersectingBoids};

            boid.velocity = Vector2Add(
                    boid.velocity, 
                    (Vector2){
                        (centerOfMass.x - boid.position.x) * boid.cohesion, 
                        (centerOfMass.y - boid.position.y) * boid.cohesion
                    }
                );

            // Wall Avoidance
            if (avoidWalls)
            {
                if (boid.position.x < boid.edgeMargin)
                    boid.velocity.x += boid.edgeTurnSpeed;
                if (boid.position.x > GetScreenWidth() - boid.edgeMargin)
                    boid.velocity.x -= boid.edgeTurnSpeed;
                if (boid.position.y < boid.edgeMargin)
                    boid.velocity.y += boid.edgeTurnSpeed;
                if (boid.position.y > GetScreenHeight() - boid.edgeMargin)
                    boid.velocity.y -= boid.edgeTurnSpeed;
            }
            else 
            {
                if (boid.position.x < -boid.size.x)
                    boid.position.x = GetScreenWidth() + boid.size.x;
                if (boid.position.x > GetScreenWidth() + boid.size.x)
                    boid.position.x = -boid.size.x;
                if (boid.position.y < -boid.size.y)
                    boid.position.y = GetScreenHeight() + boid.size.y;
                if (boid.position.y > GetScreenHeight() + boid.size.y)
                    boid.position.y = -boid.size.y;
            }

            // Clamp velocity
            //boid.velocity = Vector2Clamp(boid.velocity, (Vector2){-boid.minSpeed, -boid.minSpeed}, (Vector2){boid.maxSpeed, boid.maxSpeed});

            float speed = sqrt(boid.velocity.x * boid.velocity.x + boid.velocity.y * boid.velocity.y);

            if (speed < boid.minSpeed)
            {
                boid.velocity = (Vector2){
                            (boid.velocity.x / speed) * boid.minSpeed, 
                            (boid.velocity.y / speed) * boid.minSpeed
                        };
            }
            else if (speed > boid.maxSpeed)
            {
                boid.velocity = (Vector2){
                            (boid.velocity.x / speed) * boid.maxSpeed,
                            (boid.velocity.y / speed) * boid.maxSpeed
                        };
            }

            // Update Position with velocity           
            // boid.position = Vector2Add(boid.position, boid.velocity);
            boid.position.x += boid.velocity.x;
            boid.position.y += boid.velocity.y;
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

            if (debugLines)
            {
                if (boid.index == 20)
                {
                    // Vision Radius
                    DrawCircleLines(
                            boid.position.x + boid.size.x / 2, 
                            boid.position.y + boid.size.y / 2, 
                            boid.visionRadius, 
                            BLUE
                        );

                    // Protected Radius
                    DrawCircleLines(
                            boid.position.x + boid.size.x / 2, 
                            boid.position.y + boid.size.y / 2, 
                            boid.protectedRadius, 
                            RED
                        );
                }
            }
        }
        
        EndDrawing();
    }

    CloseWindow();


    return 0;
}
