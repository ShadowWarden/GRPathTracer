/* events.cpp
*  Omkar H. Ramachandran
*
*  SDL definitions for processing events
*
*/

#include "general.h"

void key_press(SDL_keysym* keysym, Camera *C, int * fps){
        switch(keysym->sym){
                case SDLK_ESCAPE:
                        quit(0);
                        break;
                case SDLK_SPACE:
                        fprintf(stderr,"FPS = %d\n",*fps);
                        break;
                case SDLK_UP:
                case SDLK_KP8:
                        C->Move(-1);
                        is_change = true;
			break;
                case SDLK_DOWN:
                case SDLK_KP2:
                        C->Move(+1);
			is_change = true;
                        break;
                case SDLK_KP4:
                case SDLK_LEFT:
                        C->Move(-2);
			is_change = true;
                        break;
                case SDLK_KP6:
                case SDLK_RIGHT:
                        C->Move(2);
			is_change = true;
                        break;
                case SDLK_p:
                        C->print();
                        break;
                case SDLK_w:
                        C->Zoom(-0.05f);
                        break;
                case SDLK_x:
                        C->Zoom(0.05f);
                        break;
                case SDLK_m:
			mode = (mode==1)?0:1;
		default:
                        break;
        }
}

void process_events(Camera *C,int *fps){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
                switch(event.type){
                        case SDL_KEYDOWN:
                                key_press(&event.key.keysym,C,fps);
                                break;
                        case SDL_QUIT:
                                quit(0);
                                break;
                }
        }
}
