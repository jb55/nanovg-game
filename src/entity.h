
#ifndef ENTITY_H
#define ENTITY_H

#include "maths.h"
#include <chipmunk/chipmunk.h>

#define MAX_ENTITIES 4096

enum entity_type {
  entity_test   = 0,
  entity_ball   = 1,
  entity_rect   = 2,
  entity_player = 3,
};

enum entity_dynamics {
  dynamics_static = 0,
  dynamics_rigid  = 1
};

struct ent_player {
  char *name;
};

struct ent_ball {
  float radius;
};

struct ent_rect {
  vec2 size;
};

union entity_data {
  struct ent_player player;
  struct ent_ball ball;
  struct ent_rect rect;
};

struct entity {
  vec2 position, velocity;
  enum entity_type type;
  cpBody *body;
  cpShape *shape;
  float mass, friction;
  union entity_data data {};
};

void entity_ball_draw(struct entity *);

struct ent_ball *
entity_create_ball(struct entity *, struct ent_ball *);

struct ent_rect *
entity_create_rect(struct entity *, struct ent_rect *, enum entity_dynamics);

void entity_draw(struct entity *);
void entity_init(struct entity *);
void entity_rect_draw(struct entity *);
void entity_set_position(struct entity *, vec2 pos);
void entity_test_draw(struct entity *);
void entity_update(struct entity *);

const char* entity_name(struct entity *);

struct entity_def {
  const char *name;
};

extern const struct entity_def entity_defs[];

#endif /* ENTITY_H */
