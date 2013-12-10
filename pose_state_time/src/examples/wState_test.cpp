
#include <cstdlib>
#include "wState.h"

int main(int argc, char** argv)
{
      CtimeStamp ti,te;
      CwState ww;
      CvqStateTs vqs;
      Crotation rot;
      Cquaternion quat;
      Cpose3d po, po1, po2;
      int testId = atoi(argv[1]);
      unsigned int ii;
      
      
      //test switch
      switch(testId)
      {
            case 1:
                  std::cout << std::endl << "******************** TEST 1 ****************************" << std::endl;
                  
                  //fill the window state
                  for (ii= 0; ii<WINDOW_STATE_SIZE; ii++)
                  {
                        vqs.ts = (double)(ii);
                        vqs.vv = ii/10.,ii/20.,ii/30.;
                        vqs.qt.set(ii/50.,ii/40.,ii/30.);
                        ww.ws.push_back(vqs);
                  }
                  
                  //compute transformation between two time stamps.
                  ti = 0.5;
                  te = WINDOW_STATE_SIZE - 1.3;
                  if ( ww.tsInWsPeriod(ti,te) )
                  {
                        ww.getTransform(ti,te,po);
                  }
                  
                  //print results
                  ww.print();
                  po.printPose(inDEGREES,true);
                  
                  break;
                  
            case 2:
                  std::cout << std::endl << "******************** TEST 2 ****************************" << std::endl;
                  
                  //fill the window state
                  for (ii= 0; ii<WINDOW_STATE_SIZE; ii++)
                  {
                        vqs.ts = (double)(ii);
                        vqs.vv = ii/10.,ii/20.,ii/30.;
                        rot.setEuler(ii*1.,ii*2.,ii*5.,inDEGREES);
                        rot.getQuaternion(quat);
                        vqs.qt = quat;
                        ww.ws.push_back(vqs);
                  }
                  ww.print();
                  
                  //compute transformation between two time stamps.
                  ti = 8;
                  te = (double)WINDOW_STATE_SIZE-1.;
                  if ( ww.tsInWsPeriod(ti,te) )
                  {
                        ww.getTransform(ti,te,po);
                  }
                  
                  //print results
                  po.printPose(inDEGREES,true);
                  
                  //check
                  quat.qq =  0.931601 , 0.328744 , 0.154087 , 0.0174277;
                  po1.rt.setQuaternion(quat);
                  quat.qq =  0.914389 , 0.365467 , 0.173736 , 0.011914;
                  po2.rt.setQuaternion(quat);
                  po1.trf(po2,FROM_REF_TO_THIS);
                  po2.printPose(inDEGREES,true);
                  
                  break; 
                  
            default:
                  break;
      
      }
      
      return 0;
}
