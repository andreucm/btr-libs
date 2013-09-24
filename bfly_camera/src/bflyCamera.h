#ifndef bflyCamera_H
#define bflyCamera_H

//includes
#include <iostream>
#include "FlyCapture2.h"
#include "opencv2/core/core.hpp"

//constants
const int BFLY_ERROR = -1;
const int BFLY_SUCCESS = 1;

//enums
enum imageModes {MONO8 = 1, RGB8};


/** \brief Class for openCV image acquisition from BlackFly Point Grey camera
 * 
 * Class for openCV image acquisition from BlackFly Point Grey camera
 * 
 **/
class CbflyCamera
{
      protected:
            bool initOk;
            FlyCapture2::Error pgError;
            FlyCapture2::PGRGuid guid;            
            FlyCapture2::GigECamera cam;
            FlyCapture2::Image rawImage;  
            FlyCapture2::GigEImageSettings imageSettings;
            cv::Mat cvImage;

      public:
            CbflyCamera();
            ~CbflyCamera();
            bool isInitOk();
            int open();
            int close();
            int configure(unsigned int imW, unsigned int imH, unsigned int pxFormat);
            int startAcquisition();
            int stopAcquisition();
            double getFrameRate();
            int getCurrentImage(cv::Mat & img);
            void printCameraInfo();
            void printImageInfo();
};
#endif
