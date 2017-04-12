#include "physique.h"


void sprite_init(sprite_t *sprite, int type, SDL_Surface * sprite_picture, int sprite_size, int anim_sprite_num)
{
  sprite->type = type;
  sprite->col = sprite_picture->clip_rect.x;
  sprite->lig = sprite_picture->clip_rect.y;
  sprite->x = sprite_picture->clip_rect.x;
  sprite->y = sprite_picture->clip_rect.y;
  sprite->size = sprite_size;
  sprite->nb_sprite = anim_sprite_num;
  sprite->vx = 0;
  sprite->vy = 0;
  sprite->current = 0;
  switch(type){
  case 0:
    sprite->current = INIT_DIR;
    break;
  case 1:
    app_ast(sprite,sprite_picture);
    break;
  case 2:
    app_ast(sprite,sprite_picture);
    break;
  case 3:
    app_ast(sprite,sprite_picture);
    break;
  default:
    break;}
}

void sprite_turn_left(sprite_t *sprite)
{
  sprite->current += 1;
  if(sprite->current >= sprite->nb_sprite)
    sprite->current = 0;
}

void sprite_turn_right(sprite_t *sprite)
{
  sprite->current -= 1;
  if(sprite->current < 0)
    sprite->current = sprite->nb_sprite - 1;
}

void sprite_move(sprite_t *sprite)
{
  sprite->x += sprite->vx;
  sprite->y += sprite->vy;
  switch(sprite->type){
  case 4:
    if(sprite->x < 0 || sprite->x > SCREEN_WIDTH - sprite->size || sprite->y < 0 || sprite->y > SCREEN_HEIGHT - sprite->size){
      sprite->x = -2;
      sprite->y = -2;}
    break;
  default:
    if(sprite->x < 0)
      sprite->x = sprite->x + SCREEN_WIDTH - sprite->size;
    else if(sprite->x > SCREEN_WIDTH - sprite->size)
      sprite->x =sprite->x - SCREEN_WIDTH + sprite->size;
    if(sprite->y < 0)
      sprite->y = sprite->y + SCREEN_HEIGHT - sprite->size;
    else if(sprite->y > SCREEN_HEIGHT - sprite->size)
      sprite->y =sprite->y - SCREEN_HEIGHT + sprite->size;
    break;}
    sprite->col = sprite->x;
    sprite->lig = sprite->y;
}

void sprite_boost(sprite_t *sprite, float accel)
{
  sprite->vx += accel * cos(sprite->current * 10 * M_PI / 180) - FROTEMENT * sprite->vx;
  sprite->vy += accel * (-sin(sprite->current * 10 * M_PI / 180)) - FROTEMENT * sprite->vy;
  if(abs(sprite->vx) >= abs(VIT_MAX * cos(sprite->current * 10 * M_PI / 180)) && accel > 0)
    sprite->vx -= accel * cos(sprite->current * 10 * M_PI / 180);
  if(abs(sprite->vy) >= abs(VIT_MAX * sin(sprite->current * 10 * M_PI / 180)))
    sprite->vy -= accel * (-sin(sprite->current * 10 * M_PI / 180));
}

void app_project(sprite_t *space_ship, sprite_t *project)
{
  project->current = space_ship->current;
  project->x = (space_ship->x + space_ship->size / 2) + (space_ship->size / 2 * cos(space_ship->current * 10 * M_PI / 180)) - project->size / 2;
  project->y = (space_ship->y + space_ship->size / 2) + (space_ship->size / 2 * (-sin(space_ship->current * 10 * M_PI / 180))) - project->size / 2;
  project->vx = VIT_PROJ * cos(project->current * 10 * M_PI / 180);
  project->vy = VIT_PROJ * (-sin(project->current * 10 * M_PI / 180));
  project->col = project->x;
  project->lig = project->y;
}

void app_ast(sprite_t *asteroid, SDL_Surface *aster)
{
  asteroid->current = rand()%36;
  switch(asteroid->type){
  case 1:
    asteroid->vx = VIT_BIG_AST * cos(asteroid->current * 10 * M_PI / 180);
    asteroid->vy = VIT_BIG_AST * (-sin(asteroid->current * 10 * M_PI / 180));
    break;
  case 2:
    asteroid->vx = VIT_MED_AST * cos(asteroid->current * 10 * M_PI / 180);
    asteroid->vy = VIT_MED_AST * (-sin(asteroid->current * 10 * M_PI / 180));
    break;
  case 3:
    asteroid->vx = VIT_SMALL_AST * cos(asteroid->current * 10 * M_PI / 180);
    asteroid->vy = VIT_SMALL_AST * (-sin(asteroid->current * 10 * M_PI / 180));
    break;}
  asteroid->col = rand()%(SCREEN_WIDTH - asteroid->size);
  asteroid->lig = rand()%(SCREEN_HEIGHT - asteroid->size);
  asteroid->x = asteroid->col;
  asteroid->y = asteroid->lig;
  aster->clip_rect.x = asteroid->col;
  aster->clip_rect.y = asteroid->lig;
}
