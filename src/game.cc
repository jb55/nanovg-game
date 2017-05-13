
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
  world_load(&game->world, game->width, game->height);
}

void game_frame(Game *game) {
  world_update(&game->world, game->time);
  world_render(&game->world, game->width, game->height, game->time);
}


