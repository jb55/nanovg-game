#include <bx/string.h>
#include <bx/timer.h>
#include <nanovg/nanovg.h>
#include <bgfx/bgfx.h>
// TODO: figure out why INCLUDE_PATH isn't working here
#include <entry/entry.h>
#include <imgui/imgui.h>
#include <bgfx-nanovg/bgfx-nanovg.h>

// TODO: organize me
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct board {
  float w, h;
  float x, y;
  int states[16];
};

struct game {
  NVGcontext *vg;
  struct board *board;
  int fontNormal;
};

struct color {
  int r, g, b;
};

static const struct color tile_colors[] = {
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

void drawTile(struct game *game, float x, float y, int ts, int n,
              const struct color *c) {
  auto vg = game->vg;
  auto bx = game->board->x;
  auto by = game->board->y;
  NVGcolor txtColor;
  float fsize = 58.0f;
  float txtx = fsize / 4.65f;
  float txty = fsize / 3.4f;
  char txtbuf[8];

  float tx = bx + (15 * (x+1)) + (ts * x);
  float ty = by + (15 * (y+1)) + (ts * y);

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
    txtx = strlen(txtbuf) * txtx;
    nvgText(vg, tx + (ts / 2) - txtx, ty + (ts / 2) + txty, txtbuf, NULL);
  }
}

void drawTiles(struct game *game) {
  int ts = 105;
  int i = 0;

  // TODO: centered
  for (auto x = 0; x < 4; ++x)
  for (auto y = 0; y < 4; ++y, ++i) {
    drawTile(game, x, y, ts, game->board->states[i],
             &tile_colors[i % ARRAY_SIZE(tile_colors)]);
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


int _main_(int argc, char *argv[])
{
  uint32_t width = 1280;
  uint32_t height = 720;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;

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

  struct game game;
  struct board board;
  board.w = 500;
  board.h = 500;
  board.x = board.w / 2;
  board.y = board.h / 2;
  for (int i = 0; i < 16; ++i) {
    board.states[i] = 2 << i;
  }
  game.board = &board;
  game.vg = nvg;
  bgfx::setViewSeq(0, true);

  if (loadData(nvg, &game) == -1) {
    return 1;
  }

  int64_t timeOffset = bx::getHPCounter();

  entry::MouseState mouseState;
  while (!entry::processEvents(width, height, debug, reset, &mouseState) )
  {
    updateGameBoard(&board, width, height);

    int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency() );
    // float time = (float)( (now-timeOffset)/freq);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    // Use debug font to print information about this example.
    // bgfx::dbgTextClear();
    // bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/20-nanovg");
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
