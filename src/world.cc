
#include "world.h"
#include "globals.h"


// TODO: move to world module
void world_render_ground(float width, float height) {
  static const float ratio = 4.0;
  static const float col = 0.2;
  static const float margin = 20.0;
  static const float darker = col * 0.01f;
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGBf(col, col, col));
  nvgStrokeColor(nvg, nvgRGBf(darker, darker, darker));
  nvgStrokeWidth(nvg, g_stroke);
  nvgRect(nvg, -margin, height - height / ratio, width + margin*2,
          height / ratio + margin);
  nvgFill(nvg);
  nvgStroke(nvg);
}
