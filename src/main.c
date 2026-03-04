#include "bsp_vis2.h"


int main(void)
{
    int map[64] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
};
    InitWindow(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y, "BSP Visualizer");
    SetTargetFPS(DEFAULT_FPS);

    Player player;
    init_player(&player, 64, map);

    while (!WindowShouldClose())
    {
        controls(&player, GetFrameTime());
        if (IsKeyDown(KEY_R))
        {
            player.position = (Vector2){.x = GetScreenWidth() / 2, .y = GetScreenHeight() / 2};
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        draw(&player, 64, map, 64, 60.0f, 8);
        DrawText(TextFormat("angle radi: %f, degrees: %.2f", player.rad_angle, player.rad_angle*RAD2DEG), 100, 40, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}