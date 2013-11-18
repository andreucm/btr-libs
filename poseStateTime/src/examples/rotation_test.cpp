
#include <cstdlib>
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
      int testId = atoi(argv[1]);
      
      //test switch
      switch(testId)
      {
            case 1:
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
                  break;
      
            case 2:
                  std::cout << std::endl << "******************** TEST 2 ****************************" << std::endl;
                  rt1.setEuler(0,0,0);
                  rt1.print();
                  rt1.rotateUaxis(30,0,0,1, inDEGREES);
                  rt1.print();
                  rt1.getAxisAngle(axis,angle);
                  std::cout << "axis: " << dlib::trans(axis);
                  std::cout << "angle: " << angle << std::endl;
                  break;

            case 3:
                  std::cout << std::endl << "******************** TEST 3 ****************************" << std::endl;
                  rt1.setEuler(-M_PI+M_PI/100,-M_PI/2+M_PI/100,-M_PI+M_PI/100);
                  rt1.getMatrix(rM1);
                  rt1.getQuaternion(qt1);
                  std::cout << "rM1: " << std::endl << rM1;
                  std::cout << "qt1: " << std::endl << qt1.qq << std::endl;
                  
                  rt1.setEuler(M_PI-M_PI/100,M_PI/2-M_PI/100,M_PI-M_PI/100);
                  rt1.getMatrix(rM1);
                  rt1.getQuaternion(qt1);
                  std::cout << "rM1: " << std::endl << rM1;
                  std::cout << "qt1: " << std::endl << qt1.qq << std::endl;
                  
                  rM1 = 0,0,1,0,1,0,-1,0,0;
                  rt1.setMatrix(rM1);
                  rt1.getQuaternion(qt1);
                  std::cout << "rM1: " << std::endl << rM1;
                  std::cout << "qt1: " << std::endl << qt1.qq;
                  std::cout << "h: " << rt1.head(inDEGREES) << "; p: " << rt1.pitch(inDEGREES) << "; r: " << rt1.roll(inDEGREES) <<std::endl;
                  break;
                  
            case 4:
                  std::cout << std::endl << "******************** TEST 4 ****************************" << std::endl;
                  for(ii=1; ii<=10; ii++)//heading, (-pi,pi]
                  {
                        euH = -M_PI+ii*(2*M_PI/10);
                        for(jj=1; jj<=5; jj++)//pitch, (-pi/2,pi/2]
                        {
                              euP = -M_PI/2+jj*(2*M_PI/10);
                              for(kk=1; kk<=10; kk++)//roll, (-pi,pi]
                              {
                                    euR = -M_PI+kk*(2*M_PI/10);
                                    rt1.setEuler(euH,euP,euR);
                                    rt1.getMatrix(rM1);
                                    rt1.getQuaternion(qt1);
                                    std::cout << rM1;
                                    std::cout << qt1.qq;
                              }
                        }
                  }
                  break;
      
            default:
                  break;
      
      }
      
      return 0;
}
