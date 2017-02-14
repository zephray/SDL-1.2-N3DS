#include <3ds.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void fill_audio(void *udata, Uint8 *stream, int len)
{
        /* Only play if we have data left */
        if ( audio_len == 0 )
            return;

        /* Mix as much data as possible */
        len = ( len > audio_len ? audio_len : len );
        SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
        audio_pos += len;
        audio_len -= len;
}

long fileLoad(const char * filename, unsigned char * * buffer) {
    FILE * pFile;
    long lSize;
  
    pFile = fopen(filename, "rb");
    if (pFile != NULL) {
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);
        *buffer = (unsigned char *)malloc(lSize);
        if (buffer != NULL) {
            return fread(*buffer, 1, lSize, pFile);
        }
        fclose(pFile);
    }
    return 0;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Surface *screen;
    SDL_Surface *bitmap;
  
    SDL_Thread *thread1, *thread2;
    int threadReturnValue;
  
    screen = SDL_SetVideoMode(400, 240, 32, SDL_SWSURFACE | SDL_TOPSCR | 
            SDL_CONSOLEBOTTOM);
  
    romfsInit();

    audio_len = fileLoad("romfs:/test.wav", &audio_chunk);
    audio_pos = audio_chunk;
    printf("Ready to play\n");
  
    SDL_AudioSpec audioSpec;
  
    audioSpec.freq = 22050;
    audioSpec.format = AUDIO_S16;
    audioSpec.channels = 2;
    audioSpec.samples = 2048;
    audioSpec.callback = fill_audio;
    audioSpec.userdata = NULL;
  
    if (SDL_OpenAudio(&audioSpec, NULL) < 0) {
        printf("Sound initialization failed.\n");
    }
    SDL_PauseAudio(0);
    while (audio_len > 0) {
        printf("Playing: %lu left \n", audio_len);
        SDL_Delay(1000);
    }
    free(audio_chunk);
    
    SDL_Delay(10000);
    SDL_Quit();
  
    return 0;
}
