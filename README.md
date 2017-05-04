CSCI 4239 : Omkar H. Ramachandran : Final Project

Black Hole Raytracer:

This is a simulation of a schwarzchild (spherically symmetric mass distribution) black hole. I used CUDA (main.cu line 88) to compute the geodesics for light travelling from the camera and hitting the skybox.

Controls : 
Arrow Keys : Move in theta and phi.
m : Change mode from Newtonian to Relativistic
Spacebar Displays FPS in console
p : Displays theta and phi angles.


The code needs CUDA to be installed to run. It also needs libSDL1.2.
The warnings shown on compilation are redundant (the only ones on my machine are that certain statements will never be accessed. This is by design).

The frame rate is quite low in GR mode (the bottleneck seems to be the comptation itself)

I was really proud of working out a shortcut in computing the geodesics for the light rays. I honestly can't believe that I actually got it to work xD
