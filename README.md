# sui.h
> [!WARNING]
> This library is unfinished. Keep your expectations low.

sui.h is a simple, do-it-yourself, single-header, immediate mode gui library.

Note: This library depends on Raylib. In the future this is planned not to be the case.

## Usage
```c
#define SUI_IMPL
#include "sui.h"
```

```c
da_init(&space_stack)
da_push(&space_stack, ROOT_RECT); /* Define ROOT_RECT to be anything. The dimensions and
                                     position will be the dimensions and position of
                                     the UI. */
reg_font = FOO;
tite_font = BAR;
```

```c
ui_do_panel(0.5, 0.5, 0.8, 0.5);

  ui_do_text(
    "You won! ... or maybe not",
    0.5, 0.15,
    title_font,
    WHITE
  );

  if (ui_do_button_down("New Game", 0.1, 0.4, 0.2)) {
    // ...
  }
  if (ui_do_button_down("Exit", 0.02, 0.4, 0.2)) {
    // ...
  }

da_pop(&space_stack);
```

To change functionality or styling, simply edit or add any ui_do functions.
