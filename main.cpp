/* main.cpp
 *
 *  Omkar H. Ramachandran
 *  omkar.ramachandran@colorado.edu
 *
 *  Rotating triangle : OpenGL + SDL
 */

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "object.h"

#include <cstdlib>

#define DEGtoRAD 0.0174533

static int fps=0,sec0=0,count=0;
int FramesPerSecond(void){
	int sec = glutGet(GLUT_ELAPSED_TIME)/1000;
	if (sec!=sec0){
		sec0 = sec;
		fps = count;
		count=0;
	}
	count++;
	return fps;
}


static void quit(int code){
	SDL_Quit();
	exit(code);
}

static void key_press(SDL_keysym* keysym, Object *T, Camera *C, int * fps){
	switch(keysym->sym){
		case SDLK_ESCAPE:
			quit(0);
			break;
		case SDLK_SPACE:
			fprintf(stderr,"FPS = %d\n",*fps);
			break;
		case SDLK_KP_PLUS:
		case SDLK_PLUS:
			T->ChangeSpeed(5.0f);
			break;
		case SDLK_KP_MINUS:
		case SDLK_MINUS:
			T->ChangeSpeed(-5.0f);
			break;
		case SDLK_i:
			T->print();
			break;
		case SDLK_UP:
		case SDLK_KP8:
			C->Move(-1);
			break;
		case SDLK_DOWN:
		case SDLK_KP2:
			C->Move(+1);
			break;
		case SDLK_KP4:
		case SDLK_LEFT:
			C->Move(-2);
			break;
		case SDLK_KP6:
		case SDLK_RIGHT:
			C->Move(2);
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
		case SDLK_COMMA:
			C->Changebeta(-0.1f);
			break;
		case SDLK_PERIOD:
			C->Changebeta(0.1f);
			break;
		default:
			break;
	}
}

static void process_events(Object *T,Camera *C,int *fps){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				key_press(&event.key.keysym,T,C,fps);
				break;
			case SDL_QUIT:
				quit(0);
				break;
		}
	}
}

static void render_init(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
}

static void draw_screen(Object *T,Camera C,int * fps, int nObj){
	int i;
	float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
	float th = C.Getth();
	float ph = C.Getph();
	float r = C.Getr();

	float Eye_x = r*sin(th*DEGtoRAD)*cos(ph*DEGtoRAD);
	float Eye_y = r*sin(th*DEGtoRAD)*sin(ph*DEGtoRAD);
	float Eye_z = r*cos(th*DEGtoRAD);

	float Up_z = (th>=180.0f && th<360.0f)?-1.0f:1.0f;

	gluLookAt(Eye_x,Eye_y,Eye_z,0.0,0.0,0.0,0.0,0.0,Up_z);

	for(i=0;i<nObj;i++){
		T[i].prepare(time);
		T[i].render(C);
	}
	*fps = FramesPerSecond();
	SDL_GL_SwapBuffers();
}

static void setup_opengl( int width, int height ){
	float ratio = (float) width / (float) height;

	/* Our shading model--Gouraud (smooth). */
	glShadeModel( GL_SMOOTH );

	/* Culling. */
	//glCullFace( GL_BACK );
	//glFrontFace( GL_CCW );
	//glEnable( GL_CULL_FACE );

	/* Set the clear color. */
	glClearColor( 0, 0, 0, 0 );

	/* Setup our viewport. */
	glViewport( 0.0, 0.0, width, height );

	/*
	 * Change to the projection matrix and set
	 * our viewing volume.
	 */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	/*
	 * EXERCISE:
	 * Replace this with a call to glFrustum.
	 */
	gluPerspective( 45.0f, ratio, 1.0f, 100.0f );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char ** argv){
	const SDL_VideoInfo* info = NULL;

	int width = 0;
	int height = 0;
	/* Colour depth */
	int bpp = 0;
	int flags = 0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "Video init failed: %s\n",SDL_GetError());
		quit(1);
	}

	info = SDL_GetVideoInfo();

	if(!info){
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		quit(1);
	}

	width = 800;
	height = 600;
	bpp = info->vfmt->BitsPerPixel;

	flags = SDL_OPENGL;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0){
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		quit(1);
	}

	SDL_WM_SetCaption("Black Hole RayTracer","BHRT");

	Object *T = new Object[2];
	T[0] = Object("solid_triangle.in");
	T[1] = Object("solid_triangle_2.in");
	T[1].ChangeSpeed(-10.0f);
	Camera C;
	int fps;
	setup_opengl(width, height);

	while(1){
		render_init();
		process_events(T,&C,&fps);
		draw_screen(T,C,&fps,2);
	}

	return 0;
}
