#ifndef PHYSIQUE_H
#define PHYSIQUE_H
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "lis_ast.h"
#include "struct.h"




void sprite_init(sprite_t *sprite, int type, SDL_Surface * sprite_picture, int sprite_size, int anim_sprite_num);
void sprite_turn_left(sprite_t *sprite);
void sprite_turn_right(sprite_t *sprite);
void sprite_move(sprite_t *sprite);
void sprite_boost(sprite_t *sprite, float accel);
void app_project(sprite_t *space_ship, sprite_t *project);
void app_ast(l_ast *aster, SDL_Surface *big, SDL_Surface *med,
	     SDL_Surface *small);
void div_ast(sprite_t ast, l_ast *rest, sprite_t project);
bool proj_contact(sprite_t *project, sprite_t *ast);
void proj_contact_list(sprite_t *project, l_ast *ast);
bool ship_contact(sprite_t *ship, sprite_t *ast);
bool ship_contact_list(sprite_t *ship, l_ast *ast);
void ast_move(l_ast *aster);

#endif
