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

    sui_do_panel(&ctx, 0.5, 0.5, 0.8, 0.5);
      sui_do_text(&ctx, "Example menu", 0.5, 0.2, ctx.title_font, WHITE);
      sui_do_button(&ctx, "No op", 0.5, 0.5, 0.4, 0.2);
      if (sui_do_button_next(&ctx, "Exit", 0.02, 0.4, 0.2)) exit(0);
    sui_ctx_pop(&ctx);

    EndDrawing();
  }

  return 0;
}
