/* sphere.cpp
*  Omkar H. Ramachandran
*  omkar.ramachandran@colorado.edu
*
*  Functions for rendering skydome
*
*/

#include "general.h"

void Vertex(double th,double ph,double R, double G, double B){
	glColor3f(R , G , B);
	glVertex3d(sin(th*DEGtoRAD)*cos(ph*DEGtoRAD) , sin(th*DEGtoRAD)*sin(ph*DEGtoRAD) , cos(th*DEGtoRAD));
}


void sphere(double R,double G,double B,double r, bool wireframe, bool use_texture){
	int th,ph;

	//  Save transformation
	glPushMatrix();
	//  Offset and scale
	//        glTranslated(x,y,z);
	glScaled(r,r,r);

	//  Latitude bands
	for (th=0;th<180;th+=d)
	{
		if(wireframe)
			glBegin(GL_LINES);
		else
			glBegin(GL_QUAD_STRIP);
		for (ph=0;ph<=360;ph+=d){
			double t,s;
			if(use_texture){
				s = (ph)/360.0;
				t = (th)/180.0;
				glTexCoord2f(s,t);
			}
			Vertex(th,ph,R,G,B);
			if(use_texture){
				s = (ph)/360.0;
				t = (th+d)/180.0;
				glTexCoord2f(s,t);
			}
			Vertex(th+d,ph,R,G,B);
		}
		glEnd();
	}

	//  Undo transformations
	glPopMatrix();
}

