
#ifndef ENTITY_H
#define ENTITY_H

#include "maths.h"

#define MAX_ENTITIES 4096

enum entity_type {
  entity_test = 0
};

typedef struct entity {
  vec2 position;
  enum entity_type type;
} Entity;

void entity_init(Entity *);
void entity_update(Entity *);
void entity_draw(Entity *);
void entity_test_draw(Entity *);


#endif /* ENTITY_H */
