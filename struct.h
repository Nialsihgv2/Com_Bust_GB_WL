#ifndef STRUCT_H
#define STRUCT_H

/* Size of the window */
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

/* In the sprite, we have 36 images of a 32x32 picture */
#define NB_SPRITE       36

/* Size of ship: */
#define SPRITE_SIZE     32
/* Size of projectiles */
#define PROJECT_SIZE    8
/* Size of asteroids */
#define BIG_AST_SIZE    64
#define MED_AST_SIZE    32
#define SMALL_AST_SIZE  16
/* Nb of life at the start */
#define MAX_LIFE        5
/* Points added to the score */
#define BIG_AST_POINT   20
#define MED_AST_POINT  50
#define SMALL_AST_POINT 100

/* Order of the different directions in the picture: */
#define INIT_DIR        9
#define ANGLE_DIR       10

/* Speed of ship */
#define CONS_ACCEL      0.01
#define INIT_DIR 9
#define VIT_MAX 5
#define VIT_PROJ 0.6
#define VIT_BIG_AST 0.1
#define VIT_MED_AST 0.2
#define VIT_SMALL_AST 0.3
#define FROTEMENT   0.001


typedef struct L_ast *l_ast;
typedef struct Sprite_t sprite_t;

struct Sprite_t
{
  int type;
  double x;
  double y;
  int current;
  int size;
  int nb_sprite;
  double vx;
  double vy;
};

struct L_ast
{
  sprite_t first;
  struct L_ast *next;
};

#endif
