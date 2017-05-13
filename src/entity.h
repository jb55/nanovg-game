
#ifndef ENTITY_H
#define ENTITY_H

#include "maths.h"
#include <chipmunk/chipmunk.h>

#define MAX_ENTITIES 4096

enum entity_type {
  entity_test = 0,
  entity_ball = 1,
  entity_rect = 2
};

enum entity_dynamics {
  dynamics_static = 0,
  dynamics_rigid  = 1
};

typedef struct entity {
  vec2 position, velocity, size;
  enum entity_type type;
  cpBody *body;
  cpShape *shape;
  float mass, friction;
} Entity;

void entity_ball_draw(Entity *);
void entity_create_ball(Entity *);
void entity_create_rect(Entity *, enum entity_dynamics);
void entity_draw(Entity *);
void entity_init(Entity *);
void entity_rect_draw(Entity *);
void entity_set_position(Entity *, vec2 pos);
void entity_test_draw(Entity *);
void entity_update(Entity *);

const char* entity_name(Entity *);

typedef struct entity_def {
  const char *name;
} EntityDef;

extern const EntityDef entity_defs[];

#endif /* ENTITY_H */
