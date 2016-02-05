#include <bx/string.h>
#include <bx/timer.h>
#include <nanovg/nanovg.h>
#include <bgfx/bgfx.h>
// TODO: figure out why INCLUDE_PATH isn't working here
#include <entry/entry.h>
#include <bgfx-nanovg/bgfx-nanovg.h>

struct board {
  float w, h;
  float x, y;
};

struct game {
  NVGcontext *vg;
  struct board *board;
  int fontNormal;
};


int loadData(struct game *game) {
  game->fontNormal = nvgCreateFont(game->vg, "sans", "font/roboto-regular.ttf");
  if (game->fontNormal == -1) {
    printf("Could not load regular font");
    return -1;
  }
  return 0;
}

void drawTile(struct game *game, float x, float y, int ts, int n) {
  auto vg = game->vg;
  auto bx = game->board->x;
  auto by = game->board->y;

  float tx = bx + (15 * (x+1)) + (ts * x);
  float ty = by + (15 * (y+1)) + (ts * y);

  nvgBeginPath(vg);

  nvgRoundedRect(vg, tx, ty, ts, ts, 10);
  nvgFillColor(vg, nvgRGBA(238, 228, 218, 255));
  nvgFill(vg);

  nvgFontSize(vg, 24.0f);
  nvgFontFace(vg, "sans");
  nvgFillColor(vg, nvgRGBA(0,0,0,255));
  nvgTextAlign(vg, NVG_ALIGN_LEFT or NVG_ALIGN_MIDDLE);
  nvgText(vg, 100, 100, "ksdhfasdjhfaslkdjfh", NULL);
}

void drawTiles(struct game *game) {
  int ts = 105;

  // TODO: centered
  for (auto x = 0; x < 4; ++x)
  for (auto y = 0; y < 4; ++y) {
    drawTile(game, x, y, ts, 0);
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
loadDemoData(struct NVGcontext* vg, struct game* game)
{
  game->fontNormal = nvgCreateFont(vg, "sans", "font/roboto-regular.ttf");
  if (game->fontNormal == -1) {
    printf("Could not add font italic.\n");
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

  NVGcontext* nvg = nvgCreate(1, 0);
  struct game game;
  struct board board;
  board.w = 500;
  board.h = 500;
  board.x = board.w / 2;
  board.y = board.h / 2;
  game.board = &board;
  game.vg = nvg;
  bgfx::setViewSeq(0, true);

  loadDemoData(nvg, &game);

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
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/20-nanovg");
    bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: NanoVG is small antialiased vector graphics rendering library.");

    nvgBeginFrame(nvg, width, height, 1.0f);

    drawBoard(&game);

    nvgEndFrame(nvg);

    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
    bgfx::frame();
  }

  nvgDelete(nvg);

  // Shutdown bgfx.
  bgfx::shutdown();

  return 0;
}
