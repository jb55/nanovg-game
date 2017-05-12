
#include "world.h"
#include "globals.h"


// TODO: move to world module
void world_render_ground(float width, float height) {
  static const float ratio = 4.0;
  static const float col = 0.2;
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGBf(col, col, col));
  nvgRect(nvg, 0.0, height - height / ratio , width, height / ratio);
  nvgFill(nvg);
}
