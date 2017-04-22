obj=triangles.o camera.o object.o

Triangle: triangles.o camera.o object.o
	g++ -Wall -std=c++11 -O3 -o bhfs triangles.o camera.o object.o main.cpp -lSDL -lglut -lGLU -lGL

camera.o: camera.cpp
	g++ -Wall -std=c++11 -O3 -c camera.cpp

object.o: object.cpp
	g++ -Wall -std=c++11 -O3 -c object.cpp

triangles.o: triangles.cpp
	g++ -Wall -std=c++11 -O3 -c triangles.cpp


.PHONY: clean
clean:
	rm -f $(obj) ./bhfs .*.swp
