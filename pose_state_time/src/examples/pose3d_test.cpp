
#include <cstdlib>
#include "pose3d.h"

int main(int argc, char** argv)
{
      Cpose3d p1, p2, p3;
      int testId = atoi(argv[1]);
      dlib::matrix<double,4,4> hM;
      dlib::matrix<double,6,1> pqv;
      lookAtValues lav;
      
      //test switch
      switch(testId)
      {
            case 1:
                  std::cout << std::endl << "******************** TEST 1 ****************************" << std::endl;

                  p1.pt = 1.,2.,3.;
                  p1.rt.setEuler(20.,30.,-40.,inDEGREES);
                  p1.printPose(inDEGREES);

                  p1.getHmat(hM); 
                  p2.setPose(hM);
                  p2.printPose(inDEGREES);
                  
                  p1.getPtQuat(pqv); 
                  std::cout << "p1.getPtQuat(): " << dlib::trans(pqv) << std::endl;
                  p2.setPose(pqv);
                  p2.printPose(inDEGREES);                  
                  
                  p3.setPose(p1);
                  p3.printPose(inDEGREES);                  
                  
                  p3.rt.setEuler(0.,0.,45.,inDEGREES);
                  p3.getLookAt(lav);
                  p3.printPose(inDEGREES);
                  std::cout <<lav.ex << " " << lav.ey << " " << lav.ez << " " << 
                              lav.ax << " " << lav.ay << " " << lav.az << " " << 
                              lav.ux << " " << lav.uy << " " << lav.uz << std::endl;
                  
                  break;

            case 2:
                  std::cout << std::endl << "******************** TEST 1 ****************************" << std::endl;
                  //motion functions and tranform
                  break;
                  
            default:
                  break;
      
      }
      
      return 0;
}
