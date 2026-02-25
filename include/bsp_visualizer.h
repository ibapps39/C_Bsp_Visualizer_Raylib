#pragma once

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h> // Required for malloc/free

// =========================================================
//                   - TODO -
// =======================================================
// - Define a playable area to separate the GUI from the map
// - Add a visual indicator (a glowing border) to the command button that is currently active?

// =========================================================
//                   - DEFAULTS, CONSTANTS, & MACROS -
// =========================================================
#define DEFAULT_BOTTOM_SCREEN DEFAULT_WINDOW_Y
#define DEFAULT_CENTER_SCREEN (Vector2){.x = DEFAULT_WINDOW_X / 2, .y = DEFAULT_WINDOW_Y / 2}
#define DEFAULT_FPS 60
#define DEFAULT_MAP_VECTOR2D \
    (Vector2) { DEFAULT_MAP_SIZE_X, DEFAULT_MAP_SIZE_Y }
#define DEFAULT_PLAYER_COLOR (Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define DEFAULT_PLAYER_SIZE \
    (Vector2) { .x = .0333 * DEFAULT_WINDOW_X, .y = .0667 * DEFAULT_WINDOW_Y }
#define DEFAULT_RESOLUTION ((int)DEFAULT_WINDOW_X * (int)DEFAULT_WINDOW_Y)
#define DEFAULT_WINDOW_X 600
#define DEFAULT_WINDOW_Y 700
#define DEFAULT_GRID_SQUARE_LW 8
#define THREE_PI_HALF ((3.0f * M_PI)/2.0f)
#define HALF_PI (M_PI/2.0f)

// =========================================================
//  NEON COLORS
// =========================================================
#define NEON_GREEN (Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define NEON_BLUE (Color){.r = 0, .g = 0, .b = 255, .a = 255}
#define NEON_RED (Color){.r = 255, .g = 0, .b = 0, .a = 255}
#define NEON_YELLOW (Color){.r = 255, .g = 255, .b = 0, .a = 255}
#define NEON_PINK (Color){.r = 255, .g = 0, .b = 255, .a = 255}
#define NEON_PURPLE (Color){.r = 255, .g = 0, .b = 255, .a = 255}
#define NEON_COLOR_COUNT 6-1

// =========================================================
//  DEFINES & TYPES - Enums
// =========================================================

typedef enum Tile_Type
{
    EMPTY = 0,
    WALL = 1
} Tile_Type;

typedef enum Neon_Colors
{
    green_n,
    blue_n,
    red_n,
    yellow_n,
    pink_n,
    purple_n
} Neon_Colors;

// =========================================================
//  DEFINES & TYPES - Structs
// =========================================================

typedef struct Player
{
    Vector2 position;
    Vector2 speed;
    Vector2 forward_v;
    Vector2 size;
    float angle_radians;
} Player;

// // =========================================================
// //                      - INTERFACE -
// // =========================================================

// // =========================================================
// //                  - IMPLEMENTATION -
// // =========================================================

// // =========================================================
// //  Math
// // =========================================================

Vector2 get_rand_vec2(Vector2 min, Vector2 max)
{
    Vector2 result;
    result.x = (float)rand() / (float)RAND_MAX * (max.x - min.x) + min.x;
    result.y = (float)rand() / (float)RAND_MAX * (max.y - min.y) + min.y;
    return result;
}

// // =========================================================
// //  Collision
// // =========================================================

int get_tile(int *map, int x, int y, Vector2 subdivisions)
{
    // Safety: check bounds before array access
    if (x < 0 || y < 0 || x >= subdivisions.x || y >= subdivisions.y)
        return -1;
    return map[y * (int)subdivisions.x + x];
}

// // =========================================================
// //  Initializations
// // =========================================================

// Camera - return Camera2D
Camera2D init_cam(Vector2 pos)
{
    Camera2D cam;
    cam.target = pos;
    cam.offset = pos;
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    return cam;
}
Player init_player(Vector2 pos, Vector2 speed, Vector2 forward_v, Vector2 size, float angle_radians)
{
    Player player;
    player.position = pos;
    player.speed = speed;
    player.forward_v = forward_v;
    player.size = size;
    player.angle_radians = angle_radians;
    return player;
}

// // =========================================================
// //  State
// // =========================================================

// // =========================================================
// //  Helpers
// // =========================================================

Vector2 get_rec_to_world(Rectangle r, Vector2 world_size)
{
    return (Vector2){
        .x = r.width / world_size.x,
        .y = r.height / world_size.y};
}

Rectangle update_rectangle(Rectangle r, float x, float y, float width, float height)
{
    return r = (Rectangle){
               .x = x,
               .y = y,
               .width = width,
               .height = height};
}

Vector2 get_screen_dimensions()
{
    return (Vector2){.x = GetScreenWidth(), .y = GetScreenHeight()};
}
Vector2 get_screen_center()
{
    return (Vector2){.x = GetScreenWidth() / 2.0f, .y = GetScreenHeight() / 2.0f};
}
Rectangle update_viewports(Rectangle r, int x, int y, int w, int h)
{
    return update_rectangle(r, x, y, w, h);
}
Camera2D get_camera(Vector2 target, Vector2 offset, float zoom)
{
    Camera2D cam = {0};
    cam.target = target;
    cam.offset = offset;
    cam.zoom = zoom;
    return cam;
}
void update_camera(Camera2D *cam, Vector2 target, Vector2 offset, float zoom)
{
    cam->target = target;
    cam->offset = offset;
    cam->zoom = zoom;
}
void update_forwardv(Player* player, float angle_radians)
{
    player->forward_v.x = cosf(angle_radians);
    player->forward_v.y = sinf(angle_radians);
}
Vector2 get_rect_center(Rectangle r)
{
    return (Vector2){r.x + r.width / 2.0f, r.y + r.height / 2.0f};
}
Vector2 get_map_world_size(Vector2 subdivisions, Vector2 tile_size)
{
    return (Vector2){subdivisions.x * tile_size.x, subdivisions.y * tile_size.y};
}

Vector2 get_map_world_center(Vector2 world_size)
{
    return (Vector2){world_size.x / 2.0f, world_size.y / 2.0f};
}
Vector2 update_world_tile_size(Vector2 world_size, Vector2 subdivisions)
{
    // typically GetScreenWidth() and GetScreenHeight()
    float tile_size_width = world_size.x / subdivisions.x;
    float tile_size_height = world_size.y / subdivisions.y;
    return (Vector2){.x = tile_size_width, .y = tile_size_height};
}
float increment_angle(float* radians, float amt)
{
    amt = fmod(amt, 2 * M_PI);
    amt = amt < 0 ? amt + 2 * M_PI : amt;
    *radians = amt;
    return *radians;
}
void update_player_angle(Player* player, float radians)
{
    player->angle_radians += radians;
}
// // =========================================================
// //  Drawing
// // =========================================================

void draw_map_2D_2D(int *map, int width, int height, Vector2 tile_size)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int i = y * width + x;

            Color color = (map[i] == WALL) ? BLUE : DARKGRAY;

            DrawRectangle(
                x * tile_size.x,
                y * tile_size.y,
                tile_size.x - 1,
                tile_size.y - 1,
                color
            );
        }
    }
}

void draw_player(Player *player, Color c)
{
    Rectangle r = {player->position.x, player->position.y, player->size.x, player->size.y};
    DrawRectangleRec(r, c);
}
void draw_ray(Ray ray, Vector2 end, Color c)
{

}
void draw_player_forwardv(Player *player, Color c)
{
    // Calculate the visual center of the player
    Vector2 center = { 
        player->position.x + player->size.x / 2.0f, 
        player->position.y + player->size.y / 2.0f 
    };

    // The end point is: Center + (Direction * Length)
    float lineLength = 30.0f; 
    Vector2 lineEnd = Vector2Add(center, Vector2Scale(player->forward_v, lineLength));

    DrawLineEx(center, lineEnd, 2.0f, c);
}

// // =========================================================
// //  Art
// // =========================================================

Color get_neon_color(Neon_Colors color)
{
    switch (color)
    {
    case green_n:
        return NEON_GREEN;
    case blue_n:
        return NEON_BLUE;
    case red_n:
        return NEON_RED;
    case yellow_n:
        return NEON_YELLOW;
    case pink_n:
        return NEON_PINK;
    case purple_n:
        return NEON_PURPLE;
    default:
        return NEON_GREEN;
    }
}

Color get_random_color()
{
    return get_neon_color((Neon_Colors)(GetRandomValue(0, NEON_COLOR_COUNT)));
}

// // =========================================================
// //  Controls
// =========================================================


void move_player(Vector2 *u, Vector2 v, float dt)
{
    *u = Vector2Add(*u, Vector2Scale(v, dt));
}
int in_bounds(Player *player, int* map, Vector2 subdivisions, Vector2 world_tile_size)
{ 
    int x = player->position.x / world_tile_size.x;
    int y = player->position.y / world_tile_size.y;
    int tile = get_tile(map, x, y, subdivisions);
    return (tile == EMPTY);
}

void update_player_orientation(Player* player, Vector2 velocity) 
{
    // Only update angle if the player is actually moving
    if (velocity.x != 0 || velocity.y != 0) {
        // atan2f gives the angle in radians for any vector
        player->angle_radians = atan2f(velocity.y, velocity.x);
        
        // Forward vector is a simple unit vector based on that angle
        player->forward_v.x = cosf(player->angle_radians);
        player->forward_v.y = sinf(player->angle_radians);
    }
}

void move_and_slide(Player *p, Vector2 velocity, int* map, Vector2 subdivisions, Vector2 world_tile_size)
{
    // --- 1. RESOLVE X-AXIS ---
    float next_x = p->position.x + velocity.x;
    
    // Determine which edge to check based on direction
    // If moving right (pos), check the right edge (x + width). 
    // If moving left (neg), check the left edge (x).
    float check_x = (velocity.x > 0) ? (next_x + p->size.x) : next_x;

    // Check both the top and bottom corners of that leading edge
    int tile_x_top = get_tile(map, (int)(check_x / world_tile_size.x), (int)(p->position.y / world_tile_size.y), subdivisions);
    int tile_x_bottom = get_tile(map, (int)(check_x / world_tile_size.x), (int)((p->position.y + p->size.y - 1) / world_tile_size.y), subdivisions);

    if (tile_x_top == EMPTY && tile_x_bottom == EMPTY) 
    {
        p->position.x = next_x;
    }

    // --- 2. RESOLVE Y-AXIS ---
    float next_y = p->position.y + velocity.y;
    
    // If moving down (pos), check bottom edge (y + height). 
    // If moving up (neg), check top edge (y).
    float check_y = (velocity.y > 0) ? (next_y + p->size.y) : next_y;

    // Check both the left and right corners of that leading edge
    int tile_y_left = get_tile(map, (int)(p->position.x / world_tile_size.x), (int)(check_y / world_tile_size.y), subdivisions);
    int tile_y_right = get_tile(map, (int)((p->position.x + p->size.x - 1) / world_tile_size.x), (int)(check_y / world_tile_size.y), subdivisions);

    if (tile_y_left == EMPTY && tile_y_right == EMPTY) 
    {
        p->position.y = next_y;
    }
}

void controls(Player *p, int* map, Vector2 subdivisions, Vector2 world_tile_size, float move_speed)
{
    float dt = GetFrameTime();
    Vector2 vel = { 0, 0 };

    if (IsKeyDown(KEY_W)) vel.y -= 1;
    if (IsKeyDown(KEY_S)) vel.y += 1;
    if (IsKeyDown(KEY_A)) vel.x -= 1;
    if (IsKeyDown(KEY_D)) vel.x += 1;

    if (vel.x != 0 || vel.y != 0) {
        vel = Vector2Scale(Vector2Normalize(vel), move_speed * dt);
        update_player_orientation(p, vel);
        move_and_slide(p, vel, map, subdivisions, world_tile_size);
    }

    if (IsKeyPressed(KEY_R)) p->position = get_screen_center();
}


// // =========================================================
// //  Camera
// // =========================================================

void manage_scissor_camera(Rectangle *scissor_rect, Camera2D *cam, int *map, Vector2 subdivisions, Player *player, Color c, Vector2 world_tile_size)
{
    BeginScissorMode(scissor_rect->x, scissor_rect->y,
                     scissor_rect->width, scissor_rect->height);
    BeginMode2D(*cam);
    draw_map_2D(map, subdivisions.x, subdivisions.y, world_tile_size);
    draw_player(player, c);
    static int get_color = 0;
    static Color forwardv_color;
    if (get_color == 0) {
        forwardv_color = get_random_color();
        get_color = 1;
    }
    draw_player_forwardv(player, forwardv_color);
    EndMode2D();
    EndScissorMode();
}