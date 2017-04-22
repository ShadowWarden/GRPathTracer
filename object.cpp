/* triangles.cpp
 *  
 *  Omkar H. Ramachandran
 *  omkar.ramachandran@colorado.edu
 *
 *  Simple rotating triangle
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <cmath>
#include "object.h"

#define PI 3.14159265
#define DEGtoRAD 0.0174533
/* Fix radius to 1.0 */
//const float r = 2.0f;

float * read(FILE * fin, int N){
	float *vert = new float[N*18];
	int i;
	for(i=0;i<N*18;i++){
		int err = fscanf(fin,"%f",vert+i);
		if(err == 0)
			fprintf(stderr,"Unable to read float %d\n",i);
	}
	return vert;
}

int getsize(FILE * fin){
	fseek(fin,SEEK_SET,0);
	int n;
	if(fscanf(fin,"%d",&n)==0)
		fprintf(stderr,"Unable to read int size\n");
	return n;
}

Object::Object(){
}

Object::Object(const char *fname){
	FILE *fin = fopen(fname,"r");
	N = getsize(fin);
	float *vert = read(fin,N);
	T = new Triangle[N];
	int i;
	for(i=0;i<N;i++){
		T[i] = Triangle(&vert[i*18]);
	}	
	m_rotationAngle = 0.0f;
	speed = 0.0f;
	flag = false;
	show_axes = false;
}

bool Object::init(){
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f,0.5f,0.5f,0.5f);

	return true;
}

void Object::prepare(float dt){
	if(!flag)
		m_rotationAngle += speed*dt;
	if(m_rotationAngle > 360.0f){
		m_rotationAngle = m_rotationAngle-360.0f;
	}
}

void Object::render(Camera C){
	glRotatef(m_rotationAngle, 0.0f, 0.0f, 1.0f);
	double beta = C.Getbeta();

	int i;
	glBegin(GL_TRIANGLES);
	/* Lorrentz transform along x. For now assume that this is the 
	*  only direction in which the velocity exists.
	*/	
		for(i=0;i<N;i++){	
			T[i].render(beta);
		}
	glEnd();

	if(show_axes){
		glBegin(GL_LINES);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(1.0f,0.0f,0.0f);
			glColor3f(0.0f,1.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,1.0f,0.0f);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,1.0f);
		glEnd();
	}
	m_rotationAngle = 0;
}

void Object::onResize(int width, int height){
	glViewport(0,0,width,height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, float(width)/float(height), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Object::print(){
	fprintf(stderr,"speed = %f ; m_rotationAngle = %f\n",speed,m_rotationAngle);
}

float Object::GetAngle(){
	return m_rotationAngle;
}

void Object::ChangeSpeed(float dv){
	speed += dv;
	//flag = true;
}
