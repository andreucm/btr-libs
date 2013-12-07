
#include <cstdlib>
#include "wState.h"

int main(int argc, char** argv)
{
      CtimeStamp ti,te;
      CwState ww;
      CvqStateTs vqs;
      Cpose3d po;
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
                  
            default:
                  break;
      
      }
      
      return 0;
}
