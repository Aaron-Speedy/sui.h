# sui.h
> [!WARNING]
> This library is unfinished. Keep your expectations low.

sui.h is a simple, do-it-yourself, single-header, immediate mode gui library.

Note: This library depends on Raylib. This dependency is planned to be removed
in the future.

## Usage
```c
#define SUI_IMPL
#include "sui.h"

int main() {
  sui_init_window("Example Menu", 0.8, 0.8, 60);
  SUI_Ctx ctx = {
    .reg_font = { .path = "recs/Daydream.ttf", .size = 0.03, },
    .title_font = { .path = "recs/Daydream.ttf", .size = 0.04, }
  };
  sui_ctx_init(&ctx);

  while (!WindowShouldClose()) {
    sui_ctx_update(&ctx);
    BeginDrawing();
    ClearBackground(WHITE);

    // x, y, width, height
    sui_do_panel(&ctx, 0.5, 0.5, 0.8, 0.5);
      // x, y, font, color
      sui_do_text(&ctx, "Example Menu", 0.5, 0.2, ctx.title_font, WHITE);
      // text, x, y, width, height
      sui_do_button(&ctx, "No op", 0.5, 0.5, 0.4, 0.2);
      // text, spacing, x, y
      if (sui_do_button_next(&ctx, "Exit", 0.02, 0.4, 0.2)) exit(0);
    sui_ctx_pop(&ctx);

    EndDrawing();
  }

  return 0;
}
```
To compile this, you need to link with Raylib.
To do this on POSIX systems, first install Raylib, then do:
```sh
cc example.c -o example -lraylib
```

To change functionality or styling, simply edit or add any ui_do functions.
