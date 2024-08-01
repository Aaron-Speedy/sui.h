/* sui.h - v0.1
  This is a single header file for doing immediate mode GUIs.

  See end of file for licensing.
*/

#ifndef SUI_H
#define SUI_H

#include <raylib.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __unix__
#include <unistd.h>
int usleep(int usec);
#endif

typedef struct {
  Vector2 pos;
  Vector2 size;
} SUI_Rect;

typedef struct {
  Font f;
  char *path;
  float size; // size * GetRenderWidth(). fontSize from LoadFontEx(...)
} SUI_Font;

typedef struct {
  int count, cap;
  SUI_Rect *items;
  SUI_Rect last_do;
  SUI_Font reg_font;
  SUI_Font title_font;
} SUI_Ctx;

// Width and height are relative to monitor
void sui_init_window(char *title, float w, float h, int target_fps);

void sui_do_text(SUI_Ctx *ctx, char *t, float x, float y, SUI_Font f, Color c);
bool sui_do_button(SUI_Ctx *ctx, char *t, float x, float y, float w, float h);
bool sui_do_button_next(SUI_Ctx *ctx, char *t, float spacing, float w, float h);
void sui_do_panel(SUI_Ctx *ctx, float x, float y, float w, float h);

void sui_ctx_init(SUI_Ctx *ctx);
void sui_ctx_update(SUI_Ctx *ctx);
void sui_ctx_push(SUI_Ctx *ctx, SUI_Rect rect);
SUI_Rect *sui_ctx_pop(SUI_Ctx *ctx);
SUI_Rect *sui_ctx_last(SUI_Ctx *ctx);

// Internal, but you can use these externally if you want
Vector2 sui_place_rel(SUI_Ctx *ctx, float x, float y, Vector2 size);
SUI_Rect sui_rel_to_abs(SUI_Ctx *ctx, float x, float y, float w, float h);
SUI_Rect sui_abs_to_rel(SUI_Ctx * ctx, SUI_Rect rect);
Rectangle sui_rect_to_rectangle(SUI_Rect rect);
bool sui_mouse_in(float x, float y, float w, float h);
bool sui_mouse_in_rec(SUI_Rect rect);

#ifdef SUI_IMPL

void sui_init_window(char *title, float w, float h, int target_fps) {
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(100, 100, title);
  SetTargetFPS(target_fps);

  int monitor = GetCurrentMonitor();
  int width = w * GetMonitorWidth(monitor),
      height = h * GetMonitorHeight(monitor);
  SetWindowSize(width, height);
}

void sui_do_text(SUI_Ctx *ctx, char *t, float x, float y, SUI_Font f, Color c) {
  SUI_Rect rect = { .size = MeasureTextEx(f.f, t, f.f.baseSize, 1) };
  rect.pos = sui_place_rel(ctx, x, y, rect.size);

  DrawTextEx(f.f, t, rect.pos, f.f.baseSize, 1, c);

  ctx->last_do = sui_abs_to_rel(ctx, rect);
}

bool sui_do_button(SUI_Ctx *ctx, char *t, float x, float y, float w, float h) {
  SUI_Rect rect = sui_rel_to_abs(ctx, x, y, w, h);

  Color button_color = WHITE;
  Color text_color = (Color) { 20, 62, 128, 255, };

  if (sui_mouse_in_rec(rect)) {
    float factor = 0.75;
    button_color.r *= factor;
    button_color.g *= factor;
    button_color.b *= factor;
  }

  DrawRectangleRounded(sui_rect_to_rectangle(rect), 0.1, 20, button_color);

  sui_ctx_push(ctx, rect);
  {
    sui_do_text(ctx, t, 0.5, 0.5, ctx->reg_font, text_color);
  }
  sui_ctx_pop(ctx);

  ctx->last_do = sui_abs_to_rel(ctx, rect);

  return sui_mouse_in_rec(rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool sui_do_button_next(SUI_Ctx *ctx, char *t, float spacing, float w, float h) {
  return sui_do_button(
    ctx,
    t,
    ctx->last_do.pos.x,
    ctx->last_do.pos.y + ctx->last_do.size.y + spacing,
    w, h
  );
}

void sui_do_panel(SUI_Ctx *ctx, float x, float y, float w, float h) {
  SUI_Rect rect = sui_rel_to_abs(ctx, x, y, w, h);
  sui_ctx_push(ctx, rect);

  Color panel_color = { 20, 62, 128, 255, };
  DrawRectangleRounded(sui_rect_to_rectangle(rect), 0.1, 20, panel_color);

  ctx->last_do = sui_abs_to_rel(ctx, rect);
}

void sui_ctx_init(SUI_Ctx *ctx) {
  assert(ctx->items == NULL);

  if (ctx->cap <= 0) ctx->cap = 256;
  ctx->items = malloc(ctx->cap * sizeof(ctx->items[0]));

  sui_ctx_push(ctx, (SUI_Rect){0});
  sui_ctx_update(ctx);
}

void sui_ctx_update(SUI_Ctx *ctx) {
  if (IsWindowResized()) {
    ctx->items[0].size.x = GetRenderWidth();
    ctx->items[0].size.y = GetRenderHeight();

    UnloadFont(ctx->reg_font.f);
    UnloadFont(ctx->title_font.f);

    ctx->reg_font.f = LoadFontEx(
      ctx->reg_font.path, 
      ctx->reg_font.size * GetRenderWidth(), 0, 0
    ),
    ctx->title_font.f = LoadFontEx(
      ctx->title_font.path,
      ctx->title_font.size * GetRenderWidth(), 0, 0
    );
  }

  #ifdef __unix__
  usleep(1000000 / 100);
  #endif
}

void sui_ctx_push(SUI_Ctx *ctx, SUI_Rect rect) {
  assert(ctx->cap > 0);
  assert(ctx->count >= 0);
  assert(ctx->items != NULL);

  if (ctx->count >= ctx->cap) {
    ctx->cap *= 2;
    ctx->items = realloc(ctx->items, ctx->cap * sizeof(ctx->items));
  }

  ctx->items[ctx->count++] = rect;
}

SUI_Rect *sui_ctx_pop(SUI_Ctx *ctx) {
  assert(ctx->count > 0);
  assert(ctx->items != NULL);

  ctx->count -= 1;
  return &ctx->items[ctx->count];
}

SUI_Rect *sui_ctx_last(SUI_Ctx *ctx) {
  assert(ctx->count > 0);
  assert(ctx->items != NULL);
  return &ctx->items[ctx->count - 1];
}

Vector2 sui_place_rel(SUI_Ctx *ctx, float x, float y, Vector2 size) {
  SUI_Rect space = *sui_ctx_last(ctx);
  return (Vector2) {
    .x = space.pos.x + x * space.size.x - size.x/2,
    .y = space.pos.y + y * space.size.y - size.y/2,
  };
}

SUI_Rect sui_rel_to_abs(SUI_Ctx *ctx, float x, float y, float w, float h) {
  SUI_Rect rect = {
    .size = {
      .x = w * sui_ctx_last(ctx)->size.x,
      .y = h * sui_ctx_last(ctx)->size.y,
    },
  };
  rect.pos = sui_place_rel(ctx, x, y, rect.size);
  return rect;
}

SUI_Rect sui_abs_to_rel(SUI_Ctx *ctx, SUI_Rect abs) {
  SUI_Rect space = *sui_ctx_last(ctx);

  SUI_Rect rect = {
    .size = {
      .x = abs.size.x / space.size.x,
      .y = abs.size.y / space.size.y,
    },
  };

  rect.pos.x = (abs.pos.x - space.pos.x + abs.size.x/2) / space.size.x;
  rect.pos.y = (abs.pos.y - space.pos.y + abs.size.y/2) / space.size.y;

  return rect;
}

Rectangle sui_rect_to_rectangle(SUI_Rect rect) {
  return (Rectangle) {
    .x = rect.pos.x,
    .y = rect.pos.y,
    .width = rect.size.x,
    .height = rect.size.y,
  };
}

bool sui_mouse_in(float x, float y, float w, float h) {
  Vector2 v = GetMousePosition();
  if (v.x < x) return 0;
  if (v.y < y) return 0;
  if (v.x > x + w) return 0;
  if (v.y > y + h) return 0;
  return 1;
}

bool sui_mouse_in_rec(SUI_Rect rect) {
  return sui_mouse_in(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
}

#endif
#undef SUI_IMPL
#endif

/*
MIT No Attribution

Copyright 2024 Aaron Speedy

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
