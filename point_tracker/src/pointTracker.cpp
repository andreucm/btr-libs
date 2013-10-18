#include "pointTracker.h"

// CpointTracker::CpointTracker(unsigned int fType, int wdth, int hght, bool vbose)
CpointTracker::CpointTracker(unsigned int fType, bool vbose)
{
	//initializes window
	cv::namedWindow("Point Tracker",CV_WINDOW_AUTOSIZE); //| CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);//, CV_WINDOW_AUTOSIZE);
	
	//sets verbose
	verbose = vbose; 
	
	//init fs_index, iteration counter
	fs_index = 0;
	iterationId = 0;
	nextTrackId = 1;
	
	//init feature detector
	featureType = fType;
	switch(featureType)
	{
		case SURF_FEATURES:
                        surfDetector = new cv::SURF(MIN_HESSIAN_DEFAULT);
                        fDescriptor = cv::DescriptorExtractor::create("SURF");
                        if (verbose) std::cout << "SURF Detector & Extractor allocated" << std::endl;
                        break;	
		case ORB_FEATURES:
                        orbDetector = new cv::ORB(MIN_FEATURES_DEFAULT);
                        fDescriptor = cv::DescriptorExtractor::create("ORB");
                        if (verbose) std::cout << "ORB Detector & Extractor allocated" << std::endl;
                        break;
            case BRISK_FEATURES:
                        briskDetector = new cv::BRISK();
                        fDescriptor = cv::DescriptorExtractor::create("BRISK");
                        if (verbose) std::cout << "BRISK Detector & Extractor allocated" << std::endl;
                        break;      
		default:
			std::cout << "WARNING: CpointTracker::CfeatureTracker(): Unknown feature type: " << featureType << ". No detector cerated" << std::endl;
			break;			
	}
	
	//init feature matcher
      matcher = cv::Ptr<cv::BFMatcher>(new cv::BFMatcher(cv::NORM_L2,false));
}

CpointTracker::~CpointTracker()
{
	trackList.clear();
	cv::destroyWindow("Point Tracker");

      switch(featureType)
      {
            case SURF_FEATURES:
                  surfDetector.release();
                  break;
            case ORB_FEATURES:
                  orbDetector.release();
                  break;
            case BRISK_FEATURES:
                  briskDetector.release();
                  break;                  
            default:
                  break;
      }                  
      fDescriptor.release();
      
      matcher.release();
}

void CpointTracker::switchBuffers()
{
	fs_index = !fs_index;
}

void CpointTracker::setTimeStamp(const double ts)
{
        frameTS = ts;
}

double CpointTracker::getTimeStamp() const
{
        return frameTS;
}

std::list<CtrackStamped> & CpointTracker::getTrackList()
{
        return this->trackList;
}

cv::Mat & CpointTracker::getDisplayImage()
{
        return displayImage;
}

void CpointTracker::findFeatures(const cv::Mat & vFrame, bool equalize)
{	
	cv::Mat vFrame1c[3];
	cv::Mat eq1c[3];

	if (equalize)
	{
// 		cv::split(vFrame,vFrame1c); //split RGB channels
// 		cv::equalizeHist(vFrame1c[0],eq1c[0]); //equalize c0
// 		cv::equalizeHist(vFrame1c[1],eq1c[1]); //equalize c1
// 		cv::equalizeHist(vFrame1c[2],eq1c[2]); //equalize c2
// 		cv::merge(eq1c,3,frame[fs_index]); //merge 3 channels to RGB image
	}
	else 
	{
		frame[fs_index] = vFrame;//.clone(); //copy the image "as is" to the frame buffer
	}

	//clear points of "before-last" iteration (t-2)
	featureSet[fs_index].clear();

	//1. Detect point-features using some Detector
      switch(featureType)
      {
            case SURF_FEATURES:
                  surfDetector->detect(frame[fs_index], featureSet[fs_index]);//, imageMask);
                  break;
            case ORB_FEATURES:
                  orbDetector->detect(frame[fs_index], featureSet[fs_index]);//, imageMask);
                  break;
            case BRISK_FEATURES:                  
                  briskDetector->detect(frame[fs_index], featureSet[fs_index]);//, imageMask);
                  break;                  
            default:
                  break;
      }
      
      //2. Extract descriptors for each detected feature. 
      fDescriptor->compute(frame[fs_index],featureSet[fs_index],descriptors[fs_index]);
        
      if (verbose)
      {
            std::cout << "Number of point features found: " << featureSet[fs_index].size() << std::endl;
            //std::cout << "descriptors = "<< std::endl << descriptors[fs_index] << std::endl;                 
            //std::cout << "Descriptor matrix: " << descriptors[fs_index].rows << "; " << descriptors[fs_index].cols << std::endl;                
            //for(unsigned int ii=0; ii<featureSet[fs_index].size(); ii++) std::cout << "(" << featureSet[fs_index].at(ii).pt.x << "," << featureSet[fs_index].at(ii).pt.y << ")" << std::endl;
      }
}

void CpointTracker::findCorrespondences()
{
	std::vector<int> indexPast, indexCurrent;
	unsigned int ii, jj;
	cv::Point2f p1, p2, auxPt;
	double dd;
	
	//reset vectors
	matches.clear();
	pointSet[fs_index].clear();
	pointSet[!fs_index].clear();

	//check for the existence of features
	if ( (featureSet[fs_index].size() == 0) || (featureSet[!fs_index].size() == 0) )
	{
		if (verbose) std::cout << "CpointTracker::findCorrespondences(): Unable to find correspondences. Empty set !" << std::endl;
		return;
	}
		
	//set matching mask only allowing "small vehicle motion" correspondences 
	slowMotionMask = cv::Mat::zeros(featureSet[!fs_index].size(), featureSet[fs_index].size(), CV_8UC1);
	for(ii=0; ii<featureSet[!fs_index].size(); ii++)
	{
		p1.x = featureSet[!fs_index].at(ii).pt.x;
		p1.y = featureSet[!fs_index].at(ii).pt.y;
		for(jj=0; jj<featureSet[fs_index].size(); jj++)
		{
			p2.x = featureSet[fs_index].at(jj).pt.x;
			p2.y = featureSet[fs_index].at(jj).pt.y;
			dd = cv::norm(p1-p2);
			if ( dd < MAX_CORRESPONDENCE_DIST_DEFAULT )
			{
				slowMotionMask.at<unsigned char>(ii,jj) = 255; //allows ij correspondence
				//std::cout << ii << "," << jj << ": " << "dd = " << dd << std::endl;
			}
		}
	}

	//compute matches
      matcher->match(descriptors[!fs_index],descriptors[fs_index], matches, slowMotionMask);
      if (verbose) std::cout << "Number of correspondences found: " << matches.size() << std::endl;
      
      // Convert keypoints into Point2f
      for (ii=0; ii<matches.size(); ii++) 
      {
            auxPt.x = featureSet[!fs_index].at(matches[ii].queryIdx).pt.x;
            auxPt.y = featureSet[!fs_index].at(matches[ii].queryIdx).pt.y;	
            pointSet[!fs_index].push_back((auxPt));
            auxPt.x = featureSet[fs_index].at(matches[ii].trainIdx).pt.x;
            auxPt.y = featureSet[fs_index].at(matches[ii].trainIdx).pt.y;	
            pointSet[fs_index].push_back((auxPt));
      }

}

void CpointTracker::updateTracks()
{
	std::list<CtrackStamped> newTracks;
	std::list<CtrackStamped>::iterator iiTrack;	
	unsigned int ii;
	CtrackStamped auxTrack;
	bool isNewTrack;
	
	for (ii=0; ii<pointSet[fs_index].size(); ii++ )
	{	
		//first check if point was already tracked
		isNewTrack = true;//sets flag to true
		for(iiTrack = trackList.begin(); iiTrack != trackList.end(); iiTrack++)
		{
			if ( iiTrack->isLastPoint(pointSet[!fs_index][ii]) ) //matching case
			{
				isNewTrack = false;
				iiTrack->addPoint(this->frameTS,pointSet[fs_index][ii]);
				break;
			}
		}
		
		//point ii was not tracked, so a new track is created
		if (isNewTrack)
		{
			if (verbose) std::cout << "new track!" << std::endl;
			auxTrack.setId(nextTrackId);
			nextTrackId ++;
			auxTrack.setVizColor();
			auxTrack.addPoint(this->frameTS,pointSet[fs_index][ii]);
			newTracks.push_back(auxTrack);
			auxTrack.clear();
		}
	}
	
	//remove old tracks , older than OLD_TRACK_LATENCY
	iiTrack = trackList.begin(); 
	while( iiTrack != trackList.end() )
	{
// 		if ( iiTrack->getLastTs() < (this->frameTS.get()-OLD_TRACK_LATENCY) )
                if ( iiTrack->getLastTs() < (this->frameTS-OLD_TRACK_LATENCY) )
		{
			iiTrack = trackList.erase(iiTrack);
		}
		else
		{
			iiTrack++;
		}
	}
	
	//add new tracks to the trackList set
	for(iiTrack = newTracks.begin(); iiTrack != newTracks.end(); iiTrack++)
	{
		trackList.push_back(*iiTrack);
	}
	newTracks.clear();
}


void CpointTracker::displayOutput(unsigned int vMode, bool save)
{
	std::ostringstream  fileName;
	std::vector<int> params;
	bool displayImageReady = false;
	std::list<CtrackStamped>::iterator iiTrack;	
	cv::vector<cv::KeyPoint> keyPts;
	unsigned int ii;
        std::ostringstream ssVlabel;
        cv::Point2f pxV;
		
	//build image to be shown
	switch(vMode)
	{
		case VIEW_POINTS: //Draw keypoints to displayImage
			cv::drawKeypoints( frame[fs_index], featureSet[fs_index], displayImage, 255, cv::DrawMatchesFlags::DEFAULT );	
			displayImageReady = true;
			break;
			
		case VIEW_CORRESPONDENCES: //draws matching results
			if ( matches.size() != 0 )
			{
				cv::drawMatches(frame[!fs_index], featureSet[!fs_index], frame[fs_index], featureSet[fs_index], matches, displayImage,cv::Scalar::all(-1),0, std::vector<char>(), cv::DrawMatchesFlags::DEFAULT);//DRAW_RICH_KEYPOINTS
				displayImageReady = true;
			}
			break;
			
            case VIEW_TRACKS:
            case VIEW_TRACKS_VELOCITY:
			displayImage = frame[fs_index];
			for(iiTrack = trackList.begin(); iiTrack != trackList.end(); iiTrack++)
			{
				if ( iiTrack->getTrackSize() > 10 )
				{
                              for(ii=0; ii<iiTrack->getTrackSize(); ii++)
                              {
                                    keyPts.push_back(cv::KeyPoint(iiTrack->getPoint(ii),1));
                              }
                              if (VIEW_TRACKS_VELOCITY)
                              {
                                    ssVlabel.str("");
                                    iiTrack->getTrackVelocity(pxV);
                                    ssVlabel << (int)cv::norm(pxV);
                                    cv::putText(displayImage,ssVlabel.str(),iiTrack->getPoint(ii-1),cv::FONT_HERSHEY_SIMPLEX,0.4,iiTrack->getVizColor(),1);
                              }                                                
                  
                              cv::drawKeypoints( displayImage, keyPts, displayImage, iiTrack->getVizColor(), cv::DrawMatchesFlags::DEFAULT );	                                        
                              keyPts.clear();
                              if (verbose) iiTrack->print();
				}
			}
			//cv::drawKeypoints( frame[fs_index], keyPts, displayImage, 255, cv::DrawMatchesFlags::DEFAULT );	
			displayImageReady = true;
			break;
		
		case VIEW_CORRESPONDENCE_MASK:
			if ( (slowMotionMask.rows !=0) && (slowMotionMask.cols !=0) ) 
			{
				displayImage = slowMotionMask;
				displayImageReady = true;
			}
			break;
			
		default:
			std::cout << "CpointTracker::displayOutput(). Unknown View mode. (vMode = " << vMode << ")" << std::endl;
			break;		
	}
	
	//show image
	if(displayImageReady)
	{
		cv::imshow("Point Tracker", displayImage);// show the image		
		cv::waitKey(50); // wait for a key
	}
	
	//Save displayed image to disk
	if ( (save) && (displayImageReady) )
	{
		fileName << "/home/andreu/Desktop/cvFrames/frame" << iterationId << ".jpg";
		params.push_back(CV_IMWRITE_JPEG_QUALITY);
		params.push_back(100);
		cv::imwrite(fileName.str(), displayImage, params);
		iterationId++;
	}
}
