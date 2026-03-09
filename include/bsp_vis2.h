#pragma once

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h> // Required for malloc/free
#include <stdio.h>

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

typedef enum SCREENS
{
    RENDER,
    RAY,
    TREE
} SCREENS;

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
//  HELPERS
// =========================================================
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
float increment_angle(float *radians, float amt)
{
    amt = fmod(amt, 2 * M_PI);
    amt = amt < 0 ? amt + 2 * M_PI : amt;
    *radians = amt;
    return *radians;
}
void update_player_angle(Player *player, float radians)
{
    player->rad_angle += radians;
}
Vector2 get_player_center(Player *player)
{
    return (Vector2){player->position.x + player->size.x / 2.0f, player->position.y + player->size.y / 2.0f};
}
Camera2D init_cam(Vector2 pos)
{
    Camera2D cam;
    cam.target = pos;
    cam.offset = pos;
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    return cam;
}
// =========================================================
//  HELPERS
// =========================================================
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
Vector4 dda(int *map, int map_width, int map_height, Vector2 ro, float rads, float world_space_units, int max_dof)
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
            side_dist_x += dx;
            // This is the perpendicular distance to the wall plane
            hit_dist = side_dist_x - dx; 
        }
        else
        {
            side = 1;
            mapy += step_y;
            side_dist_y += dy;
            // This is the perpendicular distance to the wall plane
            hit_dist = side_dist_y - dy;
        }

        // Boundary check
        if (mapx < 0 || mapx >= map_width || mapy < 0 || mapy >= map_height)
        {
            return (Vector4){.x = -1, .y = -1, .z = -1, .w = -1};
        }

        if (map[mapx + mapy * map_width] == WALL)
        {
            hit = 1;
        }
        dof++;
    }

    // Since hit_dist was calculated in tile-units, scale it to world space once at the end
    float final_perp_dist = hit_dist * world_space_units;

    // Use final_perp_dist for the hit coordinates so they stay on the wall
    hitx = ro.x + (final_perp_dist * dirx);
    hity = ro.y + (final_perp_dist * diry);

    return (Vector4){.x = hitx, .y = hity, .z = final_perp_dist, .w = (float)side};
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

void controls(Player *p, float dt, Vector4 *ray)
{
    dt = dt * GetFrameTime();
    float rads = 0.1f;
    Vector2 next = {0};

    if (IsKeyDown(KEY_A))
    {
        controls_ang(p, rads, dt);
    }

    if (IsKeyDown(KEY_D))
    {
        controls_ang(p, -rads, dt);
    }

    if (IsKeyDown(KEY_W))
    {
        if (ray->z > 2)
        {
            next.x = (p->player_angle.x / 8) * 20;
            next.y = (p->player_angle.y / 8) * 20;
        }

        p->position.x += next.x;
        p->position.y += next.y;
    }

    if (IsKeyDown(KEY_S))
    {
        if (ray->z > 2)
        {
            next.x = -(p->player_angle.x / 8) * 20;
            next.y = -(p->player_angle.y / 8) * 20;
        }

        p->position.x += next.x;
        p->position.y += next.y;
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

Vector4 dda_fov_i(Player *player, int map_size, int *map, int num_rays, float fov_degrees, int tile_size, int max_dof, int i)
{
    float fov_rad = fov_degrees * DEG2RAD;
    // Calculate the specific angle for this ray
    float rayi_angle = (player->rad_angle - (fov_rad / 2.0f)) + (i * fov_rad / (float)num_rays);
    
    Vector4 result = dda(map, 8, 8, player->position, rayi_angle, tile_size, max_dof);
    
    // --- THE FISHEYE FIX ---
    // Multiply distance by the cosine of the relative angle
    float relative_angle = rayi_angle - player->rad_angle;
    result.z *= cosf(relative_angle); 
    
    return result;
}

void draw_dda_topdown(Vector2 player, Vector2 v)
{
    DrawLineEx(player, v, 3.0f, RED);
}

void draw_dda_fp_at(int ray_index, int num_rays, Vector4 v, int screen_w, int screen_h, float sx, float sy)
{
    if (v.z <= 0.01f) return;

    float col_w = (float)screen_w / (float)num_rays;
    float col_x = sx + (ray_index * col_w); // Add scissor X offset

    float wall_h = (64.0f * screen_h) / v.z; // Use tile size (64) to scale height

    float wall_top = sy + (screen_h * 0.5f) - (wall_h * 0.5f);
    float wall_bottom = sy + (screen_h * 0.5f) + (wall_h * 0.5f);

    // Ceiling and Floor
    DrawRectangle(col_x, sy, col_w + 1, (screen_h * 0.5f) - (wall_h * 0.5f), SKYBLUE);
    DrawRectangle(col_x, wall_bottom, col_w + 1, sy + screen_h - wall_bottom, DARKGRAY);

    // Wall column
    Color c = (v.w == 0) ? BLUE : (Color){0, 0, 150, 255};
    DrawRectangle(col_x, wall_top, col_w + 1, wall_h, c);
}

void manage_scissor_camera(Rectangle *scissor_rect, Camera2D *cam, int *map, Vector2 subdivisions, Player *player, Color c, Vector2 world_tile_size, int id, Vector4 *rdda)
{
    BeginScissorMode(scissor_rect->x, scissor_rect->y, scissor_rect->width, scissor_rect->height);

    if (id == RENDER)
    {
        // 3D Rendering - Draw columns directly to the scissor area
        for (int i = 0; i < 60; i++)
        {
            Vector4 ray_i = dda_fov_i(player, 64, map, 60, 60, 64, 8, i);
            *rdda = ray_i; 
            
            // Draw column relative to scissor_rect.x and y
            float col_w = scissor_rect->width / 60.0f;
            float col_x = scissor_rect->x + (i * col_w);
            
            // Call your FP draw function but pass the calculated X
            draw_dda_fp_at(i, 60, ray_i, scissor_rect->width, scissor_rect->height, scissor_rect->x, scissor_rect->y);
        }
    }
    else 
    {
        // 2D Rendering - Use the Camera
        BeginMode2D(*cam);
        draw_map_2D(64, 8, 8, map); // Draw map once
        draw_player(player);

        for (int i = 0; i < 60; i++)
        {
            Vector4 ray_i = dda_fov_i(player, 64, map, 60, 60, 64, 8, i);
            Vector2 rayi_v2 = (Vector2){.x = ray_i.x, .y = ray_i.y};
            
            if (id == RAY) draw_dda_topdown(player->position, rayi_v2);
        }
        EndMode2D();
    }

    EndScissorMode();
}