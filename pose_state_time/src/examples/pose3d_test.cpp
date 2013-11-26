
#include <cstdlib>
#include "pose3d.h"

int main(int argc, char** argv)
{
      Cpose3d p1, p2, p3;
      int testId = atoi(argv[1]);
      dlib::matrix<double,4,4> hM;
      dlib::matrix<double,6,1> pqv;
      lookAtValues lav;
      dlib::matrix<double,3,1> aa;
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
                  //std::cout << hM << std::endl;
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
                  //transform point

                  p1.pt = 1.,1.,1.;
                  p1.rt.setEuler(45.,45.,45.,inDEGREES);
                  for (ii=1; ii<=500; ii++)
                  {
                        aa = 0.5,-1,-0.5;
                        p1.moveForward(0.05*cos(ii/100.));
                        p1.rt.rotateUaxis(2,-ii/100.,ii/50.,-ii/200., inDEGREES);
                        p1.trf(aa, FROM_THIS_TO_REF);
                        p1.printPose(inDEGREES,false);//without endl
                        std::cout << " " << aa(0) << " " << aa(1) << " " << aa(2) << std::endl;
                  }
                  
                  break;

            case 5:
                  std::cout << std::endl << "******************** TEST 5 ****************************" << std::endl;
                  //transform pose 1

                  p1.pt = 1.,1.,1.;
                  p1.rt.setEuler(45.,45.,45.,inDEGREES);
                  for (ii=1; ii<=10; ii++)
                  {
                        p2.pt = 0.5,-1.,1.5;
                        p2.rt.setEuler(45*cos(ii/10.),30*cos(ii/20.),60*cos(ii/30.),inDEGREES);
                        p1.moveForward(0.05*cos(ii/100.));
                        p1.rt.rotateUaxis(3,-ii/100.,ii/50.,-ii/200., inDEGREES);
                        p1.trf(p2, FROM_THIS_TO_REF);
                        p1.printPose(inDEGREES,false);//without endl
                        std::cout << " ";
                        p2.printPose(inDEGREES,true);//with endl                        
                  }
                  
            case 6:
                  std::cout << std::endl << "******************** TEST 6 ****************************" << std::endl;
                  //transform pose 2

                  p1.pt = 1.,1.,1.;
                  p1.rt.setEuler(45.,45.,45.,inDEGREES);
                  for (ii=1; ii<=10; ii++)
                  {
                        p2.pt = 0.5,-1.,1.5;
                        p2.rt.setEuler(45*cos(ii/10.),30*cos(ii/20.),60*cos(ii/30.),inDEGREES);
                        p1.moveForward(0.05*cos(ii/100.));
                        p1.rt.rotateUaxis(3,-ii/100.,ii/50.,-ii/200., inDEGREES);
                        
                        p1.trf(p2, p3,FROM_THIS_TO_REF);
                        p2.printPose(inDEGREES);                 
                        p1.trf(p3,FROM_REF_TO_THIS);
                        p3.printPose(inDEGREES);                                         
                  }
                                    
                  break;
                  
                  
            default:
                  break;
      
      }
      
      return 0;
}
