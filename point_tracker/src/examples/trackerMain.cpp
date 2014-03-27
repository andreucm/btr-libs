#include "pointTracker.h"
#include <sstream>
#include <sys/time.h>

enum inputMode {FRAMES=1, VIDEO_FILE, VIDEO_DEVICE};
const bool VERBOSE_MODE = false;
const unsigned int FILE_NAME_LENGTH = 6;
const unsigned int IMAGE_COUNTER_INIT = 100;
const bool SAVE_IMAGES = true;

int main(int argc, char *argv[])
{
	//variable declaration
	bool runOk = true;
	std::string fileName;
	std::string pathName;
	unsigned int imageCounter = IMAGE_COUNTER_INIT;
	std::ostringstream imageName;
	cv::VideoCapture video;
	cv::Mat currentFrameRaw; 
      timeval ts;
      double timeStamp;
      inputMode inMode;
      unsigned int fType;
      pointTrackerParams fTrackerParams;
      
      //creates a point tracker object
      CpointTracker fTracker;      
	
	//check input params
	if (argc!=4)
	{
		std::cout << std::endl << 
		"Calling sequence is: $ trackerMain [inputMode] [inputName] [featureType]" << std::endl <<
		"     inputMode: 'f' , 'v' or 'd' " << std::endl <<
		"     inputName: folder|fileName|deviceId, respectively if inputMode is f,v or d" << std::endl <<
		"     featureType: 1->SURF, 2->ORB, 3->BRISK" << std::endl << std::endl <<
		"EXIT PROGRAM" << std::endl;
		return -1;
	}

	//get and set user's choice for input mode
	switch(*argv[1])
      {
            case 'f':
                  inMode = FRAMES;
                  pathName = std::string(argv[2]);
                  break;
            case 'v':
                  inMode = VIDEO_FILE;
                  fileName = std::string(argv[2]);
                  if (!video.open(fileName)) //Open video file
                  {
                        std::cout << "ERROR: video file name not found! " << std::endl;
                        return -1;
                  }
                  break;
            case 'd':
                  inMode = VIDEO_DEVICE;
                  if (!video.open(atoi(argv[2])))
                  {
                        std::cout << "ERROR: video device not found! " << std::endl;
                        return -1;
                  }
                  break;
            default:
                  std::cout << "\nUnknown Input format. Please set 'f' , 'v' or 'd'" << std::endl;
                  return -1;
                  break;
	}

	//get and set user's choice for feature type
	fType = (unsigned int)atoi(argv[3]);
      if ( (fType < 1) || (fType > 3) )
      {
            std::cout << "Unknown Feature Type: Please set 1->SURF, 2->ORB, 3->BRISK" << std::endl; 
            return -1;
      }
      else
      {
            fTrackerParams.featureType = (featureTypeEnum)fType;
      }
      
      //sets other params to default
      fTrackerParams.verbose = VERBOSE_MODE;
      fTrackerParams.min_hessian = MIN_HESSIAN_DEFAULT;
      fTrackerParams.min_features = MIN_FEATURES_DEFAULT;
      fTrackerParams.max_correspondence_dist = MAX_CORRESPONDENCE_DIST_DEFAULT;
      fTrackerParams.old_track_latency = OLD_TRACK_LATENCY_DEFAULT;
      fTrackerParams.viewMode = VIEW_TRACKS;
           
      //sets default configs & print them 
      fTracker.setParameters(fTrackerParams);
      fTracker.printConfig();

	//main loop 
	while(runOk)
	{
		//get a frame
		switch (inMode)
            {
                  case FRAMES:
                        //build image name and get it
                        imageName.str("");
                        imageName.clear();
                        imageName << pathName;
                        if ( imageName.str().at(imageName.str().length()-1) != '/') imageName << "/";
                        imageName.fill('0');
                        imageName.width(FILE_NAME_LENGTH);
                        imageName << imageCounter;
                        imageName.clear();
                        imageName << ".jpg";
                        currentFrameRaw = cv::imread(imageName.str());
                        if (fTrackerParams.verbose) std::cout << "Processing image " << imageName.str() << std::endl;
                        imageCounter = imageCounter + 1;
                        break;
                  case VIDEO_FILE:
                  case VIDEO_DEVICE:
                        video >> currentFrameRaw; 
                        if(!currentFrameRaw.data) runOk = false;
                        break;
                  default:
                        break;
            }
            
		//check for an empty image
		if ( currentFrameRaw.empty() )
		{
			std::cout << "ERROR: Image " << imageName.str() << " not found. Empty image." << std::endl;
			return -1;
		}
		else //frame exists
		{	
                  //sets time stamp to now
                  gettimeofday(&ts, NULL); 
                  timeStamp = (double)(ts.tv_sec + ts.tv_usec/1e6);       
                  fTracker.setTimeStamp(timeStamp);
                  
                  //displays verbose info
                  if (fTrackerParams.verbose) std::cout << "Image size: " << currentFrameRaw.rows << "," << currentFrameRaw.cols << std::endl;
                  if (fTrackerParams.verbose) std::cout << "currentFrameRaw.flags: " << std::hex << currentFrameRaw.flags << std::endl;
			
			//find point features 
			fTracker.findFeatures(currentFrameRaw,false);
			
			//find point pair correspondences
			fTracker.findCorrespondences();
			
			//update individual feature tracks
			fTracker.updateTracks();
			
                  //build output image, if ok, display it, and save it according SAVE_IMAGES
                  if ( fTracker.buildOutputImage() ) 
                  {
                        fTracker.displayOutputImage();
                        if ( SAVE_IMAGES ) fTracker.saveOutputImage("/home/andreu/dataSets/fakeTests/");
                  }
			
			//switch frame and keypoint buffers
			fTracker.switchBuffers();
		}
	}
	
	return 0;
}
