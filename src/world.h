
#ifndef WORLD_H
#define WORLD_H

#include <chipmunk/chipmunk.h>
#include "maths.h"
#include "entity.h"

struct world {
  cpSpace *space;
  struct entity *entities;
  int entity_count;
};

void world_init(struct world *world);
void world_free(struct world *world);
void world_set_gravity(struct world *world, vec2 v2);
void world_update(struct world *world, float dt);
void world_render(struct world *world, float width, float height, float time);
void world_render_ground(float width, float height);
int world_entity_add(struct world *world, struct entity *entity);
void world_load(struct world *world, float width, float height);
void world_get_ground_ext(float width, float height,
                          vec2 *top_left, vec2 *bottom_right);

#endif /* WORLD_H */
