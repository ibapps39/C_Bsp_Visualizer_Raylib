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
#define PI_2 M_PI * 2
#define PI_3H M_PI * 3 / 2
#define PI_H M_PI / 2
#define TRUE 1
#define FALSE 0
#define WALL 1
#define EMPTY 0
#define HIT 1

// =========================================================
//  NEON COLORS
// =========================================================
#define NEON_GREEN (Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define NEON_BLUE (Color){.r = 0, .g = 0, .b = 255, .a = 255}
#define NEON_RED (Color){.r = 255, .g = 0, .b = 0, .a = 255}
#define NEON_YELLOW (Color){.r = 255, .g = 255, .b = 0, .a = 255}
#define NEON_PINK (Color){.r = 255, .g = 0, .b = 255, .a = 255}
#define NEON_PURPLE (Color){.r = 255, .g = 0, .b = 255, .a = 255}
#define NEON_COLOR_COUNT 6 - 1

typedef struct Player
{
    float rad_angle;
    float x_offset;
    float y_offset;
    float speed;
    float rotation_speed;
    Vector2 position;
    Vector2 size;
    Vector2 player_angle;
    Color color;
} Player;

typedef struct ray
{
    Vector2 origin;
    Vector2 direction;
    float ro; // t
} ray;

void draw_map_2D(int m_size, int m_w, int m_h, int *map)
{
    for (int y = 0; y < m_h; y++)
    {
        for (int x = 0; x < m_w; x++)
        {
            Color c = map[y * m_w + x] == 1 ? BLUE : WHITE;
            DrawRectangle(x * m_size, y * m_size, m_size - 1, m_size - 1, c);
        }
    }
}

int get_tile(int col, int row, int *map, int size)
{
    return map[row * size + col];
}

// DDA - everything in tile space until end
Vector3 dda(int *map, int map_width, int map_height, Vector2 ro, float rads, float world_space_units, int max_dof)
{
    float origin_x = ro.x / world_space_units;
    float origin_y = ro.y / world_space_units;
    int mapx, mapy; // where we are in the map, tile_space
    int side = 0;
    int step_x, step_y;
    int hit = 0;
    mapx = (int)(origin_x);
    mapy = (int)(origin_y);

    // must be converted to tile space
    float dirx, diry;
    float dx, dy;
    float side_dist_x, side_dist_y;
    float hit_dist = 0.0f;
    float hitx = 0.0f;
    float hity = 0.0f;

    dirx = cosf(rads);
    diry = sinf(rads);

    dx = (fabsf(dirx) < EPSILON) ? INFINITY : fabsf(1.0f / dirx);
    dy = (fabsf(diry) < EPSILON) ? INFINITY : fabsf(1.0f / diry);

    step_x = (dirx < 0) ? -1 : 1;
    step_y = (diry < 0) ? -1 : 1;

    if (dirx < 0)
    {
        side_dist_x = (origin_x - mapx) * dx;
    }
    else
    {
        side_dist_x = ((mapx + 1) - origin_x) * dx;
    }
    if (diry < 0)
    {
        side_dist_y = (origin_y - mapy) * dy;
    }
    else
    {
        side_dist_y = ((mapy + 1) - origin_y) * dy;
    }

    int dof = 0;
    while (dof < max_dof && !hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side = 0;
            mapx += step_x;
            hit_dist = side_dist_x;
            side_dist_x += dx;
        }
        else
        {
            side = 1;
            mapy += step_y;
            hit_dist = side_dist_y;
            side_dist_y += dy;
        }
        if (mapx < 0 || mapx >= map_width || mapy < 0 || mapy >= map_height)
        {
            hit = 1;
            return (Vector3){.x = -1, .y = -1, .z = -1};
        }
        if (map[mapx + mapy * map_width] == WALL)
        {
            hit = 1;
        }
        dof++;
    }
    // Convert back to world space
    hit_dist *= world_space_units;
    hitx = ro.x + (hit_dist * dirx);
    hity = ro.y + (hit_dist * diry);
    Vector3 hit_point = {.x = hitx, .y = hity, .z = hit_dist};
    return hit_point;
}

void set_player_offset(Player *player, float x_o, float y_o)
{
    player->x_offset = x_o;
    player->y_offset = y_o;
}

void draw_player_offset(Player *player, float line_thickness)
{
    Vector2 start = {.x = player->position.x + player->size.x / 2, .y = player->position.y + player->size.y / 2};
    Vector2 pda = {.x = cosf(player->rad_angle) * 20, .y = sinf(player->rad_angle) * 20};
    Vector2 end = Vector2Add(player->position, pda);
    DrawRectangle(end.x, end.y, 5, 5, RED);
    DrawLineEx(player->position, end, line_thickness, RED);
}

void draw_player(Player *player)
{
    Vector2 center = {.x = player->position.x - player->size.x / 2, .y = player->position.y - player->size.y / 2};
    DrawRectangleV(center, player->size, player->color);
}

Vector2 get_next(Vector2 current, float angle)
{
    Vector2 next = {.x = current.x + cosf(angle), .y = current.y + sinf(angle)};
    return next;
}

float controls_ang(Player *p, float rads, float dt)
{
    p->rad_angle += rads;
    const int gate = rads > 0 ? TRUE : FALSE;
    switch (gate)
    {
    case TRUE:
        if (p->rad_angle > PI_2)
            p->rad_angle -= PI_2;
        p->player_angle.x = cosf(p->rad_angle);
        p->player_angle.y = sinf(p->rad_angle);
        break;
    case FALSE:
        if (p->rad_angle < 0)
            p->rad_angle += PI_2;
        p->player_angle.x = cosf(p->rad_angle);
        p->player_angle.y = sinf(p->rad_angle);
        break;
    default:
        break;
    }
}

void controls(Player *p, float dt)
{
    float rads = 0.1;
    Vector2 next;
    if (IsKeyDown(KEY_A))
    {
        controls_ang(p, rads, dt);
    }
    if (IsKeyDown(KEY_D))
    {
        int dir = -1;
        controls_ang(p, dir * rads, dt);
    }
    if (IsKeyDown(KEY_W))
    {
        p->position.x += (p->player_angle.x / 8) * 20;
        p->position.y += (p->player_angle.y / 8) * 20;
    }
    if (IsKeyDown(KEY_S))
    {
        p->position.x -= (p->player_angle.x / 8) * 20;
        p->position.y -= (p->player_angle.y / 8) * 20;
    }
}

void init_player(Player *p, int map_size, int *map)
{
    float px = GetScreenWidth() / 2;
    float py = GetScreenHeight() / 2;
    px /= map_size;
    py /= map_size;
    // check if position is valid and not a wall or out of bounds
    if (get_tile(px, py, map, map_size) == 1)
    {
        for (size_t y = 0; y < map_size; y++)
        {
            for (size_t x = 0; x < map_size; x++)
            {
                if (get_tile(x, y, map, map_size) == 0)
                {
                    px = x;
                    py = y;
                    break;
                }
            }
        }
    }

    *p = (Player){
        .rad_angle = 0.0f,
        .x_offset = 0.0f,
        .y_offset = 0.0f,
        .speed = 200.0f, // pixels
        .rotation_speed = 0.1f,
        .position = {.x = (GetScreenWidth() / 2), .y = (GetScreenHeight() / 2)},
        .size = {.x = (1.0f / 4.0f) * map_size, .y = (1.0f / 3.0f) * map_size},
        .player_angle = {.x = cosf(1), .y = sinf(0)},
        .color = YELLOW};
    p->x_offset = p->position.x / 2 + p->size.x * 2;
    p->y_offset = p->position.y / 2 + p->size.y * 2;
}

Vector3 dda_fov_i(Player *player, int map_size, int *map, int num_rays, float fov_degrees, int tile_size, int max_dof, int i)
{
    // Convert FOV to Radians
    float fov_rad = fov_degrees * DEG2RAD;
    float rayi_angle = (player->rad_angle - (fov_rad / 2.0f)) + (i * fov_rad / num_rays);
    Vector3 result = dda(map, 8, 8, player->position, rayi_angle, tile_size, max_dof);
    return result;
}

void draw_dda_topdown(Vector2 player, Vector2 v)
{
    DrawLineEx(player, v, 3.0f, RED);
}
#include <stdio.h>
void draw_dda_fp(Vector2 player, Vector3 v)
{
    float line_h = (v.z < 2) ? GetScreenHeight() : 1.0f/v.z;
    Vector2 start = {.x = v.x, .y = v.y};
    Vector2 end = {.x = v.x, .y = v.y + line_h};
    DrawLineEx(start, end, 10.0f, RED);
    printf("x: %.2f, y: %.2f, z: %.2f\n", v.x, v.y, v.z);
    
}
void draw(Player *player, int map_size, int *map, int num_rays, float fov_degrees, int max_dof)
{
    //draw_map_2D(map_size, 8, 8, map);
    //draw_player(player);
    //draw_player_offset(player, 3);
    float tile_size = 8 * 8; // map_w * map_h
    for (size_t i = 0; i < num_rays; i++)
    {
        Vector3 ray_i = dda_fov_i(player, map_size, map, num_rays, fov_degrees, tile_size, max_dof, i);
        
        Vector2 rayi_v2 = (Vector2){.x = ray_i.x, .y = ray_i.y};
        //draw_dda_topdown(player->position, rayi_v2);
        draw_dda_fp(player->position, ray_i);
    }
    
    
}