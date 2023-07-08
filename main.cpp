
#include <raylib.h>
#include <iostream>
#include <vector>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "vector.cpp"
#include "internal.cpp"

const float2 Center = resolution / 2;

float FrameTime = 0.0f;
int Tick = 0;

int main () {
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Anti-aliasing x4 (if available)
    // SetConfigFlags(FLAG_VSYNC_HINT); // Anti-aliasing x4 (if available)
    InitWindow(resolution.x, resolution.y, "");

    RenderTexture2D target = LoadRenderTexture(resolution.x, resolution.y);
    RenderTexture2D samplerTarget = LoadRenderTexture(resolution.x, resolution.y);
    
    SetTargetFPS(60);

    start();

    HideCursor();

    while (WindowShouldClose() == false){
        FrameTime = GetFrameTime(); // Get time took to render last frame
        Loop(Tick);

        if(IsWindowResized())
        {
            resolution = (float2) { (float)GetScreenWidth(), (float)GetScreenHeight() };
            center = resolution * 0.5f;
        }


        BeginTextureMode(target);
            ClearBackground(BLACK);


                Render();


            DrawText(std :: to_string(GetFPS()).c_str(), 30, 30, 20, SKYBLUE);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);

            RenderVFX(&target, &samplerTarget);

        EndDrawing();


        Tick ++;
    }

    CloseWindow();

    system("cls");
    return 0;
}
