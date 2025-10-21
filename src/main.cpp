#include <format>
#include <iostream>
#include "raylib.h"

int main() {
    constexpr int screenWidth = 1200;
    constexpr int screenHeight = 800;
    std::cout << std::format("Starting with screen dimensions {}x{}", screenWidth, screenHeight);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}