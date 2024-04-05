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
do_panel(0.5, 0.5, 0.8, 0.5);

  do_text(
    "You won! ... or maybe not",
    0.5, 0.15,
    title_font,
    WHITE
  );

  if (do_button_down("New Game", 0.1, 0.4, 0.2)) {
    mode = MODE_SRC;
  }
  if (do_button_down("Exit", 0.02, 0.4, 0.2)) {
    mode = MODE_SRC;
  }

da_pop(&space_stack);
```

To change functionality or styling, simply edit or add any ui_do functions.
