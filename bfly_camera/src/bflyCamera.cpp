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

bool CbflyCamera::isInitOk() const
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

/*
int CbflyCamera::configure(const bfly_videoMode vMode, const bfly_pixelFormat pxFormat)
{
      FlyCapture2::Format7ImageSettings imgSettings;
      FlyCapture2::Format7Info fmt7info;
      FlyCapture2::Format7PacketInfo fmt7infoPacket;
      bool supported, valid;
      
      //Set video mode
      switch (vMode)
      {
            case MODE0: imgSettings.mode = FlyCapture2::MODE_0; break;
            case MODE1: imgSettings.mode = FlyCapture2::MODE_1; break;
            case MODE4: imgSettings.mode = FlyCapture2::MODE_4; break;
            case MODE5: imgSettings.mode = FlyCapture2::MODE_5; break;
            default: 
                  std::cout << "CbflyCamera::configure(): Unknown video mode" << std::endl; 
                  return BFLY_ERROR;
                  break;
      }
      
      //set pixel format
      switch (pxFormat)
      {
            case MONO8: imgSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_MONO8; break;
            case RGB8: imgSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_RGB8; break;
            default:
                  std::cout << "CbflyCamera::configure(): Unknown pixel format: " << pxFormat << std::endl;
                  return BFLY_ERROR;
                  break;
      }
      
      //get max image size, given the video mode
      fmt7info.mode = imgSettings.mode;
      pgError = cam.GetFormat7Info( &fmt7info, &supported );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
            
      //set image size as the maximum allowed, and resets ROI offsets
      imgSettings.offsetX = 0;
      imgSettings.offsetY = 0;
      imgSettings.width = fmt7info.maxWidth;
      imgSettings.height = fmt7info.maxHeight;
      
      //check validity of configuration
      pgError = cam.ValidateFormat7Settings( &imgSettings, &valid, &fmt7infoPacket );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      if ( !valid )
      {
            std::cout << "CbflyCamera::configure(): Format7 settings are not valid" << std::endl;
            return BFLY_ERROR;
      }

      //finally set configuration
      pgError = cam.SetFormat7Configuration( &imgSettings, fmt7infoPacket.recommendedBytesPerPacket );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      
      //return SUCCESS
      return BFLY_SUCCESS;
}
*/

int CbflyCamera::configure(const bfly_videoMode vMode, const bfly_pixelFormat pxFormat)
{
      FlyCapture2::GigEImageSettingsInfo imgSettingsInfo;
      FlyCapture2::Mode vModeFC2;
      
std::cout << "configure(): " << __LINE__ << std::endl;
      
      //Set video mode
      switch (vMode)
      {
            case MODE0: vModeFC2 = FlyCapture2::MODE_0; break;
            case MODE1: vModeFC2 = FlyCapture2::MODE_1; break;
            case MODE4: vModeFC2 = FlyCapture2::MODE_4; break;
            case MODE5: vModeFC2 = FlyCapture2::MODE_5; break;
            default: 
                  std::cout << "CbflyCamera::configure(): Unknown video mode" << std::endl; 
                  return BFLY_ERROR;
                  break;
      }
      pgError = cam.SetGigEImagingMode( vModeFC2 );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }

      //set pixel format and image size to maximum allowed
      pgError = cam.GetGigEImageSettingsInfo( &imgSettingsInfo );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
      imgSettings.offsetX = 0;
      imgSettings.offsetY = 0;
      imgSettings.height = imgSettingsInfo.maxHeight;
      imgSettings.width = imgSettingsInfo.maxWidth;
      switch (pxFormat)
      {
            case MONO8: imgSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_MONO8; break;
            case RGB8: imgSettings.pixelFormat = FlyCapture2::PIXEL_FORMAT_RGB8; break;
            default:
                  std::cout << "CbflyCamera::configure(): Unknown pixel format: " << pxFormat << std::endl;
                  return BFLY_ERROR;
                  break;
      }
      pgError = cam.SetGigEImageSettings( &imgSettings );
      if (pgError != FlyCapture2::PGRERROR_OK)
      {
            pgError.PrintErrorTrace();
            return BFLY_ERROR;
      }
            
      //return SUCCESS
      return BFLY_SUCCESS;
}

int CbflyCamera::configure(const unsigned int streamCh)
{
      
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
      if ( imgSettings.pixelFormat == FlyCapture2::PIXEL_FORMAT_MONO8 ) 
            cvImage.create(rawImage.GetRows(),rawImage.GetCols(),CV_8UC1);
      if ( imgSettings.pixelFormat == FlyCapture2::PIXEL_FORMAT_RGB8 ) 
            cvImage.create(rawImage.GetRows(),rawImage.GetCols(),CV_8UC3);
      cvImage.data = rawImage(0,0);//set cv data pointer to rawImage address
      
      //clone to external argument image
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

void CbflyCamera::printImageInfo() const
{
      std::cout   << "  Image Rows: " << rawImage.GetRows() << std::endl
            << "  Image Cols: " << rawImage.GetCols() << std::endl
            << "  Image Stride: " << rawImage.GetStride() << std::endl
            << "  Image BitsPerPixel: " << rawImage.GetBitsPerPixel() << std::endl
            << "  Image DataSize (Bytes): " << rawImage.GetDataSize() << std::endl
            << std::endl;
}