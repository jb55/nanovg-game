
#include "world.h"
#include "globals.h"


// TODO: move to world module
void world_render_ground(float width, float height) {
  static const float ratio = 4.0;
  static const float col = 0.2;
  nvgFillColor(nvg, nvgRGBf(col, col, col));
  nvgRect(nvg, 0.0, height / ratio , width, height - height / ratio);
  nvgFill(nvg);
}
