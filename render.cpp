/* render.cpp
 * Omkar H. Ramachandran
 * omkar.ramachandran@colorado.edu
 *
 * Functions for setting up the renderer 
 */


#include "general.h"

void render_init(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
}

void setup_opengl( int width, int height ){
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

	gluPerspective( 45.0f, ratio, 1.0f, 100.0f );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
