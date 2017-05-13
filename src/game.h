
#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "world.h"

typedef struct game {
  float height;
  float width;

  float time;

  int fontNormal;

  World world;
  int entity_count;
} Game;

void game_init(Game *game);
void game_free(Game *game);
void game_frame(Game *game);
void game_setup(Game *game);

#endif /* GAME_H */
