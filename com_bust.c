#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "physique.h"

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
  SDL_Surface *screen, *temp, *sprite, *background, *project, *big_ast,
    *med_ast, *small_ast;
  int colorkey;
  sprite_t space_ship, projectile, big_asteroid, med_asteroid, small_asteroid;
  
  /* Information about the current situation of the ship: */
  
  /* Rectangle to store the position of the sprite in the window.
   * Only the x and y coordinates are used. */
  SDL_Rect spritePosition;
  
  
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  
  /* set the title bar */
  SDL_WM_SetCaption("Comete Buster", "Comete Buster");
  
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
  spritePosition.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
  sprite->clip_rect.x = spritePosition.x;
  spritePosition.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;
  sprite->clip_rect.y = spritePosition.y;
  sprite_init(&space_ship,0,sprite,SPRITE_SIZE,NB_SPRITE);
  project->clip_rect.x = -1;
  project->clip_rect.y = -1;
  sprite_init(&projectile,4,project,PROJECT_SIZE,1);
  big_ast->clip_rect.x = 0;
  big_ast->clip_rect.y = 0;
  sprite_init(&big_asteroid, 1, big_ast, BIG_AST_SIZE, 32);
  med_ast->clip_rect.x = 64;
  med_ast->clip_rect.y = 0;
  sprite_init(&med_asteroid, 2, med_ast, MED_AST_SIZE, 32);
  small_ast->clip_rect.x = 96;
  small_ast->clip_rect.y = 0;
  sprite_init(&small_asteroid, 2, small_ast, SMALL_AST_SIZE, 32);
  
  int gameover = 0;
  int anim = 0;
  int temps = 0;
  /* Define the float position of the ship */
  
  /* main loop: check events and re-draw the window until the end */
  while (!gameover)
    {
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
      if (SDL_PollEvent(&event)) {
	HandleEvent(event, &gameover, &space_ship, &projectile, &accel);
      }
      sprite_boost(&space_ship, accel);
      sprite_move(&space_ship);
      /* collide with edges of screen */
      spritePosition.x = space_ship.col;
      spritePosition.y = space_ship.lig;
      sprite_move(&projectile);

      /* draw the background */
      SDL_BlitSurface(background, NULL, screen, NULL);
      /* Draw the selected image of the sprite at the right position */
	/* Define the source rectangle for the BlitSurface */
      {
	SDL_Rect spriteImage;
	spriteImage.y = 0;
	spriteImage.w = space_ship.size;
	spriteImage.h = space_ship.size;
	/* choose image according to direction and animation flip: */
	spriteImage.x = space_ship.size * space_ship.current;
	
	SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition);
      }
      {
	if(projectile.x>=0){
	  spritePosition.x = projectile.col;
	  spritePosition.y = projectile.lig;
	  SDL_Rect projImage;
	  projImage.x = 0;
	  projImage.y = 0;
	  projImage.w = projectile.size;
	  projImage.h = projectile.size;
	  
	  SDL_BlitSurface(project, &projImage, screen, &spritePosition);}
      }
      {
	spritePosition.x = big_asteroid.col;
	spritePosition.y = big_asteroid.lig;
	SDL_Rect bastImage;
	bastImage.x = anim * big_asteroid.size;
	bastImage.y = 0;
	bastImage.w = big_asteroid.size;
	bastImage.h = big_asteroid.size;

	SDL_BlitSurface(big_ast, &bastImage, screen, &spritePosition);
      }
      {
	spritePosition.x = med_asteroid.col;
	spritePosition.y = med_asteroid.lig;
	SDL_Rect mastImage;
	mastImage.x = anim * med_asteroid.size;
	mastImage.y = 0;
	mastImage.w = med_asteroid.size;
	mastImage.h = med_asteroid.size;

	SDL_BlitSurface(med_ast, &mastImage, screen, &spritePosition);
      }
      {
	spritePosition.x = small_asteroid.col;
	spritePosition.y = small_asteroid.lig;
	SDL_Rect sastImage;
	sastImage.x = anim * small_asteroid.size;
	sastImage.y = 0;
	sastImage.w = small_asteroid.size;
	sastImage.h = small_asteroid.size;

	SDL_BlitSurface(small_ast, &sastImage, screen, &spritePosition);
      }
      /* update the screen */
      SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
  
  /* clean up */
  SDL_FreeSurface(sprite);
  SDL_FreeSurface(background);
  SDL_Quit();
  
  return 0;
}
