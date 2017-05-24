#ifndef PHYSIQUE_H
#define PHYSIQUE_H
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "lis_ast.h"
#include "struct.h"

/* initialisation of one sprite */
void sprite_init(sprite_t *sprite, int type, SDL_Surface * sprite_picture, int sprite_size, int anim_sprite_num);

/* the space ship otate to the left */
void sprite_turn_left(sprite_t *sprite);

/* the space ship otae to the right */
void sprite_turn_right(sprite_t *sprite);

/* Movement of the sprite */
void sprite_move(sprite_t *sprite);

/* Boost of space_ship */
void sprite_boost(sprite_t *sprite, float accel);

/* fonction used when the space ship shot */
void app_project(sprite_t *space_ship, sprite_t *project);

/* fonction to create a new asteroid */
void app_ast(l_ast *aster, SDL_Surface *big, SDL_Surface *med,
	     SDL_Surface *small);

/* fonction to divide an asteroid in 2 asteroid or destroy a small asteroid */
void div_ast(sprite_t ast, l_ast *rest, sprite_t project, int *points);

/* fonction to verify if the projectile is on contact with an asteroid */
bool proj_contact(sprite_t *project, sprite_t *ast);

/* fonction to verify if the projectile is on contact with each asteroid */
void proj_contact_list(sprite_t *project, l_ast *ast, int *points);

/* fonction to verify if the space ship crash on an asteoid */
bool ship_contact(sprite_t *ship, sprite_t *ast);

/* fonction to verify if the space ship crach on each asteroid */
bool ship_contact_list(sprite_t *ship, l_ast *ast);

/* fonction to move each asteroid */
void ast_move(l_ast *aster);

#endif
