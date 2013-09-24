#include "bflyCamera.h"

CbflyCamera::CbflyCamera()
{
      FlyCapture2::BusManager busMgr;
      FlyCapture2::InterfaceType interfaceType;
      unsigned int numCameras;
      
      //reset inti flag
      initOk = false;
      
      //get number of cameras available
      pgError = busMgr.GetNumOfCameras(&numCameras);
      if (pgError != FlyCapture2::PGRERROR_OK) 
      {
            pgError.PrintErrorTrace();
            return;
      }
      std::cout << std::endl << "Number of cameras detected: " << numCameras << std::endl;
      if ( numCameras < 1 )
      {
            std::cout << "Insufficient number of cameras... exiting" << std::endl;
            return;
      }
    
      //Gets id of the first camera found (camera 0)
      pgError = busMgr.GetCameraFromIndex(0, &guid);
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return;
      }
      
      //check if it is a GigE camera
      pgError = busMgr.GetInterfaceTypeFromGuid( &guid, &interfaceType );
      if ( pgError != FlyCapture2::PGRERROR_OK )
      {
            pgError.PrintErrorTrace();
            return;
      }
      if ( interfaceType != FlyCapture2::INTERFACE_GIGE )
      {
            std::cout << "Error: GigE camera not found" << std::endl;
      }

      //set init flag
      initOk = true;
      std::cout << "GigE camera found!" << std::endl;
}

CbflyCamera::~CbflyCamera()
{
      if ( cam.IsConnected() ) this->close();
}

bool CbflyCamera::isInitOk()
{
      return initOk;
}

int CbflyCamera::open()
{
      //connects to the camera
      pgError = cam.Connect(&guid);
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      return BFLY_SUCCESS;
}

int CbflyCamera::close()
{
      // Disconnects from the camera
      pgError = cam.Disconnect();
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      return BFLY_SUCCESS;    
}

int CbflyCamera::configure(unsigned int imW, unsigned int imH, unsigned int pxFormat)
{
      //load image settings to imageSettings struct
      imageSettings.offsetX = 0;
      imageSettings.offsetY = 0;
      imageSettings.height = imH;
      imageSettings.width = imW;
      switch(pxFormat)
      {
            case MONO8:
                  imageSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_MONO8;
                  break;
            case RGB8:
                  imageSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_RGB8;
                  break;
            default:
                  std::cout << "CbflyCamera::configure(): Unknown image format: " << pxFormat << std::endl;
                  return BFLY_ERROR;
                  break;
      }
      
      //set image settings
      std::cout << "Setting GigE image settings" << std::endl;
      pgError = cam.SetGigEImageSettings( &imageSettings );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }

      //return success
      return BFLY_SUCCESS;
}

int CbflyCamera::startAcquisition()
{
      pgError = cam.StartCapture(); // Start capturing images
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      std::cout << "Start Image Acquisition" << std::endl;
      return BFLY_SUCCESS;
}

int CbflyCamera::stopAcquisition()
{
      pgError = cam.StopCapture();// Stop capturing images
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      std::cout << "Stop Image Acquisition" << std::endl;
      return BFLY_SUCCESS;
}

double CbflyCamera::getFrameRate()
{
      FlyCapture2::Property frameRate;
      
      frameRate.type = FlyCapture2::FRAME_RATE;
      pgError = cam.GetProperty( &frameRate );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      return frameRate.absValue;
}

int CbflyCamera::getCurrentImage(cv::Mat & img)
{
      //Gets current camera buffer image
      pgError = cam.RetrieveBuffer( &rawImage );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      
      //do conversion from FlyCapture2::Image to cv::Mat      
      if ( imageSettings.pixelFormat == FlyCapture2::PIXEL_FORMAT_MONO8 ) 
            cvImage.create(rawImage.GetRows(),rawImage.GetCols(),CV_8UC1);
      if ( imageSettings.pixelFormat == FlyCapture2::PIXEL_FORMAT_RGB8 ) 
            cvImage.create(rawImage.GetRows(),rawImage.GetCols(),CV_8UC3);
      cvImage.data = rawImage(0,0);//set cv data pointer to rawImage address
      img = cvImage.clone();
      
      //return success
      return BFLY_SUCCESS;
}

void CbflyCamera::printCameraInfo()
{
      // Get the camera information
      FlyCapture2::CameraInfo info;
      pgError = cam.GetCameraInfo(&info);
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return;
      }       
      else
      {
            std::cout << std::endl
                  << "******** CAMERA INFORMATION ********" << std::endl
                  << "Camera vendor - " << info.vendorName << std::endl
                  << "Camera model - " << info.modelName << std::endl
                  << "Serial number - " << info.serialNumber << std::endl
                  << "Sensor - " << info.sensorInfo << std::endl
                  << "Resolution - " << info.sensorResolution << std::endl
                  << "Firmware version - " << info.firmwareVersion << std::endl
                  << "Firmware build time - " << info.firmwareBuildTime << std::endl
                  << "GigE version - " << info.gigEMajorVersion << "." << info.gigEMinorVersion << std::endl
                  << "IP Address - " << (unsigned int)info.ipAddress.octets[0] << "." << (unsigned int)info.ipAddress.octets[1] << "." << (unsigned int)info.ipAddress.octets[2] << "." << (unsigned int)info.ipAddress.octets[3] << std::endl
                  << std::endl;
      }
}

void CbflyCamera::printImageInfo()
{
      std::cout   << "  Image Rows: " << rawImage.GetRows() << std::endl
            << "  Image Cols: " << rawImage.GetCols() << std::endl
            << "  Image Stride: " << rawImage.GetStride() << std::endl
            << "  Image BitsPerPixel: " << rawImage.GetBitsPerPixel() << std::endl
            << "  Image DataSize (Bytes): " << rawImage.GetDataSize() << std::endl
            << std::endl;
}