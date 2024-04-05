/* sui.h - v0.0001  - public domain - no warranty implied; use at your own risk

  This is a single header file for doing immediate mode guis.

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.
*/

#ifndef UI_H
#define UI_H

#define Vec2 Vector2

typedef struct {
  Vec2 pos;
  Vec2 size;
} Rect;

Vec2 _place_rel(float x, float y, Vec2 size);
Rect _rel_to_abs(float x, float y, float width, float height);
Rect _abs_to_rel(Rect *rect);

Rectangle _rect_to_rectangle(Rect *rect);

bool _mouse_in(float x, float y, float width, float height);
bool _mouse_in_rec(Rect *rect);

void ui_do_text(char *text, float x, float y, Font font, Color color);
bool ui_do_button(char *text, float x, float y, float width, float height);
bool ui_do_button_down(char *text, float spacing, float width, float height);
void ui_do_panel(float x, float y, float width, float height);

#ifdef SUI_IMPL

Font reg_font;
Font title_font;

struct {
  int count, cap;
  Rect *items;
} space_stack = {
  .cap = 100,
};

Rect last_do = {0};

Vec2 _place_rel(float x, float y, Vec2 size) {
  Rect space = da_last(&space_stack);
  return (Vec2) {
    .x = space.pos.x + x * space.size.x - size.x/2,
    .y = space.pos.y + y * space.size.y - size.y/2,
  };
}

Rect _rel_to_abs(float x, float y, float width, float height) {
  Rect rect = {
    .size = {
      .x = width * da_last(&space_stack).size.x,
      .y = height * da_last(&space_stack).size.y,
    },
  };
  rect.pos = _place_rel(x, y, rect.size);
  return rect;
}

Rect _abs_to_rel(Rect *abs) {
  Rect space = da_last(&space_stack);

  Rect rect = {
    .size = {
      .x = abs->size.x / space.size.x,
      .y = abs->size.y / space.size.y,
    },
  };

  rect.pos.x = (abs->pos.x - space.pos.x + abs->size.x/2) / space.size.x;
  rect.pos.y = (abs->pos.y - space.pos.y + abs->size.y/2) / space.size.y;

  return rect;
}

Rectangle _rect_to_rectangle(Rect *rect) {
  return (Rectangle) {
    .x = rect->pos.x,
    .y = rect->pos.y,
    .width = rect->size.x,
    .height = rect->size.y,
  };
}

bool _mouse_in(float x, float y, float width, float height) {
  Vec2 v = GetMousePosition();
  if (v.x < x) return 0;
  if (v.y < y) return 0;
  if (v.x > x + width) return 0;
  if (v.y > y + height) return 0;
  return 1;
}

bool _mouse_in_rec(Rect *rect) {
  return _mouse_in(rect->pos.x, rect->pos.y, rect->size.x, rect->size.y);
}

void ui_do_text(char *text, float x, float y, Font font, Color color) {
  Rect rect = { .size = MeasureTextEx(font, text, font.baseSize, 1) };
  rect.pos = _place_rel(x, y, rect.size);

  DrawTextEx(
    font,
    text,
    rect.pos,
    font.baseSize,
    1,
    color
  );

  last_do = _abs_to_rel(&rect);
}

bool ui_do_button(char *text, float x, float y, float width, float height) {
  Rect rect = _rel_to_abs(x, y, width, height);

  Color button_color = WHITE;
  Color text_color = BLACK;

  if (_mouse_in_rec(&rect)) {
    float factor = 0.75;
    button_color.r *= factor;
    button_color.g *= factor;
    button_color.b *= factor;
  }

  DrawRectangleRounded(_rect_to_rectangle(&rect), 0.1, 20, button_color);

  da_push(&space_stack, rect);

    ui_do_text(text, 0.5, 0.5, reg_font, text_color);

  da_pop(&space_stack);

  last_do = _abs_to_rel(&rect);

  return _mouse_in_rec(&rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool ui_do_button_down(char *text, float spacing, float width, float height) {
  return ui_do_button(
    text,
    last_do.pos.x,
    last_do.pos.y + last_do.size.y + spacing,
    width, height
  );
}

void ui_do_panel(float x, float y, float width, float height) {
  Rect rect = _rel_to_abs(x, y, width, height);

  da_push(&space_stack, rect);

  Color panel_color = { 20, 20, 20, 255, };

  DrawRectangleRounded(_rect_to_rectangle(&rect), 0.1, 20, panel_color);

  last_do = _abs_to_rel(&rect);
}

#endif

#endif
