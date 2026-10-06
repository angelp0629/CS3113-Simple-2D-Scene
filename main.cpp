/**

* Author: Angel Perez

* Assignment: Simple 2D Scene

* Date due: 10/05/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/cs3113.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// ---------------------------------------------------------
// Global Constants
// ---------------------------------------------------------

constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS           = 60;

// Object constants

constexpr float EARTH_ORBIT_X = 40.0f,
                EARTH_ORBIT_Y = 25.0f,
                EARTH_SPEED   = 0.05f,
                EARTH_ROT_SPEED = 10.0f;

constexpr float MOON_ORBIT_X = 360.0f,
                MOON_ORBIT_Y = 220.0f,
                MOON_SPEED   = 0.02f;

constexpr float ROCKET_SPEED = 90.0f,
                ROCKET_CURVE = 180.0f,
                ROCKET_ROT_SPEED = 8.0f;

constexpr float METEOR_SPEED = 120.0f,
                METEOR_GROWTH = 300.0f;

// Background color

constexpr char BG_COLOUR[] = "#151526";

// Objects

constexpr char EARTH_FP[] = "assets/game/earth.png";
constexpr char MOON_FP[] = "assets/game/moon.png";
constexpr char ROCKET_FP[] = "assets/game/rocket.png";
constexpr char METEOR_FP[] = "assets/game/meteor.png";

// ---------------------------------------------------------
// Global Variables
// ---------------------------------------------------------

AppStatus gAppStatus = RUNNING;

// Textures
Texture2D gEarthTexture;
Texture2D gMoonTexture;
Texture2D gRocketTexture;
Texture2D gMeteorTexture;

// Positions
Vector2 gEarthPosition = { 400.0f, 450.0f };
Vector2 gMoonPosition = { 700.0f, 300.0f };
Vector2 gRocketPosition = { 0.0f, 0.0f };
Vector2 gRocketStartPosition = { 0.0f, 0.0f };
Vector2 gMeteorPosition = { 1200.0f, 200.0f };

// Sizes
Vector2 gEarthScale = { 200.0f, 200.0f };
Vector2 gMoonScale = { 120.0f, 120.0f };
Vector2 gRocketScale = { 150.0f, 150.0f };
Vector2 gMeteorScale = { 100.0f, 100.0f };

// Rotation angles
float gEarthAngle = 0.0f;
float gMoonAngle = 0.0f;
float gRocketAngle = 0.0f;
float gMeteorAngle = 0.0f;

// Orbit locations
float gEarthOrbitLocation = 0.0f;
float gMoonOrbitLocation = 0.0f;

// Origin locations
Vector2 EARTH_ORIGIN = gEarthPosition;
Vector2 MOON_ORIGIN = gMoonPosition; 

// Used for animations
float gTime = 0.0f;
float gRocketTime = 0.0f;
Color gBackgroundColor = ColorFromHex(BG_COLOUR);

// ---------------------------------------------------------
// Function Declarations
// ---------------------------------------------------------

void initialise();
void processInput();
void update();
void render();
void shutdown();

void drawObject(
    Texture2D texture,
    Vector2 position,
    Vector2 scale,
    float angle
);

// ---------------------------------------------------------
// Function Definitions
// ---------------------------------------------------------

void initialise()
{
    InitWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Assignment 1 - Simple 2D Scene"
    );

    // Load textures
    gEarthTexture = LoadTexture(EARTH_FP);
    gMoonTexture = LoadTexture(MOON_FP);
    gRocketTexture = LoadTexture(ROCKET_FP);
    gMeteorTexture = LoadTexture(METEOR_FP);

    gRocketStartPosition.x = gEarthPosition.x + 80.0f;
    gRocketStartPosition.y = gEarthPosition.y;
    gRocketPosition = gRocketStartPosition;

    SetTargetFPS(FPS);

}

// ---------------------------------------------------------

void processInput()
{
    if (WindowShouldClose())
    {
        gAppStatus = TERMINATED;
    }
}

// ---------------------------------------------------------

void update()
{
    // Delta time
    float deltaTime = GetFrameTime();

    // Keep track of total time
    gTime += deltaTime;

    // Make Earth rotate and orbit elliptically
    gEarthAngle += EARTH_ROT_SPEED * deltaTime;
    gEarthOrbitLocation += EARTH_SPEED;
    gEarthPosition.x = EARTH_ORIGIN.x + EARTH_ORBIT_X * cosf(gEarthOrbitLocation);
    gEarthPosition.y = EARTH_ORIGIN.y + EARTH_ORBIT_Y * sinf(gEarthOrbitLocation);

    // Moon Orbit
    gMoonOrbitLocation += MOON_SPEED;
    gMoonPosition.x = gEarthPosition.x + MOON_ORBIT_X * cosf(gMoonOrbitLocation);
    gMoonPosition.y = gEarthPosition.y + MOON_ORBIT_Y * sinf(gMoonOrbitLocation);

    // Rocket movement
    gRocketTime += deltaTime;
    gRocketPosition.x = gRocketStartPosition.x + ROCKET_SPEED * gRocketTime;
    gRocketPosition.y = gRocketStartPosition.y - ROCKET_CURVE * sinf(gRocketTime * 0.6f);
    gRocketAngle = -20.0f + 15.0f * sinf(gRocketTime * 0.6f);

    // Meteor movement
    gMeteorPosition.x -= METEOR_SPEED * deltaTime;
    gMeteorPosition.y += METEOR_SPEED * 0.5f * deltaTime;
    // Meteor grows as if it is coming toward the screen
    gMeteorScale.x += METEOR_GROWTH * deltaTime;
    gMeteorScale.y += METEOR_GROWTH * deltaTime;

    // Background color change
    unsigned char red = static_cast<unsigned char>(20.0f + 15.0f * sinf(gTime * 0.5f));
    unsigned char green = static_cast<unsigned char>(20.0f + 10.0f * sinf(gTime * 0.7f));
    unsigned char blue = static_cast<unsigned char>(50.0f + 30.0f * sinf(gTime * 0.6f));
    gBackgroundColor = {
    red,
    green,
    blue,
    255};
}

// ---------------------------------------------------------

void drawObject(
    Texture2D texture,
    Vector2 position,
    Vector2 scale,
    float angle
)
{
    // Whole texture
    Rectangle textureArea = {
        0.0f,
        0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)
    };

    // Position and size on screen
    Rectangle destinationArea = {
        position.x,
        position.y,
        scale.x,
        scale.y
    };

    // Center of texture
    Vector2 originOffset = {
        scale.x / 2.0f,
        scale.y / 2.0f
    };

    DrawTexturePro(
        texture,
        textureArea,
        destinationArea,
        originOffset,
        angle,
        WHITE
    );
}

// ---------------------------------------------------------

void render()
{
    BeginDrawing();

    ClearBackground(gBackgroundColor);
    // Draw all objects
    drawObject(
        gEarthTexture,
        gEarthPosition,
        gEarthScale,
        gEarthAngle
    );
    drawObject(
        gMoonTexture,
        gMoonPosition,
        gMoonScale,
        gMoonAngle
    );
    drawObject(
        gRocketTexture,
        gRocketPosition,
        gRocketScale,
        gRocketAngle
    );
    drawObject(
        gMeteorTexture,
        gMeteorPosition,
        gMeteorScale,
        gMeteorAngle
    );

    EndDrawing();
}

// ---------------------------------------------------------

void shutdown()
{
    UnloadTexture(gEarthTexture);
    UnloadTexture(gMoonTexture);
    UnloadTexture(gRocketTexture);
    UnloadTexture(gMeteorTexture);

    CloseWindow();
}

// ---------------------------------------------------------

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}