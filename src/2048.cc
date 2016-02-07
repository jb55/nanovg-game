
#define NTILES 16

enum gameState {
  GAME_IDLE = 0,
  GAME_ANIMATING
};

static const char *gameStateStrs[] = {
  "idle",
  "animating"
};

enum direction {
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_RIGHT,
  DIR_LEFT
};

struct tile {
  float x, y;
  int value;
};

struct board {
  float w, h;
  float x, y;
  // for animations
  struct tile tiles[16];
};

struct game {
  NVGcontext *vg;
  direction lastDir;
  gameState state;
  int fontNormal;
  struct board *board;
};

struct color {
  int r, g, b;
};

static const struct NVGcolor white = { 1, 1, 1, 1 };

static const struct color tile_colors[] = {
  { 205, 193, 180 }, // 0
  { 238, 228, 218 }, // 2
  { 237, 224, 200 }, // 4
  { 242, 177, 121 }, // 8
  { 245, 149, 99 },  // 16
  { 246, 124, 95 },  // 32
  { 246, 94,  59 },  // 64
  { 237, 207, 114 }, // 128
};


void drawTile(struct game *game, float x, float y, struct tile *tile, float ts,
              or (auto y = 0; y < 4; ++y, ++i) {
    tile = &game->board->tiles[i];
    state = tile->value;
    if (state == 0) continue;
    colorind = int(log2(state)) % ARRAY_SIZE(tile_colors);
    drawTile(game, x, y, tile, ts, state, &tile_colors[colorind]);
  }
}


static void
drawBoard(struct game *game) {
  auto bw = game->board->w;
  auto bh = game->board->h;
  auto bx = game->board->x;
  auto by = game->board->y;
  auto vg = game->vg;

  auto bg = nvgLinearGradient(vg, bx, by, bx+bw, by+bh,
                              nvgRGBA(187,173,160,255),
                              nvgRGBA(159,147,136,255));

  nvgBeginPath(vg);
  nvgRoundedRect(vg, bx, by, bw, bh, 20);
  nvgFillPaint(vg, bg);
  nvgFill(vg);

  drawTiles(game);
}

static void
updateGameBoard(struct board *board, int swidth, int sheight) {
  board->x = (swidth / 2) - (board->w / 2);
  board->y = (sheight / 2) - (board->h / 2);
}

static void
updateGameAnimations(struct tile *tiles, direction dir) {
  static const float move = 1.f;
  float xm, ym;
  if (dir == DIR_UP)    { xm = 0.f;  ym = -1.f; }
  if (dir == DIR_RIGHT) { xm = 1.f;  ym = 0.f; }
  if (dir == DIR_DOWN)  { xm = 0.f;  ym = 1.f; }
  if (dir == DIR_LEFT)  { xm = -1.f; ym = 0.f; }
  if (dir == DIR_NONE)  { xm = 0.f;  ym = 0.f; }

  for (int i = 0; i < NTILES; ++i) {
    if (tiles[i].value != 0) {
      tiles[i].x += move * xm;
      tiles[i].y += move * ym;
    }
  }
}

static void
initBoard(struct board *board) {
  board->w = 500;
  board->h = 500;
  board->x = board->w / 2;
  board->y = board->h / 2;

  for (int i = 0; i < NTILES; ++i) {
    board->tiles[i].x = 0;
    board->tiles[i].y = 0;
    board->tiles[i].value = 0;
  }
  int r = rand() % 15;
  board->tiles[r].value = 2;
}

static int
initGame(struct game *game, struct board *board, NVGcontext *nvg) {
  initBoard(board);

  game->lastDir = DIR_NONE;
  game->board = board;
  game->vg = nvg;

  return 0;
}

static void
showStates(const struct tile *tiles, char *buf) {
  char *pos = buf;
  for (int i = 0 ; i < NTILES; i++) {
    if (i) pos += sprintf(pos, ", ");
    pos += sprintf(pos, "%d", tiles[i].value);
  }
}


void
the2048Game(float width, float height) {
  bgfx::dbgTextClear();
  if (game.state != GAME_ANIMATING) {
    direction dir = DIR_NONE;
    if (inputGetKeyState(entry::Key::Up))         dir = DIR_UP;
    else if (inputGetKeyState(entry::Key::Right)) dir = DIR_RIGHT;
    else if (inputGetKeyState(entry::Key::Down))  dir = DIR_DOWN;
    else if (inputGetKeyState(entry::Key::Left))  dir = DIR_LEFT;
    if (dir != DIR_NONE)
      game.lastDir = dir;
  }
  else {
  }
  bgfx::dbgTextPrintf(0, 3, 0x2f, "dir: %d (u%d r%d l%d d%d)", game.lastDir,
                      entry::Key::Up, entry::Key::Right, entry::Key::Down,
                      entry::Key::Left);

  bool shouldAnimate = game.state == GAME_ANIMATING ||
    (game.state == GAME_IDLE && game.lastDir != DIR_NONE);

  if (shouldAnimate) {
    game.state = GAME_ANIMATING;
    updateGameAnimations(board.tiles, game.lastDir);
  }

  // updates the size of the board on resize
  updateGameBoard(&board, width, height);

  // Use debug font to print information about this example.
  showStates(board.tiles, txtbuf);
  bgfx::dbgTextPrintf(0, 1, 0x4f, "state: %s %s", gameStateStrs[game.state], txtbuf);
  // bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: NanoVG is small antialiased vector graphics rendering library.");

  nvgBeginFrame(nvg, width, height, 1.0f);

  drawBoard(&game);

  nvgEndFrame(nvg);

  // Advance to next frame. Rendering thread will be kicked to
  // process submitted rendering primitives.
}
