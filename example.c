#define SUI_IMPL
#include "sui.h"

int main() {
  sui_init_window("example", 0.8, 0.8, 60);
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
  }

  return 0;
}
