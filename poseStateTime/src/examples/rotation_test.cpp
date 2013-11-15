
#include "rotation.h"


int main(int argc, char** argv)
{
      Crotation rt1, rt2, rt3;
      Cquaternion qt1;
      dlib::matrix<double,3,3> rM1;
      dlib::matrix<double,3,1> axis;
      double angle;
      unsigned ii,jj,kk;
      double euH, euP, euR; //euler angles;
      
      std::cout << std::endl << "******************** TEST 1 ****************************" << std::endl;
      rt1.setEuler(45,60,30,inDEGREES);
      rt1.print();
      
      qt1.set(0.822363,0.02226,0.531976,0.200562);
      rt2.setQuaternion(qt1);
      rt2.print();
      std::cout << "rt2.head: " << rt2.head(inDEGREES) << "; rt2.pitch: " << rt2.pitch(inDEGREES) << "; rt2.roll: " << rt2.roll(inDEGREES) <<std::endl;
      
      rM1 = 0.353553,-0.306186,0.883884, 0.353553,0.918559,0.176777, -0.866026,0.25,0.433012;
      rt3.setMatrix(rM1);
      rt3.print();
      
      std::cout << std::endl << "******************** TEST 2 ****************************" << std::endl;
      rt1.setEuler(0,0,0);
      rt1.print();
      rt1.rotateUaxis(30,0,0,1, inDEGREES);
      rt1.print();
      rt1.getAxisAngle(axis,angle);
      std::cout << "axis: " << dlib::trans(axis);
      std::cout << "angle: " << angle << std::endl;
      
      std::cout << std::endl << "******************** TEST 3 ****************************" << std::endl;
      for(ii=1; ii<=60; ii++)//heading, (-pi,pi]
      {
            euH = -M_PI+ii*(2*M_PI/60);
            for(jj=1; jj<=30; jj++)//pitch, (-pi/2,pi/2]
            {
                  euP = -M_PI/2+jj*(M_PI/30);
                  for(kk=1; kk<=60; kk++)//roll, (-pi,pi]
                  {
                        euR = -M_PI+kk*(2*M_PI/60);
                        rt1.setEuler(euH,euP,euR);
                        rt1.getMatrix(rM1);
                        rt1.getQuaternion(qt1);
                        //std::cout << rM1;
                        std::cout << qt1.qq;
                  }
            }
      }
      
      
      
      return 0;
}
