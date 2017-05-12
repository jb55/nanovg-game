
#ifndef WORLD_H
#define WORLD_H

#include <chipmunk/chipmunk.h>
#include "maths.h"
#include "entity.h"

typedef struct world {
  cpSpace *space;
  Entity *entities;
  int entity_count;
} World;

void world_init(World *world);
void world_free(World *world);
void world_set_gravity(World *world, vec2 v2);
void world_render(World *world, int width, int height, float time);
void world_render_ground(float width, float height);
int world_entity_add(World *world, Entity *entity);

#endif /* WORLD_H */
