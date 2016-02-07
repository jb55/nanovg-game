#include <bx/string.h>
#include <bx/timer.h>
#include <nanovg/nanovg.h>
#include <bgfx/bgfx.h>
// TODO: figure out why INCLUDE_PATH isn't working here
#include <entry/entry.h>
#include <entry/input.h>
#include <imgui/imgui.h>
#include <math.h>
#include <time.h>
#include <bgfx-nanovg/bgfx-nanovg.h>
#include <nanosvg/nanosvg.h>
#include "nanosvg-test.h"

// TODO: organize me
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct resources {
  int fontNormal;
  struct NSVGimage *testSVG;
};

int loadResources(NVGcontext *nvg, struct resources *res) {
  res->fontNormal = nvgCreateFont(nvg, "sans", "font/roboto-regular.ttf");
  if (res->fontNormal == -1) {
    printf("Could not load regular font");
    return -1;
  }

  res->testSVG = nsvgParseFromFile("23.svg", "px", 96);
  if (res->testSVG == NULL) {
    printf("Could not load test.svg");
    return -1;
  }
  return 0;
}

void
destroyResources(struct resources *res) {
  nsvgDelete(res->testSVG);
}


int _main_(int argc, char **argv)
{
  char txtbuf[256];
  uint32_t width = 1280;
  uint32_t height = 720;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;
  struct resources res;
  // struct game game;
  // struct board board;

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

  // imguiCreate();

  NVGcontext* nvg = nvgCreate(1, 0);
  // if (initGame(&game, &board, nvg) == -1) {
  //   return 1;
  // }
  if (loadResources(nvg, &res) == -1) {
    return -1;
  }

  bgfx::setViewSeq(0, true);

  int64_t timeOffset = bx::getHPCounter();

  entry::MouseState mouseState;
  while (!entry::processEvents(width, height, debug, reset, &mouseState) )
  {
    int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency() );
    // float time = (float)( (now-timeOffset)/freq);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    // the2048Game();
    nanosvgTest(nvg, res.testSVG);

    bgfx::frame();
  }

  nvgDelete(nvg);
  imguiDestroy();

  // Shutdown bgfx.
  bgfx::shutdown();

  return 0;
}
