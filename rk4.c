/* rk4.c
*  Omkar H. Ramachandran
*
*  4th Order Runge Kutta implementation for solving an ODE.
*  ux' = -3/2*x/(x**2+y**2)**3
*  uy' = -3/2*y/(x**2+y**2)**3
*  dx/dt = ux
*  dt/dt = uy
*  f(t,ux,x,y)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define E 2.718281828459045
#define h 1.414

double uxp(double x, double y){
	return -3/2.0*h*h*x/pow(x*x+y*y,3);
}

double uyp(double x, double y){
	return -3/2.0*h*h*y/pow(x*x+y*y,3);
}

double xp(double ux){
	return ux;
}

double yp(double uy){
	return uy;
}

double * rk4(double(*f1)(double, double),
	double(*f2)(double, double),
	double(*g1)(double),
 	double(*g2)(double),
	double dt, double t, 
	double ux, double uy,
	double x, double y){
//	printf("%g\t%g\t%g\t%g\n",dt,t,u1,u2);
	double	k11 = dt * f1(x, y),
		k12 = dt * f2(x, y),
		k13 = dt * g1(ux),
		k14 = dt * g2(uy);
	double * sol = (double *) malloc (sizeof(double)*4); 
	sol[0] = ux + k11;
	sol[1] = uy + k12;
	sol[2] = x + k13;
	sol[3] = y + k14;
//	sol[1] = u2 + (k12+2*k22+2*k32+k42)/6;	
//	printf("%g\t%g\n",sol[0],sol[1]);
	return sol;
}

int main(int argc, char ** argv){
	double *u1, *u2, t, y1, y2;
	double x0 = 0, x1 = 10, dx = 0.025;
	int i, n = 1 + (x1 - x0)/dx;
	double *ux = malloc(sizeof(double) * n);
	double *uy = malloc(sizeof(double) * n);
	double *x = malloc(sizeof(double) * n);
	double *y = malloc(sizeof(double) * n); 
	double xinit = atof(argv[1]);

	for (ux[0] = 0, uy[0] = 1, x[0] = -xinit, y[0] = -5, i = 1; i < n; i++){
		double *sol;
		sol = rk4(uxp,uyp,xp,yp,dx,x0 + dx * (i - 1), ux[i-1], uy[i-1], x[i-1], y[i-1]);
 		ux[i] = sol[0];
		uy[i] = sol[1];	
		x[i] = sol[2];
		y[i] = sol[3];
		free(sol);
		if(sqrt(x[i]*x[i]+y[i]*y[i])<1){
			n = i;
			break;
		}
	}
	for (i = 0; i < n; i ++) {
		t = x0 + dx * i;
//		y2 = 1/3.0*pow(E,5*x)+2/3.0*pow(E,-x)+x*x*pow(E, 2*x);
		printf("%g\t%g\t%g\t%g\t%g\n", t, x[i],y[i],ux[i],uy[i]);
	}
 
	free(x);
	free(y);
	free(ux);
	free(uy);
	return 0;
}
