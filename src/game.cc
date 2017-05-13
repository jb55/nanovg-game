
#include "game.h"
#include "logging.h"
#include "entity.h"
#include "globals.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void world_render(struct game *game);
void draw_test_ent(struct game *game);
void entity_draw(struct entity *ent);
void entity_test_draw(struct entity *ent);

static inline float rand_range(float minv, float maxv) {
  return ((float(rand()) / float(RAND_MAX)) * (maxv - minv)) + minv;
}

static inline float rand_unit() {
  return rand_range(-1.0, 1.0);
}

void
game_init(Game *game) {
  world_init(&game->world);
}

void
game_free(Game *game) {
  logdebug("freeing game\n");
  world_free(&game->world);
  logdebug("freed game\n");
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
    world_entity_add(&game->world, &ent);
  }
}

void
game_render(Game *game) {
  world_render(&game->world, game->width, game->height, game->time);
}

