
#include "entity.h"
#include "globals.h"

void entity_draw(Entity *ent) {
  switch(ent->type) {
  case entity_test:
    entity_test_draw(ent);
    break;
  }
}

void entity_update(Entity *ent) {
}

void entity_init(Entity *ent) {
  ent->position = vec2(0, 0);
  ent->type     = entity_test;
}

void entity_test_draw(Entity *ent) {
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGB(255, 255, 255));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  nvgCircle(nvg, 0.0, 0.0, 50.0);
  nvgFill(nvg);
  nvgStrokeWidth(nvg, g_stroke);
  nvgStroke(nvg);
}
