#ifndef PHYSIQUE_H
#define PHYSIQUE_H
#include <SDL.h>
#include <math.h>
#include <time.h>


#define INIT_DIR 9
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define VIT_MAX 5
#define VIT_PROJ 0.6
#define VIT_BIG_AST 0.1
#define VIT_MED_AST 0.2
#define VIT_SMALL_AST 0.4
#define FROTEMENT   0.001

typedef struct Sprite_t sprite_t;

struct Sprite_t{
  int type;
  int col;
  int lig;
  double x;
  double y;
  int current;
  int size;
  int nb_sprite;
  double vx;
  double vy;
};

typedef struct L_Asteroid L_asteroid;

struct L_Asteroid{
  sprite_t first;
  L_asteroid *next;
};


void sprite_init(sprite_t *sprite, int type, SDL_Surface * sprite_picture, int sprite_size, int anim_sprite_num);
void sprite_turn_left(sprite_t *sprite);
void sprite_turn_right(sprite_t *sprite);
void sprite_move(sprite_t *sprite);
void sprite_boost(sprite_t *sprite, float accel);
void app_project(sprite_t *space_ship, sprite_t *project);
void app_ast(sprite_t *asteroid, SDL_Surface *aster);


#endif
