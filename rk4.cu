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
#include <sys/time.h>
#include <sys/types.h>

#define Nruns 1
#define E 2.71828182845904
#define DIM 10

double calctime(struct timeval start, struct timeval end){
        double time = 0.0;
        time = end.tv_usec - start.tv_usec;
        time = time/1000000.0;
        time += end.tv_sec - start.tv_sec;

        return time;
}


__global__ void euler(float * ux, float * uy, float * x, float * y, int * n){
//	printf("%g\t%g\t%g\t%g\n",dt,t,u1,u2);
	const float h = 1.414;
	const float dt = 0.1;
	int i = blockIdx.x*1024 + threadIdx.x;
	int j=0;
	while(j!=n[0]){
		float	k11 = dt * (-3/2.0*h*h*x[i]/pow(x[i]*x[i]+y[i]*y[i],3)),
			k12 = dt * (-3/2.0*h*h*y[i]/pow(x[i]*x[i]+y[i]*y[i],3)),
			k13 = dt * ux[i],
			k14 = dt * uy[i];
			ux[i] += k11;
			uy[i] += k12;
			x[i] += k13;
			y[i] += k14;
		if(sqrt(x[i]*x[i]+y[i]*y[i])<1){
	//		x[i] -= k13;
	//		y[i] -= k14;
			break;
		}else if(sqrt(x[i]*x[i]+y[i]*y[i])>10){
	//		x[i] -= k13;
	//		y[i] -= k14;
			break;
		}
		j++;
	}
//	sol[1] = u2 + (k12+2*k22+2*k32+k42)/6;	
//	printf("%g\t%g\n",sol[0],sol[1]);
}

int main(int argc, char ** argv){
	double x0 = 0, x1 = 10, dx = 0.1;
	int n = 1 + (x1 - x0)/dx;
	int i;
	int N = 1024;
	float *ux = (float *) malloc(sizeof(float)*N);
	float *uy = (float *) malloc(sizeof(float)*N);
	float *x = (float *) malloc(sizeof(float)*N);
	float *y = (float *) malloc(sizeof(float)*N); 

        double time;
        struct timeval start;
        struct timeval end;

	for(i=0;i<N;i++){	
		ux[i] = 0;
		uy[i] = 1;	
		x[i] = -4.0/N*i;
		y[i] = -5.0;
	}

	float * dev_ux, * dev_uy, * dev_x, * dev_y;
	int * dev_n;

	cudaMalloc((void **) &dev_ux, sizeof(float)*N);
	cudaMalloc((void **) &dev_uy, sizeof(float)*N);
	cudaMalloc((void **) &dev_x, sizeof(float)*N);
	cudaMalloc((void **) &dev_y, sizeof(float)*N);
	cudaMalloc((void **) &dev_n, sizeof(int));

	cudaMemcpy(dev_ux,ux,sizeof(float)*N,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_uy,uy,sizeof(float)*N,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_x,x,sizeof(float)*N,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_y,y,sizeof(float)*N,cudaMemcpyHostToDevice);
	cudaMemcpy(dev_n,&n,sizeof(int),cudaMemcpyHostToDevice);

	gettimeofday(&start, NULL);
	euler<<<1,N>>>(dev_ux,dev_uy,dev_x,dev_y,dev_n);
	gettimeofday(&end, NULL);

	cudaMemcpy(ux,dev_ux,sizeof(float)*N,cudaMemcpyDeviceToHost);
	cudaMemcpy(uy,dev_uy,sizeof(float)*N,cudaMemcpyDeviceToHost);
	cudaMemcpy(x,dev_x,sizeof(float)*N,cudaMemcpyDeviceToHost);
	cudaMemcpy(y,dev_y,sizeof(float)*N,cudaMemcpyDeviceToHost);
//	cudaMemcpy(&n,dev_n,sizeof(int),cudaMemcpyDeviceToHost);


	cudaFree(dev_ux);
	cudaFree(dev_uy);
	cudaFree(dev_x);
	cudaFree(dev_y);
	cudaFree(dev_n);
	
	time = calctime(start,end);
	
	printf("(Nruns = %d) Calctime: %lf\n",Nruns,time);	
	for(i=0;i<N;i++)
		printf("%g\t%g\t%g\t%g\n", ux[i], uy[i], x[i], y[i]);
 
	free(x);
	free(y);
	free(ux);
	free(uy);
	return 0;
}
