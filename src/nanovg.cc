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
  struct NSVGimage *monstercat;
  struct NSVGimage *firefox;
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

  res->monstercat = nsvgParseFromFile("monstercat.svg", "px", 96);
  if (res->monstercat == NULL) {
    printf("Could not load monstercat.svg");
    return -1;
  }

  res->firefox = nsvgParseFromFile("firefox.svg", "px", 96);
  if (res->firefox == NULL) {
    printf("Could not load firefox.svg");
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
  struct NSVGimage *images[3];
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
    // , 0xffffffff
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

  images[0] = res.testSVG;
  images[1] = res.monstercat;
  images[2] = res.firefox;

  bgfx::setViewSeq(0, true);

  int64_t timeOffset = bx::getHPCounter();

  entry::MouseState mouseState;
  while (!entry::processEvents(width, height, debug, reset, &mouseState) )
  {
    int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency() );
    float time = (float)( (now-timeOffset)/freq);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, width, height);

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    nvgBeginFrame(nvg, width, height, 1.0f);

    nanosvgTest(nvg, images, ARRAY_SIZE(images), time);

    nvgEndFrame(nvg);

    // the2048Game();

    bgfx::frame();
  }

  nvgDelete(nvg);
  imguiDestroy();

  // Shutdown bgfx.
  bgfx::shutdown();

  return 0;
}
