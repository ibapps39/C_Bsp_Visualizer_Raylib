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
