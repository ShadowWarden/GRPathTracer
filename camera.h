/* camera.h
*
*  Omkar H. Ramachandran
*  omkar.ramachandran@colorado.edu
*  
*  Simple rotating triangle
*/

class Camera{
	public:
		Camera();

		void Move(int dir);
		float Getth();
		float Getph();
		void print();
		void Zoom(float dr);
		void Changebeta(float dr);
		float Getr();
		double Getbeta();
	private:
		float th;
		float ph;
		float r;
	// Radial velocity
		double beta_x;
};
