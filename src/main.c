#include "bsp_vis2.h"

int main(void)
{

    // Maybe make an algorithm to determine width?
    int map[64] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1};

    int map_count = sizeof(map) / sizeof(map[0]);
    Vector2 map_subdivisions = {.x = (int)8, .y = (int)8};

    // Initialization
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y, "Map Conquer");
    SetTargetFPS(60);
    int scissorMode = 0;

    // DONT CALL GETSCREENWIDTH OR GETSCREENHEIGHT BEFORE INITWINDOW
    Vector2 screen_center;
    Vector2 world_dimensions;
    Vector2 world_tile_size;
    Vector2 player_size;
    float tile_sw, tile_sh;
    Camera2D map_cam, mini_map_cam, INFO_cam;
    Player player;
    Rectangle left_half, upper_right, lower_right;

    // Map dimensions
    screen_center = get_screen_center();
    world_tile_size = update_world_tile_size(get_screen_dimensions(), map_subdivisions);
    tile_sw = world_tile_size.x; // world_size / subdivisions
    tile_sh = world_tile_size.y;
    world_dimensions = get_map_world_size(map_subdivisions, world_tile_size);
    // Scissor viewports
    left_half = update_rectangle(left_half, 0, 0, screen_center.x, screen_center.y * 2);
    upper_right = update_rectangle(upper_right, screen_center.x, 0, screen_center.x, screen_center.y);
    lower_right = update_rectangle(lower_right, screen_center.x, screen_center.y, screen_center.x, screen_center.y);
    // Player
    player_size = (Vector2){.x = tile_sw / 4, .y = tile_sh / 3};
    init_player(&player, 64, map);
    
    
    // Cameras
    map_cam = init_cam(screen_center);
    mini_map_cam = init_cam(player.position);
    INFO_cam = init_cam(screen_center);

    float move_speed_factor = 200;
    Vector4 rdda = (Vector4){.x = 0, .y = 0, .z = 0, .w = 0};
    int num_rays = 60;

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_R))
        {
            player.position = get_screen_center();
        }
        
        if (IsWindowResized())
        {
            screen_center = get_screen_center();
            world_tile_size = update_world_tile_size(get_screen_dimensions(), map_subdivisions);
            tile_sw = world_tile_size.x; // world_size / subdivisions
            tile_sh = world_tile_size.y;
            world_dimensions = get_map_world_size(map_subdivisions, world_tile_size);
            
            left_half = update_rectangle(left_half, 0, 0, screen_center.x, screen_center.y * 2);
            upper_right = update_rectangle(upper_right, screen_center.x, 0, screen_center.x, screen_center.y);
            lower_right = update_rectangle(lower_right, screen_center.x, screen_center.y, screen_center.x, screen_center.y);
            
            map_cam = init_cam(screen_center);
            mini_map_cam = init_cam(screen_center);
            INFO_cam = init_cam(screen_center);
        }
        
        // --- Update player ---
        Vector4* r = &rdda;
        controls(&player, move_speed_factor, GetFrameTime(), r);

        // --- MAP VIEW ---
        update_camera(&map_cam, player.position, get_rect_center(left_half), 1.0f);
        // --- MINI_MAP VIEW ---
        Vector2 upper_right_to_world = get_rec_to_world(upper_right, world_dimensions);
        float mini_map_zoom = fminf(upper_right_to_world.x, upper_right_to_world.y);
        update_camera(&mini_map_cam, get_map_world_center(world_dimensions), get_rect_center(upper_right), mini_map_zoom);
        // --- INFO VIEW  ---
        Vector2 lower_right_to_world = get_rec_to_world(lower_right, world_dimensions);
        float INFO_zoom = fminf(lower_right_to_world.x, lower_right_to_world.y);
        update_camera(&INFO_cam, get_map_world_center(world_dimensions), get_rect_center(lower_right), mini_map_zoom);

        // --- RENDER ---
        BeginDrawing();
        ClearBackground(BLACK);

        // RENDER VIEW
        manage_scissor_camera(&left_half, &map_cam, map, map_subdivisions, &player, RED, world_tile_size, RENDER, r, num_rays);
        // RAY RIGHT VIEW
        manage_scissor_camera(&upper_right, &mini_map_cam, map, map_subdivisions, &player, RED, world_tile_size, RAY, r, num_rays);
        // INFO RIGHT VIEW
        manage_scissor_camera(&lower_right, &mini_map_cam, map, map_subdivisions, &player, RED, world_tile_size, INFO, r, num_rays);

        // Left View Title
        DrawText("Rendered", left_half.x + 20, left_half.y + 20, 20, WHITE);
        // Right View Title
        DrawText("Ray Cast", upper_right.x + 20, upper_right.y + 20, 20, WHITE);
        // Bottom Right View Title
        DrawText("Info", lower_right.x + 20, lower_right.y + 20, 20, WHITE);
        // Draw player angle
        EndDrawing();
    }
    CloseWindow();
    return 0;
}