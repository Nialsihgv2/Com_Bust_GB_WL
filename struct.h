#ifndef STRUCT_H
#define STRUCT_H

/* Dimensions of screen */
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

/* Number of sprite for the space ship */
#define NB_SPRITE       36

/* Size of each object */
#define SPRITE_SIZE     32
#define PROJECT_SIZE    8
#define BIG_AST_SIZE    64
#define MED_AST_SIZE    32
#define SMALL_AST_SIZE  16

/* Number of life */
#define MAX_LIFE        5

/* Points of each asteroid */
#define BIG_AST_POINT   20
#define MED_AST_POINT  50
#define SMALL_AST_POINT 100

/* Direction of space ship on start */
#define INIT_DIR        9

/* Variation between each angle */
#define ANGLE_DIR       10

/* Boost of ship */
#define CONS_ACCEL      0.01

/* Max speed of ship */
#define VIT_MAX 5

/* speed of each object */
#define VIT_PROJ 0.6
#define VIT_BIG_AST 0.1
#define VIT_MED_AST 0.2
#define VIT_SMALL_AST 0.3

#define FROTEMENT   0.001


typedef struct L_ast *l_ast;
typedef struct Sprite_t sprite_t;

/* Definition of sprite */
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

/* Definition of a list of asteroids */
struct L_ast
{
  sprite_t first;
  struct L_ast *next;
};

#endif
