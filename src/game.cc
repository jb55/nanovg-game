
#include "game.h"
#include "logging.h"
#include "entity.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct NVGcontext *nvg;

void world_render(struct game *game);
void draw_test_ent(struct game *game);
void entity_draw(struct entity *ent);
void entity_test_draw(struct entity *ent);

static inline float rand_nat() {
  return (float)((double) rand() / (double)RAND_MAX);
}

static inline float rand_range(float minv, float maxv) {
  return ((float(rand()) / float(RAND_MAX)) * (maxv - minv)) + minv;
}

static inline float rand_unit() {
  return rand_range(-1.0, 1.0);
}

void
game_init(Game *game) {
  void *entities = malloc(sizeof(struct entity) * MAX_ENTITIES);
  game->entities = (struct entity*)entities;
  game->entity_count = 0;
}

void
game_free(Game *game) {
  logdebug("freeing game\n");
  //free(game->entities);
  logdebug("freed game\n");
}

int
game_entity_add(Game *game, Entity *entity) {
  if (game->entity_count == MAX_ENTITIES) {
    logwarn("Could not add any more entities");
    return 0;
  }
  game->entities[game->entity_count++] = *entity;
  return 1;
}

void game_setup(Game *game) {
  Entity ent;
  for (int i = 0; i < 100; ++i) {
    float r1 = rand_unit();
    float r2 = rand_unit();
    logdebug("rand_unit x y %f %f\n", r1, r2);
    vec2 pos = vec2(r1 * game->width, r2 * game->height);
    ent.position = pos;
    ent.type = entity_test;
    game_entity_add(game, &ent);
  }
}

void
game_render(Game *game) {
  Entity *entities = game->entities;
  vec2 nudge;
  for (int i = 0; i < game->entity_count; ++i) {
    Entity *ent = &game->entities[i];
    nudge.x = cos(game->time);
    nudge.y = sin(game->time);

    game->entities[i].position += nudge;

    nvgSave(nvg);
    nvgTranslate(nvg, ent->position.x, ent->position.y);
    entity_update(&entities[i]);
    entity_draw(&entities[i]);
    nvgRestore(nvg);
  }
}
