
#include <cstdlib>
#include "pose3d.h"

int main(int argc, char** argv)
{
      Cpose3d p1, p2, p3;
      int testId = atoi(argv[1]);
      dlib::matrix<double,4,4> hM;
      dlib::matrix<double,6,1> pqv;
      lookAtValues lav;
      dlib::matrix<double,3,1> pt1;
      unsigned int ii;
      
      //test switch
      switch(testId)
      {
            case 1:
                  std::cout << std::endl << "******************** TEST 1 ****************************" << std::endl;
                  //general set/get 
                  
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
                  std::cout << std::endl << "******************** TEST 2 ****************************" << std::endl;
                  //Euler functions in gimbal lock rotations
                  hM =  0, 1, 0, -2,
                        0, 0, -1, 3, 
                        -1, 0, 0, 4, 
                        0, 0, 0, 1;
                  p1.setPose(hM);
                  p1.printPose();
                  
                  p2.pt = -2.,3.,4.;
                  p2.rt.setEuler(-90,90,0,inDEGREES);
                  p2.printPose();

                  p2.pt = -2.,3.,4.;
                  p2.rt.setEuler(0,90,90,inDEGREES);
                  p2.printPose();
                  
                  break;

            case 3:
                  std::cout << std::endl << "******************** TEST 3 ****************************" << std::endl;
                  //motion functions

                  p1.pt = 1.,2.,3.;
                  p1.rt.setEuler(-20.,30.,-40.,inDEGREES);
                  p1.printPose(inDEGREES);
                  p1.moveForward(2.77);
                  p1.printPose(inDEGREES);
                  
                  break;

            case 4:
                  std::cout << std::endl << "******************** TEST 4 ****************************" << std::endl;
                  //transform functions

                  p1.pt = 1.,1.,1.;
                  p1.rt.setEuler(45.,45.,45.,inDEGREES);
                  pt1 = 0.5,1,-1;
                  for (ii=1; ii<=500; ii++)
                  {
                        pt1 = 0.5,1,-1;
                        p1.moveForward(0.05*cos(ii/100.));
                        p1.rt.rotateUaxis(2,-ii/100.,ii/50.,-ii/200., inDEGREES);
                        p1.printPose(inDEGREES,false);
                        //std::cout << pt1(0) << " " << pt1(1) << " " << pt1(2) << " ";
                        p1.trf(pt1, FROM_THIS_TO_REF);
                        std::cout << " " << pt1(0) << " " << pt1(1) << " " << pt1(2) << std::endl;
                  }
                  
                  break;
                  
            default:
                  break;
      
      }
      
      return 0;
}
