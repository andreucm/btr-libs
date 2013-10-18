#ifndef pointTracker_H
#define pointTracker_H

//std
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

//opencv
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"

//this lib
#include "trackStamped.h"

const int MIN_HESSIAN_DEFAULT = 250;//500 //SURF features 
const unsigned int MIN_FEATURES_DEFAULT = 30; //ORB features
const double MAX_CORRESPONDENCE_DIST_DEFAULT = 10; //[pixels]
const double OLD_TRACK_LATENCY = 0.5; //keep old tracks during a short time period before remove them
enum typeOfFeatures {SURF_FEATURES=1, ORB_FEATURES, BRISK_FEATURES};
enum viewMode {VIEW_POINTS=1, VIEW_CORRESPONDENCES, VIEW_TRACKS, VIEW_TRACKS_VELOCITY, VIEW_CORRESPONDENCE_MASK};

class CpointTracker
{
	protected: 
		/** 
		 * 
		 * Verbose execution mode
		 * 
		 **/
		bool verbose;
		
		/**
		 * 
		 * Index indicating the current buffer for image and point sets
		 * 
		 **/
		unsigned int fs_index;

		/**
		 * 
		 * Number of current iteration
		 * 
		 **/		
		unsigned int iterationId;

		/**
		 * 
		 * Id to be set to the next point track
		 * 
		 **/			
		unsigned int nextTrackId;

		/**
		 * 
		 * Image to be displayed for visualization purposes
		 * 
		 **/		
		cv::Mat displayImage;
		
		/**
		 * 
		 * Previous and current image frame. 
		 * Current is indicated by frame[fs_index] and previous by frame[!fs_index]
		 * 
		 **/				
		cv::Mat frame[2]; 
		
		/**
		 * 
		 * matching mask allowing only point corresponcences closer enough between consecutive frames
		 * 
		 **/
		cv::Mat slowMotionMask;
		
		/**
		 * 
		 * Time stamp of the current image frame
		 * 
		 **/
            double frameTS;
		
		/**
		 * 
		 * Indicates whether features are SURF_FEATURES or ORB_FEATURES
		 * 
		 **/
		unsigned int featureType;
		
            /**
            * 
            * Pointer to a Feature detector. To be allocated at the constructor depending on featureType
            * 
            **/
            cv::Ptr<cv::SURF> surfDetector;
            cv::Ptr<cv::ORB> orbDetector;
            cv::Ptr<cv::BRISK> briskDetector;
                
		/**
		 * 
		 * Set of visual features keypoints (image coordinates)
		 * Double buffer corresponding to current, featureSet[fs_index], and previous featureSet[!fs_index]
		 * 
		 **/
		cv::vector<cv::KeyPoint> featureSet[2]; 

            /**
            * 
            * Pointer to a Feature descriptor. To be allocated at the constructor depending on featureType
            * 
            **/
            cv::Ptr<cv::DescriptorExtractor> fDescriptor;
		
		/**
		 * 
		 * Set of feature point descriptors
		 * Double buffer corresponding to current, descriptors[fs_index], and previous descriptors[!fs_index]
		 * 
		 **/
		cv::Mat descriptors[2]; 
		
		/**
		 * 
		 * Matcher object to find corresponcences between feature descriptors
		 * 
		 **/
		cv::Ptr<cv::BFMatcher> matcher; //(2,true);
		
		/**
		 * 
		 * Set of point correspondence pairs. 
		 * Output result of the feature matcher
		 * 
		 **/
		cv::vector<cv::DMatch> matches;
		
		/**
		 * 
		 * Point coordinates corresponding to matched features
		 * pointSet[fs_index] is related to current image frame and pointSet[!fs_index] relates to the previous image frame
		 * 
		 **/
		cv::vector<cv::Point2f> pointSet[2]; 
		
		/**
		 * 
		 * Track of all point features
		 * 
		 **/
		std::list<CtrackStamped> trackList;

	public:
		/**
		 * 
		 * Constructor 
		 * 
		 **/
            CpointTracker(unsigned int fType, bool vbose = false);

		/**
		 * 
		 * Destructor 
		 * 
		 **/
		~CpointTracker();
		
		/**
		 * 
		 * Switch current buffer index
		 * 
		 **/
		void switchBuffers();

		/**
		 * 
		 * Sets time stamp with the provided value
		 * 
		 **/
		void setTimeStamp(const double ts);

            /** \brief returns time stamp value
            * 
            * Gets time stamp value
            * 
            **/
            double getTimeStamp() const;

            /** \brief Returns a reference to the track list
            * 
            * Returns a reference to the track list
            * 
            **/
            std::list<CtrackStamped> & getTrackList();                
            
            /** \brief Returns a reference to the marked image
            * 
            * Returns a reference to the marked image
            * 
            **/
            cv::Mat & getDisplayImage();

		/**
		 * 
		 * Find features in vFrame image. 
		 * Sets point detections to featureSet[fs_index] and feature descriptors at descriptors[fs_index]
		 * 
		 **/
		void findFeatures(const cv::Mat & vFrame, bool equalize=true);
		
		/**
		 * 
		 * Finds matches between current and previous frame. Sets pointSet
		 * 
		 **/
		void findCorrespondences();
		
		/**
		 * 
		 * Updates tracks by creating new tracks when no previous match was found
		 * and removing them if a track has no detections during an OLD_TRACK_LATENCY period
		 * 
		 **/
		void updateTracks();
		
		/**
		 * 
		 * Visualize displayImage, which is a composition of frame[fs_index] with some overpainted marks 
		 * 
		 **/
		void displayOutput(unsigned int vMode, bool save=false);
                
};
#endif

