#include "raygrid.h"

int main(void)
{

    // Maybe make an algorithm to determine width?
    int map[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1};
    int map_total_element = sizeof(map) / sizeof(map[0]);
    Vector2 map_subdivisions = {.x = (int)8, .y = (int)8};

    // Initialization
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y, "Map Conquer");
    SetTargetFPS(60);
    int scissorMode = 0;

    // DONT CALL GETSCREENWIDTH OR GETSCREENHEIGHT BEFORE INITWINDOW
    Vector2 center = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};


    Camera2D map_cam, player_cam, mini_map_cam;
    Vector2 player_size = (Vector2){.x = .0333 * DEFAULT_WINDOW_X, .y = .0667 * DEFAULT_WINDOW_Y};
    Player player = init_player(center, (Vector2){0, 0}, (Vector2){0, 0}, player_size, 0, 0);

    map_cam = init_cam(center);
    player_cam = init_cam(player.position);
    mini_map_cam = init_cam(center);
    // half width and full height
    Rectangle left_half =
        {
            .x = 0,
            .y = 0,
            .width = GetScreenWidth() / 2,
            .height = GetScreenHeight()};
    Rectangle upper_right =
        {
            .x = GetScreenWidth() / 2,
            .y = 0,
            .width = GetScreenWidth() / 2,
            .height = GetScreenHeight() / 2};
    Rectangle lower_right =
        {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2,
            .width = GetScreenWidth() / 2,
            .height = GetScreenHeight() / 2};

    while (!WindowShouldClose())
    {
        // --- Recalculate viewports (window is resizable) ---
        float tsx = GetScreenWidth() / map_subdivisions.x;
        float tsy = GetScreenHeight() / map_subdivisions.y;
        float world_x = map_subdivisions.x * tsx;
        float world_y = map_subdivisions.y * tsy;
        left_half = update_rectangle(left_half, 0, 0, GetScreenWidth() / 2, GetScreenHeight());
        upper_right = update_rectangle(upper_right, GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight() / 2);
        lower_right = update_rectangle(lower_right, GetScreenWidth() / 2, GetScreenHeight() / 2, GetScreenWidth() / 2, GetScreenHeight() / 2);
        Vector2 MAP_WORLD_CENTER = (Vector2){
            world_x / 2.0f,
            world_y / 2.0f
        };
        Vector2 map_cam_center = (Vector2){
            left_half.x + left_half.width / 2.0f,
            left_half.height / 2.0f
        };
        Vector2 player_cam_center = (Vector2){
            upper_right.x + upper_right.width / 2.0f,
            upper_right.height / 2.0f
        };
        Vector2 mini_map_screen_center = (Vector2){
            lower_right.x + lower_right.width / 2.0f,
            lower_right.y + lower_right.height / 2.0f
        };
        
        // --- Update player ---
        controls(&player);

        // --- LEFT CAMERA ---
        map_cam.target = player.position;
        map_cam.offset = map_cam_center;

        // --- UP RIGHT CAMERA ---
        player_cam.target = player.position;
        player_cam.offset = player_cam_center;

        // --- LOWER RIGHT CAMERA ---
        mini_map_cam.target = MAP_WORLD_CENTER; // World Space
        mini_map_cam.offset = mini_map_screen_center; // Screen Space
        mini_map_cam.zoom = fminf(
            lower_right.width  / world_x,
            lower_right.height / world_y
        );

        // --- RENDER ---
        BeginDrawing();
        ClearBackground(BLACK);

        // LEFT VIEW
        BeginScissorMode(left_half.x, left_half.y,
                        left_half.width, left_half.height);
        BeginMode2D(map_cam);
            draw_map(map, 8, 8);
            draw_player(&player, RED);
        EndMode2D();
        EndScissorMode();

        // UPPER RIGHT VIEW
        BeginScissorMode(upper_right.x, upper_right.y,
                        upper_right.width, upper_right.height);
        BeginMode2D(player_cam);
            draw_map(map, 8, 8);
            draw_player(&player, RED);
        EndMode2D();
        EndScissorMode();
        
        // BOTTOM RIGHT VIEW
        BeginScissorMode(lower_right.x, lower_right.y,
                        lower_right.width, lower_right.height);
        BeginMode2D(mini_map_cam);
            draw_map(map, 8, 8);
            draw_player(&player, RED);
        EndMode2D();
        EndScissorMode();

        // Left View Title
        DrawText("Rendered", left_half.x + 20, left_half.y+ 20, 20, WHITE);
        // Right View Title
        DrawText("Ray Cast", upper_right.x + 20, upper_right.y+ 20, 20, WHITE);
        // Bottom Right View Title
        DrawText("BSP Tree", lower_right.x + 20, lower_right.y+ 20, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}