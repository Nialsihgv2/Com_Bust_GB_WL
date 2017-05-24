#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "struct.h"
#include "lis_ast.h"
#include "physique.h"


/* Handle events coming from the user:
   - quit the game?
   - use left/right to change the orientation of the ship
   - use up to move on the ship to the right direction
*/


void update_events(char *keys, int *gameover)
{
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch (event.type){
    case SDL_QUIT:
      *gameover = 1;
      break;
    case SDL_KEYUP:
      keys[event.key.keysym.sym] = 0;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
      case SDLK_q:
	*gameover = 1;
	break;
      default:
	break;
      }
      keys[event.key.keysym.sym] = 1;
      break;
    default:
      break;
    }
  }
}

void alternative_HandleEvent(char *key, double *accel, sprite_t *ship, sprite_t *project)
{
  SDLKey tabkey[5] = {SDLK_UP,SDLK_DOWN,SDLK_LEFT,SDLK_RIGHT,SDLK_SPACE};
  if(key[tabkey[0]])
    *accel = CONS_ACCEL;
  if(key[tabkey[1]])
    *accel = - CONS_ACCEL;
  if(key[tabkey[2]])
    sprite_turn_left(ship);
  if(key[tabkey[3]])
    sprite_turn_right(ship);
  if(key[tabkey[4]]){
      if(project->x < 0)
	app_project(ship, project);
  }
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  SDL_Surface *screen, *temp, *sprite, *background, *project, *big_ast,
    *med_ast, *small_ast, *expl, *number;
  int colorkey;
  sprite_t space_ship, projectile;  
  /* Information about the current situation of the ship: */
  
  /* Rectangle to store the position of the sprite in the window.
   * Only the x and y coordinates are used. */
  SDL_Rect spritePosition;
  
  
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  
  /* set the title bar */
  SDL_WM_SetCaption("Comet Busters", "Comet Busters");
  
  /* create window */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
  
  /* set keyboard repeat */
  SDL_EnableKeyRepeat(10, 10);
  
  /* load sprite */
  temp   = SDL_LoadBMP("greenship-v1.bmp");
  sprite = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  
  /* setup sprite colorkey and turn on RLE */
  colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  temp = SDL_LoadBMP("explosion17.bmp");
  expl = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  SDL_SetColorKey(expl, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  temp = SDL_LoadBMP("number.bmp");
  number = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  SDL_SetColorKey(number, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  temp = SDL_LoadBMP("bullet02.bmp");
  project = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  colorkey = SDL_MapRGB(screen->format, 255, 125, 0);
  SDL_SetColorKey(project, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  temp = SDL_LoadBMP("asteroid-model1-32_64x64.bmp");
  big_ast = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  colorkey = SDL_MapRGB(screen->format, 0, 255, 255);
  SDL_SetColorKey(big_ast, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  temp = SDL_LoadBMP("asteroid-model1-32_32x32.bmp");
  med_ast = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  SDL_SetColorKey(med_ast, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  temp = SDL_LoadBMP("asteroid-model1-32_16x16.bmp");
  small_ast = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  SDL_SetColorKey(small_ast, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  
  /* load background */
  temp  = SDL_LoadBMP("backgroundlvl1.bmp");
  background = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  
  /* set sprite position in the middle of the window */
  project->clip_rect.x = -1;
  project->clip_rect.y = -1;
  sprite_init(&projectile,4,project,PROJECT_SIZE,1);
  
  int gameover = 0;
  int anim = 0;
  int temps = 0;
  time_t actual;
  time_t app = time(NULL);
  time_t app_aster = time(NULL);
  time_t death;
  bool appear = true;
  bool inv = true;
  bool dest = false;
  bool crash;
  int anim_exp;
  l_ast aster = l_ast_new_empty();
  int vies = MAX_LIFE;
  int points = 0;
  char key[SDLK_LAST] = {0};
  int time_k = 0;
  
  /* Define the float position of the ship */
  
  /* main loop: check events and re-draw the window until the end */
  while (!gameover)
    {
      crash = false;
      actual = time(NULL);
      if(dest && (difftime(actual, death) >= 3)){
	dest = false;
	appear = true;
	vies -= 1;}
      if(vies == 0)
	gameover = 1;
      if(inv && (difftime(actual, app) >= 3))
	inv = false;
      if(!dest && appear){
	spritePosition.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
	sprite->clip_rect.x = spritePosition.x;
	spritePosition.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;
	sprite->clip_rect.y = spritePosition.y;
	sprite_init(&space_ship,0,sprite,SPRITE_SIZE,NB_SPRITE);
	appear = false;
	inv = true;
	app = time(NULL);}
      if(difftime(actual, app_aster) >= 5){
	app_ast(&aster, big_ast, med_ast, small_ast);
	app_aster = time(NULL);}
      

      double accel=0.0;
      temps += 1;
      if(temps >= 100){
	anim += 1;
	if(anim >= 32){
	  anim = 0;}
	temps = 0;}
	
      /* look for an event; possibly update the position and the shape
       * of the sprite. */
      if(!dest && time_k % 20 == 0){
	update_events(key, &gameover);
	alternative_HandleEvent(key, &accel, &space_ship, &projectile);
      }
      time_k ++;
      sprite_boost(&space_ship, accel);
      sprite_move(&space_ship);
      if(!l_ast_is_empty(aster)){
	ast_move(&aster);}
      /* collide with edges of screen */
      spritePosition.x = space_ship.x;
      spritePosition.y = space_ship.y;
      sprite_move(&projectile);
      if(projectile.x >= 0)
	proj_contact_list(&projectile, &aster, &points);
      if(!dest && !l_ast_is_empty(aster) && !inv)
	crash = ship_contact_list(&space_ship, &aster);
      if(crash){
	death = time(NULL);
	anim_exp = 0;
	space_ship.vx = 0;
	space_ship.vy = 0;
	dest = true;}

      /* draw the background */
      SDL_BlitSurface(background, NULL, screen, NULL);
      /* Draw the selected image of the sprite at the right position */
	/* Define the source rectangle for the BlitSurface */
      if(!dest)
      {
	{SDL_Rect spriteImage;
	spriteImage.y = 0;
	spriteImage.w = space_ship.size;
	spriteImage.h = space_ship.size;
	/* choose image according to direction and animation flip: */
	spriteImage.x = space_ship.size * space_ship.current;
	
	SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition);}
      }
      else{
	if(anim_exp/50 < 25){
	  {SDL_Rect spriteImage;
	    spriteImage.y = 0;
	    spriteImage.w = 64;
	    spriteImage.h = 64;
	    /* choose image according to direction and animation flip: */
	    spriteImage.x = anim_exp/50 * 64;
	    spritePosition.x = space_ship.x - 16;
	    spritePosition.y = space_ship.y - 16;
	    
	    SDL_BlitSurface(expl, &spriteImage, screen, &spritePosition);
	    anim_exp += 1;}
	}
      }
      {
	if(projectile.x>=0){
	  spritePosition.x = projectile.x;
	  spritePosition.y = projectile.y;
	  SDL_Rect projImage;
	  projImage.x = 0;
	  projImage.y = 0;
	  projImage.w = projectile.size;
	  projImage.h = projectile.size;
	  
	  SDL_BlitSurface(project, &projImage, screen, &spritePosition);}
      }
      {
	if(!l_ast_is_empty(aster)){
	  l_ast tmp = l_ast_copy(aster);
	  sprite_t tmp_sp;
	  SDL_Rect astImage;
	  while(!l_ast_is_empty(tmp)){
	    tmp_sp = l_ast_pop(&tmp);
	    spritePosition.x = tmp_sp.x;
	    spritePosition.y = tmp_sp.y;
	    astImage.x = anim * tmp_sp.size;
	    astImage.y = 0;
	    astImage.w = tmp_sp.size;
	    astImage.h = tmp_sp.size;
	    switch(tmp_sp.type){
	    case 1:
	      SDL_BlitSurface(big_ast, &astImage, screen, &spritePosition);
	      break;
	    case 2:
	      SDL_BlitSurface(med_ast, &astImage, screen, &spritePosition);
	      break;
	    case 3:
	      SDL_BlitSurface(small_ast, &astImage, screen, &spritePosition);
	      break;
	    default:
	      break;}
	  }
	  l_ast_free(&tmp);
	}
      }
      /* update the screen */
  for(int i=0;i<vies-1;i++){
    {
      SDL_Rect spriteImage;
      spriteImage.x = 0;
      spriteImage.y = 0;
      spriteImage.w = space_ship.size;
      spriteImage.h = space_ship.size;
      /* choose image according to direction and animation flip: */
      spritePosition.x = i * space_ship.size;
      spritePosition.y = 0;
      SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition);
    }
  }
  int tmp1 = points;
  int tmp2;
  for(int i=1;i<=5;i++){
    {
      tmp2 = tmp1 % 10;
      tmp1 = tmp1 / 10;
      SDL_Rect pointImage;
      pointImage.x = tmp2 * 16;
      pointImage.y = 0;
      pointImage.w = 16;
      pointImage.h = 16;

      spritePosition.x = SCREEN_WIDTH - 16 * i;
      spritePosition.y = 0;
      SDL_BlitSurface(number, &pointImage, screen, &spritePosition);}
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}
  /* clean up */
  SDL_FreeSurface(sprite);
  SDL_FreeSurface(background);
  SDL_FreeSurface(project);
  SDL_FreeSurface(big_ast);
  SDL_FreeSurface(med_ast);
  SDL_FreeSurface(small_ast);
  l_ast_free(&aster);
  SDL_Quit();
  return 0;
}
