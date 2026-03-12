```c
BeginDrawing()
  BeginScissorMode (/*optional*/)
    BeginMode2D()
      // Draw world
    EndMode2D()
  EndScissorMode (/*if begun*/)
EndDrawing()
```

// LEFT VIEW, Repeat for upper_right and lower_right. That gives you split-screen rendering.
```c
BeginScissorMode(left_half.x, left_half.y,
                 left_half.width, left_half.height);
BeginMode2D(cam);
draw_map(map, 8, 8);
EndMode2D();
EndScissorMode();
```
<b>Always pair BeginScissorMode() with EndScissorMode()
<b>Put scissor outside BeginMode2D()

// Manages the scissor views
```c
void manage_scissor_camera(Rectangle *scissor_rect, Camera2D *cam, int *map, Vector2 subdivisions, Player *player, Color c, Vector2 world_tile_size, int id, Vector4 *rdda)
```

// maybe add
Your `sample_texture()` should treat `u` and `v` as **normalized texture coordinates** in `[0,1]`. Then generate patterns mathematically. Since your DDA already returns the hit position, you’ll compute `u` outside and pass it in; `v` is the vertical position along the wall column when drawing.

Below is a **complete procedural sampler** for **brick, stone, and wood**.

```c
Color sample_texture(int type, float u, float v)
{
    // clamp for safety
    if (u < 0) u = 0; if (u > 1) u = 1;
    if (v < 0) v = 0; if (v > 1) v = 1;

    switch(type)
    {

        // ======================
        // BRICK
        // ======================
        case 0:
        {
            float brick_u = u * 4.0f;   // bricks per row
            float brick_v = v * 8.0f;   // rows

            float mortar_thickness = 0.05f;

            float fu = fmodf(brick_u, 1.0f);
            float fv = fmodf(brick_v + (floorf(brick_u) * 0.5f), 1.0f);

            if (fu < mortar_thickness || fv < mortar_thickness)
            {
                return (Color){180,180,180,255}; // mortar
            }

            return (Color){150,40,40,255}; // brick
        }


        // ======================
        // STONE
        // ======================
        case 1:
        {
            float freq = 10.0f;

            float noise =
                sinf(u * freq) *
                sinf(v * freq);

            noise = (noise + 1.0f) * 0.5f;

            int c = 100 + noise * 80;

            return (Color){c,c,c,255};
        }


        // ======================
        // WOOD
        // ======================
        case 2:
        {
            float rings = 30.0f;

            float r = sqrtf(u*u + v*v);

            float grain = sinf(r * rings);

            grain = (grain + 1.0f) * 0.5f;

            int rcol = 110 + grain * 60;
            int gcol = 70  + grain * 40;
            int bcol = 30;

            return (Color){rcol,gcol,bcol,255};
        }

        default:
            return WHITE;
    }
}
```

---

# How to Compute `u` (important)

Inside your **ray column rendering**, compute `u` depending on which wall side was hit.

You already have:

```
Vector4 result
x = hitx
y = hity
z = dist
w = side
```

Use:

```c
float u;

if(v.w == 0) // vertical wall
    u = fmodf(v.y, tile_size) / tile_size;
else         // horizontal wall
    u = fmodf(v.x, tile_size) / tile_size;
```

---

# How to Compute `v`

When drawing the vertical wall column:

```c
float v = (y - wall_top) / wall_height;
```

Then sample:

```c
Color tex = sample_texture(0, u, v);
```

And draw pixel:

```c
DrawPixel(column_x, y, tex);
```

---

# Why This Works

Procedural textures use **math patterns instead of images**:

| Texture | Function                 |
| ------- | ------------------------ |
| Brick   | `fmod()` grid            |
| Stone   | `sin()` noise            |
| Wood    | `sin(sqrt(x²+y²))` rings |

Old engines did similar tricks before GPU textures were common.

---

# Small Upgrade (Looks WAY Better)

Add cheap noise:

```c
float noise = sinf(u*53.0f)*sinf(v*97.0f);
```

and mix it into colors.

This makes stone/wood look **much less flat**.

---

# Result

Your raycaster now supports **infinite procedural textures** without loading images.

Which is actually a really cool feature for an **educational renderer**.

---

If you want, I can also show you a **much cooler trick**:

**procedural textures that align perfectly with the grid and never stretch**, which is something most beginner raycasters accidentally break.
