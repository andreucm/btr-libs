#include "ballDetector.h"

CballDetector::CballDetector()
{
	cv::namedWindow("ballDetector",CV_WINDOW_AUTOSIZE); //initializes output window
}

CballDetector::~CballDetector()
{
	
}

void CballDetector::setParameters(detectorConfig & cfgP)
{
      params.gaussian_blur_size = cfgP.gaussian_blur_size;
      params.gaussian_blur_sigma = cfgP.gaussian_blur_sigma; 
      params.canny_edge_th = cfgP.canny_edge_th;
      params.hough_accum_resolution = cfgP.hough_accum_resolution;
      params.min_circle_dist = cfgP.min_circle_dist;
      params.hough_accum_th = cfgP.hough_accum_th;
      params.min_radius = cfgP.min_radius;
      params.max_radius = cfgP.max_radius;
}

void CballDetector::setDefaultParameters()
{
      params.gaussian_blur_size = GAUSSIAN_BLUR_SIZE_DEFAULT;
      params.gaussian_blur_sigma = GAUSSIAN_BLUR_SIGMA_DEFAULT;
      params.canny_edge_th = CANNY_EDGE_TH_DEFAULT;
      params.hough_accum_resolution = HOUGH_ACCUM_RESOLUTION_DEFAULT;
      params.min_circle_dist = MIN_CIRCLE_DIST_DEFAULT;
      params.hough_accum_th = HOUGH_ACCUM_TH_DEFAULT;
      params.min_radius = MIN_RADIUS_DEFAULT;
      params.max_radius = MAX_RADIUS_DEFAULT;      
}

void CballDetector::setInputImage(const cv::Mat & inIm)
{
	inImage = inIm.clone();	
}

void CballDetector::getOutputImage(cv::Mat & outIm)
{
      this->drawOutputImage();
      outIm = outImage.clone();	            
}

void CballDetector::houghDetection(const unsigned int imgEncoding)
{
	cv::Mat grayImage;
	cv::vector<cv::Vec3f> circlesCurrent;
	
	//clear previous circles
 	circles.clear();
	
	// If input image is RGB, convert it to gray 
      if ( imgEncoding == IMG_RGB8 ) cv::cvtColor(inImage, grayImage, CV_BGR2GRAY);

	//Reduce the noise so we avoid false circle detection
	cv::GaussianBlur( grayImage, grayImage, cv::Size(params.gaussian_blur_size, params.gaussian_blur_size), params.gaussian_blur_sigma );

	//Apply the Hough Transform to find the circles
	cv::HoughCircles( grayImage, circlesCurrent, CV_HOUGH_GRADIENT, params.hough_accum_resolution, params.min_circle_dist, params.canny_edge_th, params.hough_accum_th, params.min_radius, params.max_radius );
	
      //set found circles to circles set. Apply some condition if desired.
	unsigned int ii;
	for (ii = 0; ii < circlesCurrent.size(); ii++ )	
      {
            circles.push_back(circlesCurrent.at(ii));
            //std::cout << "circle " << ii << ": (" << circles.at(ii)[0] << "," << circles.at(ii)[1] << ")" << std::endl;
      }
}

void CballDetector::drawOutputImage()
{
      cv::Point center;
      int radius;
      size_t ii;
      
      //draws results to output Image
      outImage = inImage.clone();
      for( ii = 0; ii < circles.size(); ii++ )
      {
            if ( circles[ii][0] != -1 )
            {
                  center = cv::Point(cvRound(circles[ii][0]), cvRound(circles[ii][1]));
                  radius = cvRound(circles[ii][2]);
                  cv::circle( outImage, center, 5, cv::Scalar(0,0,255), -1, 8, 0 );// circle center in green
                  cv::circle( outImage, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );// circle outline in red
            }
      }      
}

unsigned int CballDetector::howManyCircles() const
{
      return circles.size();
}

int CballDetector::getCircle(const unsigned int ii, double & xx, double & yy, double & rr) const
{
      if ( ii >= circles.size() ) 
            return -1;
      else
      {
            xx = circles[ii][0];
            yy = circles[ii][1];
            rr = circles[ii][2];
            return 1;
      }
}

void CballDetector::displayOutput()
{
	cv::Mat outImageFlip;
      
      this->drawOutputImage();
	cv::flip(outImage,outImageFlip, 1);
	cv::imshow("ballDetector", outImageFlip);
	cv::waitKey(1);
}

void CballDetector::printConfig() const
{
      std::cout << "Detetctor Configuration:" << std::endl 
            << "    gaussian_blur_size: " << params.gaussian_blur_size << std::endl
            << "    gaussian_blur_sigma: " << params.gaussian_blur_sigma << std::endl
            << "    canny_edge_th: " << params.canny_edge_th << std::endl
            << "    hough_accum_resolution: " << params.hough_accum_resolution << std::endl
            << "    min_circle_dist: " << params.min_circle_dist << std::endl
            << "    hough_accum_th: " << params.hough_accum_th << std::endl
            << "    min_radius: " << params.min_radius << std::endl
            << "    max_radius: " << params.max_radius << std::endl;
}
