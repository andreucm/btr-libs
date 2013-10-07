
#include <sstream>
#include "bflyCamera.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

//constants
const int numUserEntries = 4;

// print command line usage
void printUsage()
{
      std::cout
            << "Execute as: bflyAcquisition [numImages videoMode pixelFormat]" << std::endl 
            << "   numImages is a positive integer" << std::endl
            << "   videoMode is one of the following video modes: 0,1,4,5" << std::endl
            << "   pixelFormat is 0->MONO8 or 1->RGB8" << std::endl;
}

//main loop 
int main(int argc, char *argv[])
{            
      //variable declaration
      unsigned int ii;
      int retV = 0; 
      unsigned int numImages;
      bfly_videoMode vMode;
      bfly_pixelFormat pxFormat;
      
      //prompt header
      std::cout << std::endl << "**************** BFLY ACQUISITION PROCESS ******************" << std::endl;
      
      //check user entries
      if ( argc != numUserEntries )
      {
            std::cout  << "EXIT due to missing or too many parameters" << std::endl;
            printUsage();
            return BFLY_ERROR;
      }
      
      //get user entries
      numImages = atoi(argv[1]);
      vMode = (bfly_videoMode)atoi(argv[2]);
      if ( (vMode < 0) || (vMode == 2) || (vMode == 3) || (vMode > 5) )
      {
            std::cout << "EXIT due to unavailable Video Mode." << std::endl;                  
            printUsage();
            return BFLY_ERROR;
      }
      pxFormat = (bfly_pixelFormat)atoi(argv[3]);
      if ( (pxFormat < 0) || (pxFormat > 1) )
      {
            std::cout << "EXIT due to unavailable Pixel Format." << std::endl;                  
            printUsage();
            return BFLY_ERROR;
      }
      
      //print user entries
      std::cout 
            << "User entries are:" << std::endl
            << "  Num Images to acquire: " << numImages << std::endl
            << "  Video Mode: " << vMode << std::endl
            << "  Pixel Format: " << pxFormat << std::endl;

      //create camera and image objects
      CbflyCamera camera;
      cv::Mat img;
                 
      //check if camera object has been built correctly. It mainly checks if there is a camera available
      if( camera.isInitOk() )
      {                  
            //opens & configures camera
            retV += camera.open();
            retV += camera.configure(vMode, pxFormat);
            camera.printCameraInfo();
            if (retV != 2*BFLY_SUCCESS) return BFLY_ERROR;
            
            //set OpenCV Window
            cv::namedWindow("rawImage", CV_WINDOW_AUTOSIZE);
            cv::moveWindow("rawImage",50,50);
                        
            //starts acquisition
            if ( camera.startAcquisition() == BFLY_ERROR ) return BFLY_ERROR;
            
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
      
      return BFLY_SUCCESS;
}
