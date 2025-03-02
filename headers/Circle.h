#include "Vec3.h"
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include "Velocity.h"

struct Circle{
    float radius=50;
    Vec3 point;
    float gravity=0;
    Velocity vel;
};

Vec3 getCirclePos (Circle& a){
    return getVecPos(a.point);
}

bool circlevsCircleOptimizedInverted(Circle& a, Circle& b)
{   
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;  
    float r = a.radius - b.radius;
    float distanceSquared = (distance_x * distance_x) + (distance_y * distance_y);
    return distanceSquared <= (r*r);
}

bool circlevsCircleOptimized(Circle& a, Circle& b)
{   
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;  
    float r = a.radius + b.radius;
    float distanceSquared = (distance_x * distance_x) + (distance_y * distance_y);
    return distanceSquared <= (r*r);
}

Vec3 resolveCircle2(Circle &a, Circle &b, float deltaTime) {
    // Calculate the vector between the two circles' centers
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;

    // Calculate the distance between the centers
    float length = sqrt(distance_x * distance_x + distance_y * distance_y);
    
    if (length < 0.001f) {
        // Push in a random direction to avoid getting stuck
        distance_x = 1.0f;
        distance_y = 0.0f;
        length = 1.0f;
    }

    // Unit vector in the direction of the collision
    float unit_x = distance_x / length;
    float unit_y = distance_y / length;

    a.point.x = b.point.x + unit_x * (b.radius - a.radius);
    a.point.y = b.point.y + unit_y * (b.radius - a.radius);

    // Calculate the dot product to reflect the velocity along the collision normal
    float dot = (a.vel.point.x * unit_x + a.vel.point.y * unit_y);

    if (dot < 0) {
        // Calculate reflection of velocity along the normal
        // v' = v - 2(v·n)n
        a.vel.point.x = a.vel.point.x - 2.0f * dot * unit_x;
        a.vel.point.y = a.vel.point.y - 2.0f * dot * unit_y;
        
        // Apply energy loss (damping)
        const float BOUNCE_DAMPING = 0.8f;
        a.vel.point.x *= BOUNCE_DAMPING;
        a.vel.point.y *= BOUNCE_DAMPING;
    }

    a.vel.point.x -= 2 * dot * unit_x;
    a.vel.point.y -= 2 * dot * unit_y;
    
    // Apply damping (multiply, not add)
    //const float DAMPING = 0.99f;  // 0.8 = 80% of original speed after bounce
    //a.vel.point.x *= DAMPING;
    //a.vel.point.y *= DAMPING;

    // Ensure minimum velocity after bounce to prevent sticking
    float speed = sqrt(a.vel.point.x * a.vel.point.x + 
        a.vel.point.y * a.vel.point.y);

    const float MIN_SPEED = 30.0f;
    if (speed < MIN_SPEED) {
    // If almost stopped, give a push away from the boundary
    float angle = atan2(unit_y, unit_x) + 3.14159f; // Push in opposite direction
    a.vel.point.x = MIN_SPEED * cos(angle);
    a.vel.point.y = MIN_SPEED * sin(angle);
    }

    // Apply a single step of movement after resolving
    a.point.x += a.vel.point.x * deltaTime;
    a.point.y += a.vel.point.y * deltaTime;

    return a.point;
    }

    Vec3 resolveCircle3(Circle &a, Circle &b, float deltaTime) {
        // Calculate the vector between the two circles' centers
        float distance_x = a.point.x - b.point.x;
        float distance_y = a.point.y - b.point.y;
    
        // Calculate the distance between the centers
        float length = sqrt(distance_x * distance_x + distance_y * distance_y);
        
        if (length < 0.001f) {
            // Push in a random direction to avoid getting stuck
            distance_x = 1.0f;
            distance_y = 0.0f;
            length = 1.0f;
        }
    
        // Unit vector in the direction of the collision
        float unit_x = distance_x / length;
        float unit_y = distance_y / length;
    
        a.point.x = b.point.x + unit_x * (b.radius + a.radius);
        a.point.y = b.point.y + unit_y * (b.radius + a.radius);
    
        // Calculate the dot product to reflect the velocity along the collision normal
        float dot = (a.vel.point.x * unit_x + a.vel.point.y * unit_y);
    
        if (dot < 0) {
            // Calculate reflection of velocity along the normal
            // v' = v - 2(v·n)n
            a.vel.point.x = a.vel.point.x - 2.0f * dot * unit_x;
            a.vel.point.y = a.vel.point.y - 2.0f * dot * unit_y;
            
            // Apply energy loss (damping)
            const float BOUNCE_DAMPING = 0.8f;
            a.vel.point.x *= BOUNCE_DAMPING;
            a.vel.point.y *= BOUNCE_DAMPING;
        }
    
        a.vel.point.x -= 2 * dot * unit_x;
        a.vel.point.y -= 2 * dot * unit_y;
        
        // Apply damping (multiply, not add)
        //const float DAMPING = 0.99f;  // 0.8 = 80% of original speed after bounce
        //a.vel.point.x *= DAMPING;
        //a.vel.point.y *= DAMPING;
    
        // Ensure minimum velocity after bounce to prevent sticking
        float speed = sqrt(a.vel.point.x * a.vel.point.x + 
            a.vel.point.y * a.vel.point.y);
    
        const float MIN_SPEED = 30.0f;
        if (speed < MIN_SPEED) {
        // If almost stopped, give a push away from the boundary
        float angle = atan2(unit_y, unit_x) + 3.14159f; // Push in opposite direction
        a.vel.point.x = MIN_SPEED * cos(angle);
        a.vel.point.y = MIN_SPEED * sin(angle);
        }
    
        // Apply a single step of movement after resolving
        a.point.x += a.vel.point.x * deltaTime;
        a.point.y += a.vel.point.y * deltaTime;
    
        return a.point;
        }


Vec3 movement(Circle &a, float deltaTime) {
    // Apply gravity to the vertical component of velocity
    // For screen coordinates, make sure gravity pulls downward (positive y)
    a.vel.point.y += 400.0f * deltaTime;  // Use a fixed positive value for downward gravity
    
    // Limit maximum velocity to prevent excessive speeds
    const float MAX_VELOCITY = 600.0f;
    float speedSq = a.vel.point.x * a.vel.point.x + a.vel.point.y * a.vel.point.y;
    if (speedSq > MAX_VELOCITY * MAX_VELOCITY) {
        float speed = sqrt(speedSq);
        a.vel.point.x = a.vel.point.x / speed * MAX_VELOCITY;
        a.vel.point.y = a.vel.point.y / speed * MAX_VELOCITY;
    }

    // Apply very minimal air resistance
    //const float DAMPING = 0.999f;
    //a.vel.point.x *= DAMPING;
    //a.vel.point.y *= DAMPING;

    // Update the position based on velocity and deltaTime
    a.point.x += a.vel.point.x * deltaTime;
    a.point.y += a.vel.point.y * deltaTime;

    return {a.point.x, a.point.y, 0};
}

void drawCircle(SDL_Renderer* renderer, Circle& circle){
    const int32_t diameter = circle.radius * 2;
    int32_t x = (circle.radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y){
         //  Each of the following renders an octant of the circle
        SDL_RenderPoint(renderer, circle.point.x + x, circle.point.y - y);
        SDL_RenderPoint(renderer, circle.point.x + x, circle.point.y + y);
        SDL_RenderPoint(renderer, circle.point.x - x, circle.point.y - y);
        SDL_RenderPoint(renderer, circle.point.x - x, circle.point.y + y);
        SDL_RenderPoint(renderer, circle.point.x + y, circle.point.y - x);
        SDL_RenderPoint(renderer, circle.point.x + y, circle.point.y + x);
        SDL_RenderPoint(renderer, circle.point.x - y, circle.point.y - x);
        SDL_RenderPoint(renderer, circle.point.x - y, circle.point.y + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void drawCircleFilled(SDL_Renderer* renderer, Circle& circle){
    const int32_t diameter = circle.radius * 2;
    int32_t x = (circle.radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y){
         //  Each of the following renders an octant of the circle
        SDL_RenderLine(renderer, circle.point.x - y, circle.point.y + x, circle.point.x + y, circle.point.y + x);
        SDL_RenderLine(renderer, circle.point.x - x, circle.point.y + y, circle.point.x + x, circle.point.y + y);
        SDL_RenderLine(renderer, circle.point.x - x, circle.point.y - y, circle.point.x + x, circle.point.y - y);
        SDL_RenderLine(renderer, circle.point.x - y, circle.point.y - x, circle.point.x + y, circle.point.y - x);
        
        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}


/*bool circlevsCircleUnoptimized(Circle& a, Circle& b)
{
  float r = a.radius + b.radius;
  return r < distance( getCirclePos(a), getCirclePos(b) );
}*/

/*bool circlevsCircleUnoptimizedInverted(Circle& a, Circle& b)
{   
    int radius;
    if (a.radius > b.radius){
        radius = a.radius - b.radius;
        radius = radius * radius - b.radius * b.radius;
    } else {
        radius = b.radius - a.radius;
        radius = radius * radius - a.radius * a.radius;
    }
    
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;  
    float r = a.radius + b.radius;
    if ((distance_x * distance_x + radius) + (distance_y * distance_y + radius) <= r * r){
        return true;
    } 
        return false;
}*/



/*Vec3 resolveCircle(Circle& a, Circle& b){
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;  
    float r = a.radius - b.radius;
    float length = sqrt(distance_x * distance_x + distance_y * distance_y);
    float unit_x = distance_x / length;
    float unit_y = distance_y / length;

    std::cout << length << std::endl;
    std::cout << unit_x << std::endl;
    std::cout << unit_y << std::endl;

    a.point.x = b.point.x + (r + 1) * unit_x;
    a.point.y = b.point.y + (r + 1) * unit_y;

    return {a.point.x ,a.point.y, 0};
}*/

/*Vec3 resolveCircle2(Circle &a, Circle &b, float deltaTime) {
    float distance_x = a.point.x - b.point.x;
    float distance_y = a.point.y - b.point.y;  

    float length = sqrt(distance_x * distance_x + distance_y * distance_y);

    float unit_x = distance_x / length;
    float unit_y = distance_y / length;

    float dot = (a.vel.point.x * unit_x + a.vel.point.y * unit_y);
    a.vel.point.x -= 2 * dot * unit_x;
    a.vel.point.y -= 2 * dot * unit_y;

    a.vel.point.y -= a.gravity * deltaTime;

    a.point.x += a.vel.point.x * deltaTime;
    a.point.y += a.vel.point.y * deltaTime;

    return {a.point.x, a.point.y, 0};
}*/

