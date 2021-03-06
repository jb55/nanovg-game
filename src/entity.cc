
#include "entity.h"
#include "globals.h"
#include "logging.h"
#include <cstring>

const struct entity_def entity_defs[] = {
  { "test" },
  { "ball" },
  { "rect" },
  { "player" },
};

void entity_draw(struct entity *ent) {
  switch(ent->type) {
  case entity_test:
    entity_test_draw(ent);
    break;
  case entity_ball:
    entity_ball_draw(ent);
    break;
  case entity_rect:
    entity_rect_draw(ent);
    break;
  case entity_player:
    entity_player_draw(ent);
    break;
  }
}

void entity_player_draw(struct entity *ent) {
}

void entity_init(struct entity *ent) {
  static const float defsize = 10.0;
  ent->position = vec2(0, 0);
  ent->type     = entity_test;
  ent->body     = 0;
  ent->shape    = 0;
  ent->mass     = 1.0;
  ent->friction = 0.7;
}

struct ent_player *
entity_create_player(struct entity *ent, struct ent_player *player) {
  ent->data.player = *player;
}

void entity_ball_draw(struct entity *ent) {
  nvgBeginPath(nvg);
  struct ent_ball *ball = &ent->data.ball;
  float offset = ball->radius * 2;
  nvgTranslate(nvg, ent->position.x, ent->position.y);
  nvgFillColor(nvg, nvgRGB(255, 255, 255));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  // TODO: sort out circle size
  nvgCircle(nvg, 0.0, 0.0, ball->radius);
  nvgFill(nvg);
  // nvgStrokeWidth(nvg, g_stroke);
  // nvgStroke(nvg);
}

void entity_rect_draw(struct entity *ent) {
  static const float color = 0.4;
  struct ent_rect *rect = &ent->data.rect;
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGBf(color, color, color));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  // ORIGIN COORDS!
  nvgRect(nvg,
          ent->position.x - rect->size.x / 2,
          ent->position.y - rect->size.y / 2, rect->size.x, rect->size.y);
  nvgFill(nvg);
}

void entity_test_draw(struct entity *ent) {
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGB(255, 255, 255));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  nvgCircle(nvg, 0.0, 0.0, 50.0);
  nvgFill(nvg);
  nvgStrokeWidth(nvg, g_stroke);
  nvgStroke(nvg);
}

void entity_set_position(struct entity *ent, vec2 pos) {
  ent->position = pos;
  cpBodySetPosition(ent->body, v2cpv(pos));
}

struct ent_rect *
entity_create_rect(struct entity *ent, struct ent_rect *rect, enum entity_dynamics dynamics) {
  ent->type = entity_rect;
  //const float mass = ent->mass;
  // TODO: plan for freeing physics stuff when done with it
  // TODO: non static rects
  ent->data.rect = *rect;

  cpBody *body = dynamics == dynamics_static ? cpBodyNewStatic() : 0;
  cpShape *shape =
    cpBoxShapeNew(body, rect->size.x, rect->size.y, 0);

  ent->shape = shape;
  ent->body = body;

  cpShapeSetElasticity(shape, 0.5);
  cpShapeSetFriction(shape, 1.0);
  // TODO: non-static rect friction

  return rect;
}

struct ent_player *
entity_create_player(struct entity *ent, const char *name) {
  memcpy(ent->data.player.name, name, MAX_NAME_SIZE);

  return &ent->data.player;
}

struct ent_ball *
entity_create_ball(struct entity *ent, struct ent_ball *ball) {
  ent->type = entity_ball;
  ent->data.ball = *ball;
  // TODO: plan for freeing physics stuff when done with it
  const float mass = ent->mass;
  const float radius = ball->radius;
  const cpVect offset = cpv(radius, radius);
  float moment = cpMomentForCircle(mass, 0, radius, cpvzero);
  cpBody *body = cpBodyNew(mass, moment);
  cpShape *shape = cpCircleShapeNew(body, radius, cpvzero);
  // TODO static balls
  ent->shape = shape;
  ent->body = body;
  logdebug("m %f r %f m %f body %x shape %x\n",
           mass, radius, moment, body, shape);
  cpShapeSetFriction(shape, 0.7);
  cpShapeSetElasticity(shape, 0.5);

  return &ent->data.ball;
}

const char * entity_name(struct entity *ent) {
  return entity_defs[ent->type].name;
}
