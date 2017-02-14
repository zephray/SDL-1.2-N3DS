#include <3ds.h>
#include <stdio.h>
#include "SDL/SDL.h"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen;
    SDL_Surface *bitmap;
  
    screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE | SDL_TOPSCR | 
            SDL_CONSOLEBOTTOM);
  
    Result rs = romfsInit();
    if (rs)
        printf("romfsInit: %08lx\n", rs);
    else {
        printf("romfs Initialization succeed.\n");
        bitmap = SDL_LoadBMP("romfs:/test.bmp");
        if (bitmap == NULL)
            printf("Open Bitmap failed!\n");
        else
            SDL_BlitSurface(bitmap, NULL, screen, NULL);
    }
    printf("Should be bilted\n");
    SDL_Flip(screen);
    printf("Should be flipped\n");
    SDL_Delay(2000);
    SDL_Quit();
  
    return 0;
}
