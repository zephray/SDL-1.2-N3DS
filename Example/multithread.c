#include <3ds.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

static int testThread(void *ptr)
{
    int cnt;

    for (cnt = 0; cnt < 10; ++cnt) {
        printf("Thread %d counter: %d\n", SDL_ThreadID(), cnt);
        SDL_Delay(50);
    }

    return cnt;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Surface *screen;
    
    SDL_Thread *thread1;
    int threadReturnValue;
  
    screen = SDL_SetVideoMode(400, 240, 32, SDL_SWSURFACE | SDL_TOPSCR | 
            SDL_CONSOLEBOTTOM); //Init console
          
    thread1 = SDL_CreateThread(testThread, (void *)NULL);
  
    for (int i = 0; i < 5; i++) {
        printf("Main thread\n");
        SDL_Delay(50);
    }
  
    if (thread1 == NULL) {
        printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
    } else {
        SDL_WaitThread(thread1, &threadReturnValue);
        printf("\nThread returned value: %d", threadReturnValue);
    }
  
    SDL_Delay(10000);
    SDL_Quit();
  
    return 0;
}
