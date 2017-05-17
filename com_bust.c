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
void HandleEvent(SDL_Event event,
		 int *quit, sprite_t *ship, sprite_t *project, double *accel)
{
  switch (event.type) {
    /* close button clicked */
  case SDL_QUIT:
    *quit = 1;
    break;
    
    /* handle the keyboard */
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
    case SDLK_q:
      *quit = 1;
      break;
    case SDLK_LEFT:
      sprite_turn_left(ship);
      break;
    case SDLK_RIGHT:
      sprite_turn_right(ship);
      break;
    case SDLK_UP:
      *accel = CONS_ACCEL;
      break;
    case SDLK_DOWN:
      *accel = - CONS_ACCEL;
      break;
    case SDLK_SPACE:
      if(project->x < 0)
	app_project(ship, project);
      break;
    default:
      break;
    }
    break;
  }
  //if(event.type == SDLK_SPACE && project->x!=-1){
  //  app_project(ship, project);}
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  SDL_Surface *screen, *temp, *sprite, *background, *project, *big_ast,
    *med_ast, *small_ast, *expl;
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
      SDL_Event event;
      temps += 1;
      if(temps >= 100){
	anim += 1;
	if(anim >= 32){
	  anim = 0;}
	temps = 0;}
	
      /* look for an event; possibly update the position and the shape
       * of the sprite. */
      if(!dest){
	if (SDL_PollEvent(&event)) {
	  HandleEvent(event, &gameover, &space_ship, &projectile, &accel);}
      }
      sprite_boost(&space_ship, accel);
      sprite_move(&space_ship);
      if(!l_ast_is_empty(aster)){
	ast_move(&aster);}
      /* collide with edges of screen */
      spritePosition.x = space_ship.x;
      spritePosition.y = space_ship.y;
      sprite_move(&projectile);
      if(projectile.x >= 0)
	proj_contact_list(&projectile, &aster);
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
