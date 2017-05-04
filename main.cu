/* main.cu
 *
 *  Omkar H. Ramachandran
 *  omkar.ramachandran@colorado.edu
 *
 *  Rotating triangle : OpenGL + SDL
 */

#include "general.h"

int fps=0,sec0=0,count=0;
int width = 640;
int height = 480;
int size = width*height;
int mode = 0; // 0 = Newton, 1 = Einstein
int Mode; // RGB vs RGBA
bool is_change = true;

float *ux,*uy,*uz,*x,*y,*z;

float *dev_ux,*dev_uy,*dev_uz,*dev_x,*dev_y,*dev_z;

Uint32 * pixels;

void quit(int code){
	cudaFree(dev_ux);
	cudaFree(dev_uy);
	cudaFree(dev_uz);
	cudaFree(dev_x);
	cudaFree(dev_y);
	cudaFree(dev_z);

	free(ux);
	free(uy);
	free(uz);
	free(x);
	free(y);
	free(z);

	free(pixels);

	SDL_Quit();
	exit(code);
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

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

__global__ void ray_solver(float * ux, float * uy, float *uz, float * x, float * y, float *z){
	// Using pretested values of h and dt
	const float h = 1.414;
	const float dt = 0.5;
	// Use multiple blocks in 1D with 1024 threads in each
	int i = blockIdx.x*1024 + threadIdx.x;

	int size = 640*480;

	/* Euler method solver - Step size is not a limiting factor
	 * here, so I'm not bothering with implementing rk4 */
	if(i<size){
		//	int flag = 0;
		float r, powr6;
		while(1){
			float r = sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]);
			float powr6 = pow(r,6);


			if(r<1){
				/* If the radius is less than 1, i.e the ray falls into
				 *  the black hole, then stop */
				break;
			}else if(r>10){
				/* If ray hits skydome, then stop */
				break;
			}

			float cost = z[i]/r;
			float   k11 = dt * (-3/2.0*h*h*sqrt(1-cost*cost)*x[i]/powr6),
				k12 = dt * (-3/2.0*h*h*sqrt(1-cost*cost)*y[i]/powr6),
				k13 = dt * (-3/2.0*h*h*z[i]/powr6),	
				k14 = dt * ux[i],
				k15 = dt * uy[i],
				k16 = dt * uz[i];
				ux[i] += k11;
				uy[i] += k12;
				uz[i] += k13;
				x[i] += k14;
				y[i] += k15;
				z[i] += k16;

		}
	}
}

void draw_screen(SDL_Surface *surface, Camera C,int * fps, GLuint texture){
	//        int i;
	float time = 0.001*glutGet(GLUT_ELAPSED_TIME);
	float th = C.Getth();
	float ph = C.Getph();
	float r = C.Getr();

	float Eye_x = r*sin(th*DEGtoRAD)*cos(ph*DEGtoRAD);
	float Eye_y = r*sin(th*DEGtoRAD)*sin(ph*DEGtoRAD);
	float Eye_z = r*cos(th*DEGtoRAD);


	if(mode == 0){
		float Up_z = (th>=180.0f && th<360.0f)?-1.0f:1.0f;

		gluLookAt(Eye_x,Eye_y,Eye_z,0.0,0.0,0.0,0.0,0.0,Up_z);

		// Render black hole
		sphere(0.0,0.0,0.0,1.0);
		// Render skydome
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture);
		sphere(1.0f,1.0f,1.0f,10.0f,false,true);
		glDisable(GL_TEXTURE_2D);
	}else if(mode == 1){
		int i,j,index;
		float ratio = width/height;
		float dphi = 90*ratio;
		float dtheta = 90;
		float delphi = dphi/width;
		float deltheta = dtheta/height;


		if(is_change){
			// Raytraced black hole image
			for(index=0;index<height;index++){
				for(j=0;j<width;j++){
					i = width*index + j;
					ux[i] = 0;
					uy[i] = 0;
					uz[i] = 1;
					x[i] =  -5.0+10.0/(width-1)*j;
					y[i] = -5.0+10.0/(height-1)*index;
					z[i] = -5.0;
				}
			}


			cudaMemcpy(dev_x,x,sizeof(float)*size,cudaMemcpyHostToDevice);
			cudaMemcpy(dev_y,y,sizeof(float)*size,cudaMemcpyHostToDevice);
			cudaMemcpy(dev_z,z,sizeof(float)*size,cudaMemcpyHostToDevice);
			cudaMemcpy(dev_ux,ux,sizeof(float)*size,cudaMemcpyHostToDevice);
			cudaMemcpy(dev_uy,uy,sizeof(float)*size,cudaMemcpyHostToDevice);
			cudaMemcpy(dev_uz,uz,sizeof(float)*size,cudaMemcpyHostToDevice);

			ray_solver<<<size/1024,1024>>>(dev_ux,dev_uy,dev_uz,dev_x,dev_y,dev_z);

			cudaMemcpy(x,dev_x,sizeof(float)*size,cudaMemcpyDeviceToHost);
			cudaMemcpy(y,dev_y,sizeof(float)*size,cudaMemcpyDeviceToHost);
			cudaMemcpy(z,dev_z,sizeof(float)*size,cudaMemcpyDeviceToHost);
			is_change = false;
		}
		/* We now have x, y and z. Calculate theta and phi for each
		 *  and print pixel
		 */
		for(index=0;index<height;index++){
			for(j=0;j<width;j++){
				i = width*index+j;	
				float r = sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]);
				//		printf("%f\t%f\t%f\t\n",x[i],y[i],z[i]);
				if(r>10.0){
					// Ray actually hit the skydome
					float theta = acos(z[i]/r);
					if(theta>3.141592){
						theta -= 3.141592;
					}
					float phi = atan2(y[i],x[i]);
					phi += 3.14159265;
					int xcord = (phi/3.141592/2.0*surface->w);
					int ycord = (theta/3.141592*surface->h);
					pixels[i] = get_pixel32(surface,xcord,ycord);
					//	pixels[i] = 1;
					//					if(!flag){
					//						printf("%d\n",xcord);
					//					}
				}else if(r<1.0){
					pixels[i] = 0;
				}
			}
		}

		glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
	}
	*fps = FramesPerSecond();
	SDL_GL_SwapBuffers();
}

int main(int argc, char ** argv){
	const SDL_VideoInfo* info = NULL;

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

	ux = (float *) malloc(sizeof(float)*size);
	uy = (float *) malloc(sizeof(float)*size);
	uz = (float *) malloc(sizeof(float)*size);
	x = (float *) malloc(sizeof(float)*size);
	y = (float *) malloc(sizeof(float)*size);
	z = (float *) malloc(sizeof(float)*size);



	cudaMalloc((void **) &dev_ux, sizeof(float)*size);
	cudaMalloc((void **) &dev_uy, sizeof(float)*size);
	cudaMalloc((void **) &dev_uz, sizeof(float)*size);
	cudaMalloc((void **) &dev_x, sizeof(float)*size);
	cudaMalloc((void **) &dev_y, sizeof(float)*size);
	cudaMalloc((void **) &dev_z, sizeof(float)*size);



	pixels = new Uint32[height*width];
	// Load image
	//	GLuint texture;
	GLuint TextureID = 0;

	// You should probably use CSurface::OnLoad ... ;)
	//-- and make sure the Surface pointer is good!
	SDL_Surface* Surface = IMG_Load("7415.jpg");

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	Mode = GL_RGB;

	//	int BPP = Surface->format->BytesPerPixel;

	if(Surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	printf("surface->w: %d, surface->h = %d\n",Surface->w,Surface->h);


	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	printf("Pixel : %d\n",get_pixel32(Surface,10,10));

	Camera C;
	int fps;
	setup_opengl(width, height);

	while(1){
		render_init();
		process_events(&C,&fps);
		draw_screen(Surface,C,&fps,TextureID);
	}
	quit(0);
	return 0;
}
