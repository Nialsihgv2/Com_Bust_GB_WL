#include <SDL.h>
#include <math.h>

/* Size of the window */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
/* Size of the grass texture picture */


/* In the sprite, we have 8 images of a 32x32 picture,
 * 2 images for each direction. */
/* Size of one image: */
#define SPRITE_SIZE     32
/* Order of the different directions in the picture: */
#define DIR_UP          0
#define DIR_RIGHT       0
#define DIR_DOWN        2
#define DIR_LEFT        3
#define NB_SPRITE       36
#define SPEED_SPRITE    10
#define ANGLE_DIR       10
#define INIT_DIR        9
#define ANGLE_DIRECTION -3

/* Number of pixels for one step of the sprite */
#define SPRITE_STEP     5

/* Handle events coming from the user:
        - quit the game?
        - use of arrows to move the sprite
   In the second case, we modify the current direction and the
   position of the sprite in the window.
   We also change the animation bit used for creating the "walk" effect.
   */
void HandleEvent(SDL_Event event,
		 int *quit, int *currDirection, double *Positionx, double *Positiony)
{
    double  dirx, diry;
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
		    *currDirection += 1;
                    //position->x -= SPRITE_STEP;
                    break;
                case SDLK_RIGHT:
                    *currDirection -= 1;
                    //position->x += SPRITE_STEP;
                    break;
                case SDLK_UP:
		  //*currDirection = DIR_UP;
		  // position->y -= SPRITE_STEP;
		    dirx=cos((ANGLE_DIR * *currDirection * M_PI) / 180);
		    diry=-sin((ANGLE_DIR * *currDirection * M_PI) / 180);
		    *Positionx = *Positionx + SPEED_SPRITE * dirx;
		    *Positiony = *Positiony + SPEED_SPRITE * diry;

                    break;
                case SDLK_DOWN:
		  //*currDirection = DIR_DOWN;
		  //position->y += SPRITE_STEP;
		  *Positionx = *Positionx + SPEED_SPRITE * dirx;
		  *Positiony = *Positiony + SPEED_SPRITE * diry;

                    break;
                default:
                    break;
            }
            break;
    }
}

int main(int argc, char* argv[])
{
    SDL_Surface *screen, *temp, *sprite, *grass;
    int colorkey;

    /* Information about the current situation of the sprite: */
    int currentDirection = INIT_DIR;
    /* Walking in a direction is animated with 2 images; we use a boolean
     * that we flip after each movement to show them in turn and create
     * the illusion */


    /* Rectangle to store the position of the sprite in the window.
     * Only the x and y coordinates are used. */
    SDL_Rect spritePosition;


    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("SDL Animation", "SDL Animation");

    /* create window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 10);

    /* load sprite */
    temp   = SDL_LoadBMP("vaisseau.bmp");
    sprite = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* setup sprite colorkey and turn on RLE */
    colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* load grass */
    temp  = SDL_LoadBMP("backg.bmp");
    grass = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* set sprite position in the middle of the window */
    spritePosition.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
    spritePosition.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;

    int gameover = 0;
    double Positionx, Positiony;
    Positionx = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
    Positiony = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;

        /* look for an event; possibly update the position and the shape
         * of the sprite. */
        if (SDL_PollEvent(&event)) {
	  HandleEvent(event, &gameover, &currentDirection, &Positionx, &Positiony);
        }

        /* collide with edges of screen */
        if (Positionx < 0)
           Positionx = SCREEN_WIDTH - SPRITE_SIZE ;
	else
        if (Positionx > SCREEN_WIDTH - SPRITE_SIZE) 
            Positionx = 0;

	spritePosition.x = Positionx;
	if (Positiony < 0)
           Positiony = SCREEN_HEIGHT - SPRITE_SIZE ;
	else
        if (Positiony > SCREEN_HEIGHT - SPRITE_SIZE) 
            Positiony = 0;
	spritePosition.y = Positiony;

        if (spritePosition.y < 0)
            spritePosition.y = SCREEN_HEIGHT - SPRITE_SIZE;
        if (spritePosition.y > SCREEN_HEIGHT - SPRITE_SIZE) 
            spritePosition.y = 0;

	if (currentDirection < 0)
	  currentDirection = NB_SPRITE - 1;
	if (currentDirection > NB_SPRITE)
	  currentDirection = 0;

        /* draw the background */
	SDL_BlitSurface(grass, NULL, screen, NULL);


        /* Draw the selected image of the sprite at the right position */
        {
            /* Define the source rectangle for the BlitSurface */
            SDL_Rect spriteImage;
            spriteImage.y = 0;
            spriteImage.w = SPRITE_SIZE;
            spriteImage.h = SPRITE_SIZE;
            /* choose image according to direction and animation flip: */
            spriteImage.x = SPRITE_SIZE * currentDirection ;

            SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition);
        }

        /* update the screen */
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    /* clean up */
    SDL_FreeSurface(sprite);
    SDL_FreeSurface(grass);
    SDL_Quit();

    return 0;
}
