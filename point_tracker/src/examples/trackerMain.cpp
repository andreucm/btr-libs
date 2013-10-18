#include "pointTracker.h"
#include <sstream>
#include <sys/time.h>

enum inputMode {FRAMES=1, VIDEO_FILE, VIDEO_DEVICE};
const bool verbose = false;
const unsigned int FILE_NAME_LENGTH = 6;

int main(int argc, char *argv[])
{
	//variable declaration
	bool runOk = true;
	std::string fileName;
	std::string pathName;
	unsigned int imageCounter = 100;
	std::ostringstream imageName;
	cv::VideoCapture video;
	cv::Mat currentFrameRaw; 
      timeval ts;
      double timeStamp;
      inputMode inMode;
	
	//check input params
	if (argc!=4)
	{
		std::cout << "\nCalling sequence is: $ trackerMain [inputMode] [inputName] [featureType]" << std::endl <<
		std::cout << "     inputMode: 'f' , 'v' or 'd' " << std::endl <<
		std::cout << "     inputName: folder|fileName|deviceId, respectively if inputMode is f,v or d" << std::endl <<
		std::cout << "     featureType: 1->SURF, 2->ORB, 3->BRISK" << std::endl;
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
	unsigned int fType = (unsigned int)atoi(argv[3]);
      if ( (fType < 1) || (fType > 3) )
      {
            std::cout << "Unknown Feature Type: Please set 1->SURF, 2->ORB, 3->BRISK" << std::endl; 
            return -1;
      }
      
      //creates a point tracker object
      CpointTracker fTracker(fType);

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
                        if (verbose) std::cout << "Processing image " << imageName.str() << std::endl;
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
                  if (verbose) std::cout << "Image size: " << currentFrameRaw.rows << "," << currentFrameRaw.cols << std::endl;
                  if (verbose) std::cout << "currentFrameRaw.flags: " << std::hex << currentFrameRaw.flags << std::endl;
			
			//find point features 
			fTracker.findFeatures(currentFrameRaw,false);
			
			//find point pair correspondences
			fTracker.findCorrespondences();
			
			//update individual feature tracks
			fTracker.updateTracks();
			
			//display
			fTracker.displayOutput(VIEW_TRACKS);
			
			//switch frame and keypoint buffers
			fTracker.switchBuffers();
		}
	}
	
	return 0;
}
