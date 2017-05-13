
#include "world.h"
#include "globals.h"
#include "logging.h"
#include <cmath>

#include <chipmunk/chipmunk.h>

static inline float rand_range(float minv, float maxv) {
  return ((float(rand()) / float(RAND_MAX)) * (maxv - minv)) + minv;
}

static inline float rand_unit() {
  return rand_range(-1.0, 1.0);
}

static inline float rand_pos() {
  return rand_range(0.0, 1.0);
}

void world_init(World *world) {
  void *entities = malloc(sizeof(struct entity) * MAX_ENTITIES);
  world->entities = (struct entity*)entities;
  assert(world->entities);
  world->entity_count = 0;
  world->space = cpSpaceNew();
}

void world_load_map(World *world, float width, float height) {
  vec2 xy, wh;
  Entity ground;
  entity_init(&ground);
  world_get_ground_ext(width, height, &xy, &wh);
  ground.size = wh;
  entity_create_rect(&ground, dynamics_static);
  entity_set_position(&ground, xy + wh / 2.0f);
  world_entity_add(world, &ground);
}

void world_load(World *world, float width, float height) {
  vec2 ground_top, ground_bottom;
  world_set_gravity(world, vec2(0.0, 0.1));
  world_load_map(world, width, height);

  Entity ent;
  entity_init(&ent);
  for (int i = 0; i < 50; ++i) {
    vec2 pos = vec2(rand_pos() * width, 200.0);
    ent.size.x = rand_pos() * 20.0;
    entity_create_ball(&ent);
    entity_set_position(&ent, pos);
    world_entity_add(world, &ent);
  }
}

void world_unload(World *world) {
  // TODO: free col_ground
}

void world_get_ground_ext(float width, float height, vec2 *xy, vec2 *wh) {
  static const float ratio = 4.0;
  static const float margin = 20.0;
  xy->x = -margin;
  xy->y = height - height / ratio;
  wh->x = width + margin;
  wh->y = height / ratio;
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

  // hook up collision here? probably.
  if (entity->body && entity->shape) {
    cpSpaceAddBody(world->space, entity->body);
    cpSpaceAddShape(world->space, entity->shape);
  }
  else {
    logwarn("Collision missing when adding %s", entity_name(entity));
  }
  // TODO: unhook collision when removed from world?
  return 1;
}


void
world_render(World *world, float width, float height, float time) {
  Entity *entities = world->entities;
  vec2 nudge;

  //world_render_ground(width, height);

  for (int i = 0; i < world->entity_count; ++i) {
    Entity *ent = &world->entities[i];
    nvgSave(nvg);
    entity_draw(&entities[i]);
    nvgRestore(nvg);
  }
}


void
world_entity_update(World *world, Entity *entity) {
  cpVect pos = cpBodyGetPosition(entity->body);
  cpVect vel = cpBodyGetVelocity(entity->body);
  entity->position = vec2(pos.x, pos.y);
  entity->velocity = vec2(vel.x, vel.y);
}

void
world_update(World *world, float dt) {

  for (int i = 0; i < world->entity_count; ++i) {
    Entity *ent = &world->entities[i];
    world_entity_update(world, ent);
  }

  cpSpaceStep(world->space, dt);
}
