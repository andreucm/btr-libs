
#include <sstream>
#include "bflyCamera.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

//constants
const int numUserEntries = 2;
//const unsigned int numImagesDefault = 1000;

//this will be user entries
const unsigned int imageW = 320;
const unsigned int imageH = 182;
const unsigned int pxFormat = RGB8;

//main loop 
int main(int argc, char *argv[])
{      
      //check user entries
      if ( argc != numUserEntries )
      {
            std::cout  << "EXIT due to missing or too many parameters" << std::endl
                  << "Execute with \"bflyAcquisition [numImages]. \" " << std::endl;
            return -1;
      }
      
      CbflyCamera camera;
      cv::Mat img;
      unsigned int ii, numImages;
      int retV = 0; 
            
      //set Window
      cv::namedWindow("rawImage", CV_WINDOW_AUTOSIZE);
      cv::moveWindow("rawImage",20,50);
      
      if( camera.isInitOk() )
      {      
            //get user entries
            numImages = atoi(argv[1]);
            std::cout << "Num Images to acquire: " << numImages << std::endl;
       
            //opens & configures camera
            retV += camera.open();
            retV += camera.configure(imageW, imageH, pxFormat);
            camera.printCameraInfo();
            retV += camera.startAcquisition();
            if (retV != 3*BFLY_SUCCESS) return -1;
            
            //acquisition loop            
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
