
#include "bflyCamera.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

//this will be user entries
const unsigned int numImages = 1000;
const unsigned int imageW = 320;
const unsigned int imageH = 182;
const unsigned int pxFormat = RGB8;

//main loop 
int main(int argc, char *argv[])
{
      CbflyCamera camera;
      cv::Mat img;
      unsigned int ii;
      
      //set Window
      cv::namedWindow("rawImage", CV_WINDOW_AUTOSIZE);
      cv::moveWindow("rawImage",20,50);
      
      //opens camera and starts acquisition loop
      if( camera.isInitOk() )
      {
            camera.open();
            camera.configure(imageW, imageH, pxFormat);
            camera.printCameraInfo();
            camera.startAcquisition();
            
            for (ii=0; ii<numImages; ii++)
            {
                  //prints useful information, each 10 frames
                  if (ii%10 == 0) 
                  {
                        std::cout << "Frame rate: " << camera.getFrameRate() << std::endl;
                        camera.printImageInfo();
                  }
                  
                  //Gets image data in openCV format
                  camera.getCurrentImage(img);
                  
                  // show current image
                  cv::imshow("rawImage", img);
                  
                  // wait for a key during 1ms
                  cv::waitKey(1); 
            }
            
            camera.stopAcquisition();
            camera.close();
      }
      
      return 0;
}
