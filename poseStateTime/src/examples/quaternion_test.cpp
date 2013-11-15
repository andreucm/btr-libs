
#include "quaternion.h"


int main(int argc, char** argv)
{
	//test 1
	Cquaternion qq1(0.5,0.5,0.5,0.5);
	qq1.conjugate();
	qq1.print();
	qq1.qq(0)=0;
	qq1.normalize();
	qq1.print();
	std::cout << "qq1(1): " << qq1(1) << std::endl;
	
	//test product
	Cquaternion qq2(0.2,-0.3,-0.5,0.1);	
	qq2.normalize();
	Cquaternion qq3(-0.1,-0.2,0.1,0.1);
	qq3.normalize();
	Cquaternion qR;
	qq2.product(qq3,qR);
	std::cout << "qqR: "; qR.print(); std::cout << std::endl;
      qR = qq2*qq3;
      std::cout << "qqR: "; qR.print(); std::cout << std::endl;
      	
	//rot matrix
	dlib::matrix<double,3,3> rM1, rM2;
	qq1.qq(2) = -0.2;
	qq1.normalize();
	qq1.print();
	rM1 = qq1.getMatrix();
	qq1.getMatrix(rM2);
	std::cout << rM1 << rM2;
	
	//3 argument constructor
	dlib::matrix<double,3,1> vq;
	vq = 0,0,0.1;
	Cquaternion qq4(vq);
	qq4.print();
	qq4.set(0,0.1,0);
	qq4.print();
      
      dlib::matrix<double,3,3> rM3;
      rM3 = -1,0,0,0,-1,0,0,0,1;
      qq4.getMatrix(rM3);
      std::cout << rM3 << std::endl;
      qq4.print();
	
	return 0;
}
