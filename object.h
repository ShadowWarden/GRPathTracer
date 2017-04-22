/* triangle.h
*
*  Omkar H. Ramachandran
*  omkar.ramachandran@colorado.edu
*  
*  Simple rotating triangle
*/

#include "camera.h"
#include "triangles.h"

class Object{
	public:
		Object(const char *);
		Object();

		bool init();
		void prepare(float dt);
		void render(Camera C);
		void shutdown();

		void onResize(int width, int height);
		void print();
		void ChangeSpeed(float);
		float GetAngle();
	private:
		int N = 1;
		Triangle *T;
		float m_rotationAngle;
		float speed;
		bool flag;
		char *fname;
		bool show_axes;
};
