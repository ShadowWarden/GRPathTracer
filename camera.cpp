/* camera.cpp
 *  
 *  Omkar H. Ramachandran
 *  omkar.ramachandran@colorado.edu
 *
 *  Simple rotating triangle
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include "camera.h"

// Amount by which the camera will move in any direction - in degrees
const int move_amt = 5.0f;

Camera::Camera(){
	th = 45.0f;
	ph = 225.0f;
	r = 5.0f;
	beta_x = 0.6; 
}

float Camera::Getr(){
	return r;
}

void Camera::Move(int dir){
	switch(dir){
		case 1: // +th
			th += move_amt; 
			if(th>360.0f){
				th-=360.0f;
			}	
			break;
		case -1: // -th
			th -= move_amt;
			if(th<=0.0f){
				th+=360.0f;
			}
			break;
		case 2: // ph
			ph += move_amt;
			if(ph>=360.0f){
				ph-=360.0f;
			}	
			break;
		case -2: // -ph
			ph -= move_amt;
			if(ph<=0.0f){
				ph+=360.0f;
			}

			break;
		default:
			break;
	}	
}

void Camera::Zoom(float dr){
	r += dr;	
}

void Camera::Changebeta(float dr){
	if((beta_x>=0.0 && beta_x+dr <= 1.0) || 
	(beta_x<=0.0 && beta_x+dr >= -1.0)){
		beta_x += dr;
		fprintf(stderr,"Beta = %f\n",beta_x);
	}else
		fprintf(stderr,"Beta approaching 1!\n");
}

void Camera::print(){
	fprintf(stderr,"Theta = %f ; Phi = %f\n",th,ph);
}

float Camera::Getth(){
	return th;
}

float Camera::Getph(){
	return ph;
}

double Camera::Getbeta(){
	return beta_x;
}
