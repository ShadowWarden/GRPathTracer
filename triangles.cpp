/* triangles.cpp
 *  
 *  Omkar H. Ramachandran
 *  omkar.ramachandran@colorado.edu
 *
 *  Simple rotating triangle
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include "triangles.h"

Triangle::Triangle(float vert[18]){
	int i,j;
	for(i=0;i<6;i++){
		if(i%2==0){
			for(j=0;j<3;j++)
				color[i/2][j] = vert[i*3+j];
		}else{
			for(j=0;j<3;j++)
				vertex[i/2][j] = vert[i*3+j];
		}
	}
}

Triangle::Triangle(){
	// fill in later	
}

void Triangle::print(){
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			fprintf(stderr,"%f ",color[i][j]);
		}
		fprintf(stderr,"\n");
	}
}

void Triangle::render(double beta){
/* Each triangle is oriented along the x axis, so just lorrentz transforming
*  along the triangle's local x axis is enough!
*  The transform is as follows: (Forget t till I get a better idea)
*  x' -> gamma*x
*  y' -> y
*  z' -> z
*/
	double gamma = sqrt(1/(1-beta*beta));
	glColor3f(color[0][0],color[0][1],color[0][2]);
	glVertex3f(vertex[0][0]*gamma,vertex[0][1],vertex[0][2]);
	glColor3f(color[1][0],color[1][1],color[1][2]);
	glVertex3f(vertex[1][0]*gamma,vertex[1][1],vertex[1][2]);
	glColor3f(color[2][0],color[2][1],color[2][2]);
	glVertex3f(vertex[2][0]*gamma,vertex[2][1],vertex[2][2]);
}
