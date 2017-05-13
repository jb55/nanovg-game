
#ifndef ENTITY_H
#define ENTITY_H

#include "maths.h"
#include <chipmunk/chipmunk.h>

#define MAX_ENTITIES 4096

enum entity_type {
  entity_test = 0,
  entity_ball = 1
};

typedef struct entity {
  vec2 position;
  enum entity_type type;
  cpShape *collision_shape;
} Entity;

void entity_init(Entity *);
void entity_update(Entity *);
void entity_draw(Entity *);
void entity_test_draw(Entity *);


#endif /* ENTITY_H */
