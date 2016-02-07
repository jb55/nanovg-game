#include <bx/string.h>
#include <bx/timer.h>
#include <nanovg/nanovg.h>
#include <bgfx/bgfx.h>
// TODO: figure out why INCLUDE_PATH isn't working here
#include <entry/entry.h>
#include <entry/input.h>
#include <imgui/imgui.h>
#include <math.h>
#include <bgfx-nanovg/bgfx-nanovg.h>

// TODO: organize me
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
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

static const struct NVGcolor white = { 1, 1, 1, 1 };

int loadData(struct game *game) {
  game->fontNormal = nvgCreateFont(game->vg, "sans", "font/roboto-regular.ttf");
  if (game->fontNormal == -1) {
    printf("Could not load regular font");
    return -1;
  }
  return 0;
}

void drawTile(struct game *game, float x, float y, struct tile *tile, float ts,
              int n, const struct color *c) {
  char txtbuf[8];
  NVGcolor txtColor;
  auto vg = game->vg;
  float bx = game->board->x;
  float by = game->board->y;
  float fsize = 58.0f;
  float txtx = fsize / 4.65f;
  float txty = fsize / 3.4f;

  float tx = bx + (15.f * (x+1.f)) + (ts * x) + tile->x;
  float ty = by + (15.f * (y+1.f)) + (ts * y) + tile->y;

  nvgBeginPath(vg);

  nvgRoundedRect(vg, tx, ty, ts, ts, 10);
  nvgFillColor(vg, nvgRGBA(c->r, c->g, c->b, 255));
  nvgFill(vg);

  if (n >= 2) {
    nvgFontSize(vg, fsize);
    nvgFontFace(vg, "sans");
    txtColor = n <= 4 ? nvgRGBA(119, 110, 101, 255) : white;
    nvgFillColor(vg, txtColor);
    bx::snprintf(txtbuf, sizeof(txtbuf), "%d", n);
    txtx = float(strlen(txtbuf)) * txtx;

    float rtxtx = tx + (ts / 2.f) - txtx;
    bgfx::dbgTextPrintf(0, 2, 0x6f, "tilex: %f", tile->x);
    nvgText(vg, rtxtx, ty + (ts / 2.f) + txty, txtbuf, NULL);
  }
}

void drawTiles(struct game *game) {
  struct tile *tile;
  float ts = 105.f;
  int i = 0;
  int state = 0;
  int colorind = 0;

  // TODO: centered
  for (auto x = 0; x < 4; ++x)
  for (auto y = 0; y < 4; ++y, ++i) {
    tile = &game->board->tiles[i];
    state = tile->value;
    colorind = (i == 0 ? 0 : int(log2(state))) % ARRAY_SIZE(tile_colors);
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

static int
loadData(struct NVGcontext* vg, struct game* game)
{
  game->fontNormal = nvgCreateFont(vg, "sans", "font/roboto-bold.ttf");
  if (game->fontNormal == -1) {
    printf("Could not add font normal.\n");
    return -1;
  }
  return 0;
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
  if (loadData(nvg, game) == -1) {
    return -1;
  }
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

int _main_(int argc, char *argv[])
{
  char txtbuf[256];
  uint32_t width = 1280;
  uint32_t height = 720;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;
  struct game game;
  struct board board;

  srand(time(NULL));

  bgfx::init();
  bgfx::reset(width, height, reset);

  // Enable debug text.
  bgfx::setDebug(debug);

  // Set view 0 clear state.
  bgfx::setViewClear(0
    , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
    , 0x303030ff
    , 1.0f
    , 0
    );

  imguiCreate();

  NVGcontext* nvg = nvgCreate(1, 0);
  if (initGame(&game, &board, nvg) == -1) {
    return 1;
  }

  bgfx::setViewSeq(0, true);

  int64_t timeOffset = bx::getHPCounter();

  entry::MouseState mouseState;
  while (!entry::processEvents(width, height, debug, reset, &mouseState) )
  {
    // updates the size of the board on resize
    updateGameBoard(&board, width, height);

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

    int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency() );
    // float time = (float)( (now-timeOffset)/freq);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    // Use debug font to print information about this example.
    showStates(board.tiles, txtbuf);
    bgfx::dbgTextPrintf(0, 1, 0x4f, "state: %s %s", gameStateStrs[game.state], txtbuf);
    // bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: NanoVG is small antialiased vector graphics rendering library.");

    nvgBeginFrame(nvg, width, height, 1.0f);

    drawBoard(&game);

    nvgEndFrame(nvg);

    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
    bgfx::frame();
  }

  nvgDelete(nvg);
  imguiDestroy();

  // Shutdown bgfx.
  bgfx::shutdown();

  return 0;
}
