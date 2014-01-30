
#ifndef ballDetector_H
#define ballDetector_H

//includes
#include <iostream>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//constants
const int GAUSSIAN_BLUR_SIZE_DEFAULT = 7;
const double GAUSSIAN_BLUR_SIGMA_DEFAULT = 2; 
const double CANNY_EDGE_TH_DEFAULT = 130;
const double HOUGH_ACCUM_RESOLUTION_DEFAULT = 2;
const double MIN_CIRCLE_DIST_DEFAULT = 30;
const double HOUGH_ACCUM_TH_DEFAULT = 120;
const int MIN_RADIUS_DEFAULT = 30;
const int MAX_RADIUS_DEFAULT = 400;
const unsigned int IMG_MONO = 0;
const unsigned int IMG_RGB8 = 1;

//struct
/** \brief Configuration parameters for circle detector
 * 
 * Configuration parameters for circle detector
 * 
 **/
struct detectorConfig
{
      int gaussian_blur_size;//size of gaussian blur kernel mask [pixel]
      double gaussian_blur_sigma; //sigma of gaussian blur kernel mask [pixel]
      double canny_edge_th; //threshold of the edge detector. 
      double hough_accum_resolution;  //resolution of the Hough accumulator, in terms of inverse ratio of image resolution
      double min_circle_dist; //Minimum distance between circles 
      double hough_accum_th; //accumulator threshold to decide circle detection
      int min_radius; //minimum circle radius allowed
      int max_radius; //maximum circle radius allowed
};

//class
/** \brief Provides image coordinates of the detected circles
 * 
 * This class provides image coordinates of the set of the detected circles in the current image
 * 
 **/
class CballDetector
{
	protected: 
		/** \brief Set of detected circles
		 * 
		 * Detected circles. For a circle i:
		 *    x_i: circles[i][0]
             *    y_i: circles[i][1] 
             *    radius_i: circles[i][2]
		 * 
		 **/
		cv::vector<cv::Vec3f> circles;
		
		/** \brief Input image
		 * 
		 * Input image which is processed by this detector
		 * 
		 **/
		cv::Mat inImage;

		/** \brief Output marked image
		 * 
		 * Output image. To be displayed if required
		 * 
		 **/		
		cv::Mat outImage;
            
            /** \brief Config parameters
             * 
             * Config parameters
             * 
             **/
            detectorConfig params;
            
		
	public:
		/**
		 * 
		 * default constructor
		 * 
		 **/
		CballDetector();
		
		/**
		 * 
		 * destructor
		 * 
		 **/
		~CballDetector();
            
            /** \brief Sets configuration
             * 
             * Sets configuration parameters for the detector
             * 
             **/
            void setParameters(detectorConfig & cfgP);

            /** \brief Sets default configuration
             * 
             * Sets default configuration parameters for the detector
             * 
             **/
            void setDefaultParameters();
            
		/**
		 * 
		 * Sets input image (this->inImage)
		 * 
		 **/
		void setInputImage(const cv::Mat & inIm);

		/**
		 * 
		 * Gets output image (this->outImage)
		 * 
		 **/		
		void getOutputImage(cv::Mat & outIm);
		
		/**
		 * 
		 * Detects circles in this->inImage without prior information, using circle Hough detector.
		 * Sets this->ballPos 
		 * Sets this->bBox
             * imgEncoding indicates whether the image is IMG_MONO or IMG_RGB8.
		 * 
		 **/
		void houghDetection(const unsigned int imgEncoding = IMG_RGB8);
		
            /**
             * 
             * Draws circles to the output image
             * 
             **/
            void drawOutputImage();
            
            /** \brief Returns the number of circles detected
             * 
             * Returns the number of circles detected
             * 
             **/
            unsigned int howManyCircles() const;
            
            /** \brief Gets i-th circle 
             * 
             * Gets ii-th circle , xx,yy pixel coordinates and rr radius
             * Returns -1 if ii index is out of the circles vector, , otherwise it returns 0 and the values are set.
             * 
             **/
            int getCircle(const unsigned int ii, double & xx, double & yy, double & rr) const;
            
		/**
		 * 
		 * Displays output image
		 * 
		 **/
		void displayOutput();
            
            /**
             * 
             * Prints current configuration parameter values to std out
             * 
             **/
            void printConfig() const; 
};
#endif
