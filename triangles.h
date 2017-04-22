/* triangle.h
*
*  Omkar H. Ramachandran
*  omkar.ramachandran@colorado.edu
*  
*  Simple rotating triangle
*/
#include <cstdio>
#include <cmath>

class Triangle{
	public:
		Triangle(float vertex[18]);
		Triangle();
		void render(double);
		void print();
	private:
		float vertex[3][3];
		float color[3][3];
};
