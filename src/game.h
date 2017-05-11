
#ifndef GAME_H
#define GAME_H

#include <nanovg/nanovg.h>
#include "entity.h"

#define MAX_ENTITIES 4096

typedef struct game {
  NVGcontext *vg;

  float height;
  float width;

  float time;

  int fontNormal;

  Entity *entities;
  int entity_count;
} Game;

void game_init(Game *game);
void game_free(Game *game);
void game_render(Game *game);
int game_entity_add(Game *game, Entity *ent);
void game_setup(Game *game);

extern struct NVGcontext *nvg;

#endif /* GAME_H */
