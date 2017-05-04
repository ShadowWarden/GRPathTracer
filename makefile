obj=camera.o sphere.o events.o render.o

bhfs: $(obj) main.cu
	nvcc -o bhfs $(obj) main.cu -lSDL -lglut -lGLU -lGL -lSDL_image -lm

camera.o: camera.cpp
	g++ -Wall -std=c++11 -O3 -c camera.cpp

sphere.o: sphere.cpp
	g++ -Wall -std=c++11 -O3 -c sphere.cpp

events.o: events.cpp
	g++ -Wall -std=c++11 -O3 -c events.cpp

render.o: render.cpp
	g++ -Wall -std=c++11 -O3 -c render.cpp

.PHONY: clean
clean:
	rm -f $(obj) ./bhfs .*.swp .*.swo
