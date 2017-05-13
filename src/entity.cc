
#include "entity.h"
#include "globals.h"
#include "logging.h"

const EntityDef entity_defs[] = {
  { "test" },
  { "ball" },
  { "rect" },
};

void entity_draw(Entity *ent) {
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
  }
}

void entity_update(Entity *ent) {
}

void entity_init(Entity *ent) {
  ent->position = vec2(0, 0);
  ent->type     = entity_test;
  ent->body     = 0;
  ent->shape    = 0;
  ent->size     = vec2(20.0, 20.0);
  ent->mass     = 1.0;
  ent->friction = 0.7;
}

void entity_ball_draw(Entity *ent) {
  nvgBeginPath(nvg);
  nvgTranslate(nvg, ent->position.x, ent->position.y);
  nvgFillColor(nvg, nvgRGB(255, 255, 255));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  // TODO: sort out circle size
  nvgCircle(nvg, 0.0, 0.0, ent->size.x);
  nvgFill(nvg);
  // nvgStrokeWidth(nvg, g_stroke);
  // nvgStroke(nvg);
}

void entity_rect_draw(Entity *ent) {
  nvgBeginPath(nvg);
  nvgFillColor(nvg, nvgRGBf(0.5, 0.5, 0.5));
  nvgStrokeColor(nvg, nvgRGB(0, 0, 0));
  nvgRect(nvg, ent->position.x, ent->position.y, ent->size.x, ent->size.y);
  logdebug("drawing rect %f %f %f %f\n",
           ent->position.x, ent->position.y, ent->size.x, ent->size.y);
  nvgFill(nvg);
  nvgStrokeWidth(nvg, g_stroke);
  nvgStroke(nvg);
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

void entity_set_position(Entity *ent, vec2 pos) {
  ent->position = pos;
  cpBodySetPosition(ent->body, v2cpv(pos));
}

void entity_create_rect(Entity *ent, enum entity_dynamics dynamics) {
  ent->type = entity_rect;
  //const float mass = ent->mass;
  // TODO: plan for freeing physics stuff when done with it
  // TODO: non static rects
  cpBody *body = dynamics == dynamics_static ? cpBodyNewStatic() : 0;
  cpShape *shape =
    cpBoxShapeNew(body, ent->size.x, ent->size.y, 0);

  ent->shape = shape;
  ent->body = body;

  // TODO: non-static rect friction
}

void entity_create_ball(Entity *ent) {
  ent->type = entity_ball;
  // TODO: plan for freeing physics stuff when done with it
  const float mass = ent->mass;
  const float radius = ent->size.x;
  float moment = cpMomentForCircle(mass, 0, radius, cpvzero);
  cpBody *body = cpBodyNew(mass, moment);
  cpShape *shape = cpCircleShapeNew(body, radius, cpvzero);
  // TODO static balls
  ent->shape = shape;
  ent->body = body;
  logdebug("m %f r %f m %f body %x shape %x\n",
           mass, radius, moment, body, shape);
  cpShapeSetFriction(shape, 0.0);
  cpShapeSetElasticity(shape, 0.8);
}

const char * entity_name(Entity *ent) {
  return entity_defs[ent->type].name;
}
