# GRPathTracer
Raytraced rendering of a Schwarzchild Black Hole

![alt text](https://github.com/ShadowWarden/GRPathTracer/blob/master/screenshots/picture2.png)

## Instructions for Compiling
You need the SDL1.2, freeglut and CUDA libraries to compile. To install the first two on ubuntu, run the following:

apt-get install libsdl1.2-dev libsdl-image1.2-dev freeglut3-dev

Installing CUDA is a pain in the neck, unfortunately. You need to install both the Nvidia driver and the CUDA libraries. Instructions to do this are provided here : http://docs.nvidia.com/cuda/cuda-installation-guide-linux/#axzz4g7polGbZ

Once these packages are installed, run make to compile and ./bhfs to run the program

Note : I just about finished writing this, so I haven't had to chance to do any kind of optimization to increase FPS. In general, the following will make the program run a little faster: 
- Decrease the resolution (i.e width, height) in main.cu (You have to change int width and int height in line 12 and inside the CUDA kernel in line 95)
- If you're really ambitious, try putting a pragma in front of the pixel rendering loop in line 206 of main.cu.
The main bottlenecks are the computation itself (one way of speeding this up is to use rk4 and reduce the step size), the cudaMemcpys - which I can get rid off in the future with a compute shader or even a fragment shader - and the pixel rendering (which can almost certainly be made much faster)

I managed to get 15 frames per second on my system while actively raytracing (Nvidia GeForce 820M, 640x480, openmp pragma with 8 threads) compared to 1100 without the raytracing... Yeah, that's steep

Once in the program, use 'm' to switch between Newtonian and Relativistic modes. Newtonian rendering just uses raster operations and is thus super fast.
Note : I think I goofed on the sign of phi, so left arrow will take you right and vice versa in the GR mode.

Arrow keys change theta and phi (distance to the Black Hole is fixed in GR mode)

'p' prints out current theta and phi for the camera

Spacebar prints the frame rate
