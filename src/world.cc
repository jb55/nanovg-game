
#include "world.h"
#include "globals.h"
#include "logging.h"

#define v2cpv(v2) (cpv((v2).x, (v2).y))

void world_init(World *world) {
  void *entities = malloc(sizeof(struct entity) * MAX_ENTITIES);
  world->entities = (struct entity*)entities;
  assert(world->entities);
  world->entity_count = 0;
  world->space = cpSpaceNew();
}

void world_load(World *world, float width, float height) {
  vec2 ground_top, ground_bottom;
  cpBody *static_body = cpSpaceGetStaticBody(world->space);
  world_get_ground_ext(width, height, &ground_top, &ground_bottom);
  world->col_ground =
    cpSegmentShapeNew(static_body, v2cpv(ground_top), v2cpv(ground_bottom), 0);
}

void world_unload(World *world) {
  // TODO: free col_ground
}

void world_get_ground_ext(float width, float height, vec2 *top_left, vec2 *bottom_right) {
  static const float ratio = 4.0;
  static const float margin = 20.0;
  top_left->x = -margin;
  top_left->y = height - height / ratio;
  bottom_right->x = width + margin*2;
  bottom_right->y = height / ratio + margin;
}

void world_free(World *world) {
  cpSpaceFree(world->space);
}

void world_set_gravity(World *world, vec2 v2) {
  cpVect gravity = cpv(v2.x, v2.y);
  cpSpaceSetGravity(world->space, gravity);
}

// TODO: move to world module
void world_render_ground(float width, float height) {
  static const float col = 0.2;
  static const float darker = col * 0.01f;
  vec2 top_left, bottom_right;
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGBf(col, col, col));
  nvgStrokeColor(nvg, nvgRGBf(darker, darker, darker));
  nvgStrokeWidth(nvg, g_stroke);

  world_get_ground_ext(width, height, &top_left, &bottom_right);
  nvgRect(nvg, top_left.x, top_left.y, bottom_right.x, bottom_right.y);
  nvgFill(nvg);
  nvgStroke(nvg);
}

int
world_entity_add(World *world, Entity *entity) {
  if (world->entity_count == MAX_ENTITIES) {
    logwarn("Could not add any more entities");
    return 0;
  }
  world->entities[world->entity_count++] = *entity;
  return 1;
}


void
world_render(World *world, int width, int height, float time) {
  Entity *entities = world->entities;
  vec2 nudge;

  world_render_ground((float)width, (float)height);

  for (int i = 0; i < world->entity_count; ++i) {
    Entity *ent = &world->entities[i];
    nudge.x = cos(time);
    nudge.y = sin(time);

    world->entities[i].position += nudge;

    nvgSave(nvg);
    nvgTranslate(nvg, ent->position.x, ent->position.y);
    entity_update(&entities[i]);
    entity_draw(&entities[i]);
    nvgRestore(nvg);
  }
}
