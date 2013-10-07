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
enum bfly_videoMode {MODE0 = 0, MODE1, MODE2, MODE3, MODE4, MODE5};
enum bfly_pixelFormat {MONO8 = 0, RGB8};


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
            FlyCapture2::GigEImageSettings imgSettings;
            cv::Mat cvImage;

      public:
            CbflyCamera();
            ~CbflyCamera();
            bool isInitOk() const;
            int open();
            int close();
            int configure(const bfly_videoMode vMode, const bfly_pixelFormat pxFormat);
            int configure(const unsigned int streamCh);
            int startAcquisition();
            int stopAcquisition();
            double getFrameRate();
            int getCurrentImage(cv::Mat & img);
            void printCameraInfo();
            void printImageInfo() const;
};
#endif
