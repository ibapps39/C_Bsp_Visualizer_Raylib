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
#define DEFAULT_map_subdivisionsINESIONS (DEFAULT_MAP_SIZE_X * DEFAULT_MAP_SIZE_Y)
#define DEFAULT_MAP_VECTOR2D \
    (Vector2) { DEFAULT_MAP_SIZE_X, DEFAULT_MAP_SIZE_Y }
#define DEFAULT_PLAYER_COLOR (Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define DEFAULT_PLAYER_SIZE \
    (Vector2) { .x = .0333 * DEFAULT_WINDOW_X, .y = .0667 * DEFAULT_WINDOW_Y }
#define DEFAULT_RESOLUTION ((int)DEFAULT_WINDOW_X * (int)DEFAULT_WINDOW_Y)
#define DEFAULT_WINDOW_X 600
#define DEFAULT_WINDOW_Y 700
#define DEFAULT_GRID_SQUARE_LW 8

// =========================================================
//  NEON COLORS
// =========================================================
#define NEON_GREEN (Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define NEON_BLUE (Color){.r = 0, .g = 0, .b = 255, .a = 255}
#define NEON_RED (Color){.r = 255, .g = 0, .b = 0, .a = 255}
#define NEON_YELLOW (Color){.r = 255, .g = 255, .b = 0, .a = 255}
#define NEON_PINK (Color){.r = 255, .g = 0, .b = 255, .a = 255}
#define NEON_PURPLE (Color){.r = 255, .g = 0, .b = 255, .a = 255}

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
    float angle_degrees;
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


int get_tile(int *map, int x, int y, Vector2 subdivisions) {
    // Safety: check bounds before array access
    if (x < 0 || y < 0 || x >= subdivisions.x || y >= subdivisions.y) return -1; 
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
Player init_player(Vector2 pos, Vector2 speed, Vector2 forward_v, Vector2 size, float angle_radians, float angle_degrees)
{
    Player player;
    player.position = pos;
    player.speed = speed;
    player.forward_v = forward_v;
    player.size = size;
    player.angle_radians = angle_radians;
    player.angle_degrees = angle_degrees;
    return player;
}

// // =========================================================
// //  State
// // =========================================================

// // =========================================================
// //  Drawing
// // =========================================================


void draw_map(int *map, int array_subdivison_width, int array_subdivision_height)
{
    int max_tiles = array_subdivison_width * array_subdivision_height;

    int tileWidth  = GetScreenWidth() / array_subdivison_width;
    int tileHeight = GetScreenHeight() / array_subdivision_height;

    for (int i = 0; i < max_tiles; i++)
    {
        int x = i % array_subdivison_width;
        int y = i / array_subdivison_width;

        Color color = (map[i] == WALL) ? BLUE : DARKGRAY;

        DrawRectangle(
            x * tileWidth,
            y * tileHeight,
            tileWidth - 1,
            tileHeight - 1,
            color
        );
    }
}

void draw_player(Player* player, Color c)
{
    Rectangle r = {player->position.x, player->position.y, player->size.x, player->size.y};
    DrawRectangleRec(r, c);
}
void draw_ray(Ray ray, Vector2 end, Color c)
{
    DrawLine(ray.position.x, ray.position.y, end.x, end.y, c);
}

Rectangle update_rectangle(Rectangle r, float x, float y, float width, float height)
{
            return r = (Rectangle){
            .x = x,
            .y = y,
            .width = width,
            .height = height
        };
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
    return get_neon_color((Neon_Colors)(GetRandomValue(0, 6)));
}

// // =========================================================
// //  Controls
// =========================================================

Vector2 get_forwardv(Player *player, Vector2 target)
{
    Vector2 dir = Vector2Subtract(target, player->position);
    return Vector2Normalize(dir);
}

void move_player(Vector2 *u, Vector2 v, float dt)
{
    *u = Vector2Add(*u, Vector2Scale(v, dt));
}
void handle_bounds(Vector2 *u, Vector2 v, float dt)
{
    // if (touching_wall())
    // {
    //     /* code */
    // }
}


void controls(Player* p)
{
    float speedValue = 200.0f; // Pixels per second
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
     p->position.y -= speedValue * GetFrameTime();  
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        p->position.y += speedValue * GetFrameTime();
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        p->position.x -= speedValue * GetFrameTime();
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        p->position.x += speedValue * GetFrameTime();
    }

    if (IsKeyPressed(KEY_R))
    {
        p->position = (Vector2){
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2
        };
    }
}
