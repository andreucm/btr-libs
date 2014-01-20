#include <string>
#include <iostream>
#include "ballDetector.h"

int main(int argc, char *argv[])
{
	cv::VideoCapture video;
	cv::Mat rawImage; 
	CballDetector ballDetector;
	unsigned int deviceId;
	bool running = true;
	
	//splash
	std::cout << std::endl << "************************** BALL DETECTOR **************************" << std::endl;
	
	//check user arguments
	if(argc != 2)
	{
		std::cout << "Usage: ../bin/ballDetector_test <X (device ID from /dev/videoX)>" << std::endl;
		return -1;
	}
	
	//open video device
	deviceId = atoi(argv[1]);
	video.open(deviceId);
	if(video.isOpened())  // check if we succeeded
      {
		std::cout << "Getting video from video device " << deviceId << std::endl;
	}
	else
	{
		std::cout << "Unable to get video from video device " << deviceId << ". EXIT" << std::endl;
		return -1;
	}

      //config detector
      ballDetector.setDefaultParameters();
      ballDetector.printConfig();
      
	//main loop
	while (running)
	{
		video >> rawImage; 
		if(!rawImage.data) running = false;
		else
		{
                  //sets current frame
			ballDetector.setInputImage(rawImage);
                        
                  //detects circles
			ballDetector.houghDetection();
                        
                  //displays output
			ballDetector.displayOutput();                        
		}
	}
		
	return 0;
}