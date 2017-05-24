#include "physique.h"


void sprite_init(sprite_t *sprite, int type, SDL_Surface * sprite_picture, int sprite_size, int anim_sprite_num)
{
  sprite->type = type;
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
  case 4:
    break;
  case 1:
    sprite->current = rand() % 36;
    sprite->vx = VIT_BIG_AST * cos(sprite->current * 10 * M_PI / 180);
    sprite->vy = VIT_BIG_AST * (-sin(sprite->current * 10 * M_PI / 180));
    break;
  case 2:
    sprite->current = rand() % 36;
    sprite->vx = VIT_MED_AST * cos(sprite->current * 10 * M_PI / 180);
    sprite->vy = VIT_MED_AST * (-sin(sprite->current * 10 * M_PI / 180));
    break;
  case 3:
    sprite->current = rand() % 36;
    sprite->vx = VIT_SMALL_AST * cos(sprite->current * 10 * M_PI / 180);
    sprite->vy = VIT_SMALL_AST * (-sin(sprite->current * 10 * M_PI / 180));
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
  case 0:
    if(sprite->x < 0)
      sprite->x = sprite->x + SCREEN_WIDTH - sprite->size;
    else if(sprite->x > SCREEN_WIDTH - sprite->size)
      sprite->x =sprite->x - SCREEN_WIDTH + sprite->size;
    if(sprite->y < 0)
      sprite->y = sprite->y + SCREEN_HEIGHT - sprite->size;
    else if(sprite->y > SCREEN_HEIGHT - sprite->size)
      sprite->y =sprite->y - SCREEN_HEIGHT + sprite->size;
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
    break;
  }
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
}

void app_ast(l_ast *aster, SDL_Surface *big, SDL_Surface *med,
	     SDL_Surface *small)
{
  sprite_t ast;
  srand(time(NULL));
  int type = rand() % 3 + 1;
  int start = rand() % 2;
  int size;
  SDL_Rect pos_dep;
  SDL_Surface tmp;
  switch(type){
  case 1:
    tmp = *big;
    size = BIG_AST_SIZE;
    break;
  case 2:
    tmp = *med;
    size = MED_AST_SIZE;
    break;
  default:
    tmp = *small;
    size = SMALL_AST_SIZE;
    break;}
  switch(start){
  case 0:
    pos_dep.x = 0;
    pos_dep.y = rand() % (SCREEN_HEIGHT - size);
    break;
  default:
    pos_dep.x = rand() % (SCREEN_WIDTH - size);
    pos_dep.y = 0;
    break;}
  tmp.clip_rect.x = pos_dep.x;
  tmp.clip_rect.y = pos_dep.y;
  sprite_init(&ast ,type ,&tmp ,size , 32);
  *aster = l_ast_cons(ast, *aster);
}

void div_ast(sprite_t ast, l_ast *rest, sprite_t project, int *points)
{
  sprite_t tmp1, tmp2;
  tmp1 = ast;
  tmp2 = ast;
  switch(ast.type){
  case 1:
    tmp1.type = 2;
    tmp2.type = 2;
    tmp1.size = MED_AST_SIZE;
    tmp2.size = MED_AST_SIZE;
    tmp1.current = project.current + 9;
    if(tmp1.current > 36)
      tmp1.current -= 36;
    else
      if(tmp1.current < 0)
	tmp1.current += 36;
    tmp2.current = project.current - 9;
    if(tmp2.current > 36)
      tmp2.current -= 36;
    else
      if(tmp2.current < 0)
	tmp2.current += 36;
    tmp1.vx = VIT_MED_AST * cos(tmp1.current * M_PI * 10 / 180);
    tmp1.vy = VIT_MED_AST * (-sin(tmp1.current * M_PI * 10 / 180));
    tmp2.vx = VIT_MED_AST * cos(tmp2.current * M_PI * 10 / 180);
    tmp2.vy = VIT_MED_AST * (-sin(tmp2.current * M_PI * 10 / 180));
    tmp1.x += (BIG_AST_SIZE - MED_AST_SIZE)/2;
    tmp1.y += (BIG_AST_SIZE - MED_AST_SIZE)/2;
    tmp2.x += (BIG_AST_SIZE - MED_AST_SIZE)/2;
    tmp2.y += (BIG_AST_SIZE - MED_AST_SIZE)/2;
    *rest = l_ast_cons(tmp2, *rest);
    *rest = l_ast_cons(tmp1, *rest);
    *points += BIG_AST_POINT;
    break;
  case 2:
    tmp1.type = 3;
    tmp2.type = 3;
    tmp1.size = SMALL_AST_SIZE;
    tmp2.size = SMALL_AST_SIZE;
    tmp1.current = project.current + 9;
    if(tmp1.current > 36)
      tmp1.current -= 36;
    else
      if(tmp1.current < 0)
	tmp1.current += 36;
    tmp2.current = project.current - 9;
    if(tmp2.current > 36)
      tmp2.current -= 36;
    else
      if(tmp2.current < 0)
	tmp2.current += 36;
    tmp1.vx = VIT_SMALL_AST * cos(tmp1.current * M_PI * 10 / 180);
    tmp1.vy = VIT_SMALL_AST * (-sin(tmp1.current * M_PI * 10 / 180));
    tmp2.vx = VIT_SMALL_AST * cos(tmp2.current * M_PI * 10 / 180);
    tmp2.vy = VIT_SMALL_AST * (-sin(tmp2.current * M_PI * 10 / 180));
    tmp1.x += (MED_AST_SIZE - SMALL_AST_SIZE)/2;
    tmp1.y += (MED_AST_SIZE - SMALL_AST_SIZE)/2;
    tmp2.x += (MED_AST_SIZE - SMALL_AST_SIZE)/2;
    tmp2.y += (MED_AST_SIZE - SMALL_AST_SIZE)/2;
    *rest = l_ast_cons(tmp2, *rest);
    *rest = l_ast_cons(tmp1, *rest);
    *points += MED_AST_POINT;
    break;
  default:
    *points += SMALL_AST_POINT;
    break;}
}

bool proj_contact(sprite_t *project, sprite_t *ast)
{
  SDL_Rect center_proj, center_ast;
  double rayon_proj, rayon_ast;
  int i, j;
  center_proj.x = project->x + project->size/2;
  center_proj.y = project->y + project->size/2;
  rayon_proj = project->size/2;
  center_ast.x = ast->x + ast->size/2;
  center_ast.y = ast->y + ast->size/2;
  rayon_ast = ast->size/2;
  for(j = project->y ; j <= project->y + project->size ; j++){
    for(i = project->x ; i <= project->x + project->size ; i++){
      if((pow(center_proj.x - i,2) + pow(center_proj.y - j,2)) <= pow(rayon_proj,2) && (pow(center_ast.x - i,2) + pow(center_ast.y - j,2)) <= pow(rayon_ast,2)){
	return true;
      }
    }
  }
  return false;
}

void proj_contact_list(sprite_t *project, l_ast *ast, int *points)
{
  sprite_t tmp_sp;
  l_ast tmp = l_ast_copy(*ast);
  l_ast tmp2 = l_ast_new_empty();
  bool cont = false;
  while(!cont && !l_ast_is_empty(tmp)){
    tmp_sp = l_ast_pop(&tmp);
    cont = proj_contact(project, &tmp_sp);
    if(cont)
      div_ast(tmp_sp, &tmp2, *project, points);
    else
      tmp2 = l_ast_cons(tmp_sp, tmp2);
  }
  if(cont){
    project->x = -2;
    project->y = -2;}
  while(!l_ast_is_empty(tmp2))
    tmp = l_ast_cons(l_ast_pop(&tmp2),tmp);
  *ast = l_ast_copy(tmp);
}

bool ship_contact(sprite_t *ship, sprite_t *ast)
{
  SDL_Rect center_ship, center_ast;
  double rayon_ship, rayon_ast;
  int i, j;
  center_ship.x = ship->x + ship->size/2;
  center_ship.y = ship->y + ship->size/2;
  rayon_ship = ship->size/2;
  center_ast.x = ast->x + ast->size/2;
  center_ast.y = ast->y + ast->size/2;
  rayon_ast = ast->size/2;
  for(j = ship->y ; j <= ship->y + ship->size ; j++){
    for(i = ship->x ; i <= ship->x + ship->size ; i++){
      if((pow(center_ship.x - i,2) + pow(center_ship.y - j,2)) <= pow(rayon_ship,2) && (pow(center_ast.x - i,2) + pow(center_ast.y - j,2)) <= pow(rayon_ast,2)){
	return true;
      }
    }
  }
  return false;
}

bool ship_contact_list(sprite_t *ship, l_ast *ast)
{
  l_ast tmp = l_ast_copy(*ast);
  bool crash = false;
  sprite_t tmp_sp;
  while(!l_ast_is_empty(tmp) && !crash){
    tmp_sp = l_ast_pop(&tmp);
    crash = ship_contact(ship, &tmp_sp);}
  l_ast_free(&tmp);
  return crash;
}

void ast_move(l_ast *aster)
{
  l_ast tmp_ast = l_ast_new_empty();
  l_ast tmp = l_ast_copy(*aster);
  sprite_t tmp_sp;
  while(!l_ast_is_empty(tmp)){
    tmp_sp = l_ast_pop(&tmp);
    sprite_move(&tmp_sp);
    tmp_ast = l_ast_cons(tmp_sp, tmp_ast);}
  l_ast_reverse(&tmp_ast);
  *aster = l_ast_copy(tmp_ast);
  l_ast_free(&tmp_ast);
  l_ast_free(&tmp);

}
