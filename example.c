#include <stdio.h>
#include <stdbool.h>

#define SUI_IMPL
#include "sui.h"

#ifdef __unix__
#include <unistd.h>
int usleep(int usec);
#endif

int main() {
  {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(100, 100, "ui");
    SetTargetFPS(60);

    int monitor = GetCurrentMonitor();
    int w = 0.8 * GetMonitorWidth(monitor),
        h = 0.8 * GetMonitorHeight(monitor);
    SetWindowSize(w, h);
  }

  SUI_Ctx ctx = {0};
  sui_ctx_init(&ctx);

  while (!WindowShouldClose()) {
    sui_ctx_update(&ctx);

    BeginDrawing();
    ClearBackground(WHITE);

    sui_do_panel(&ctx, 0.5, 0.5, 0.8, 0.5);

      sui_do_text(
        &ctx,
        "You won! ... or maybe not",
        0.5, 0.2,
        ctx.title_font,
        WHITE
      );

      if (sui_do_button(&ctx, "No op", 0.5, 0.5, 0.4, 0.2));
      if (sui_do_button_next(&ctx, "Exit", 0.02, 0.4, 0.2)) {
        exit(1);
      }

    sui_ctx_pop(&ctx);

    EndDrawing();

    #ifdef __unix__
    usleep(1000000 / 100);
    #endif
  }

  return 0;
}
