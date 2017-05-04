#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "camera.h"

#include <cstdlib>
#include <cmath>

#define DEGtoRAD 0.0174533

const int d = 10;

extern int mode;
extern bool is_change;

struct pixel{
	GLubyte r,g,b;
};

// sphere.cpp
void Vertex(double,double,double,double,double);
void sphere(double,double,double,double,bool=false,bool=false);

// events.cpp
void quit(int code);
void key_press(SDL_keysym*, Camera *, int *);
void process_events(Camera *,int *);

// render.cpp
void render_init();
void draw_screen(SDL_Surface *, Camera, int *, GLuint);
void setup_opengl(int,int);

// main.cpp
int FramesPerSecond(void);
