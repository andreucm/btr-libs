#ifndef wState_H
#define wState_H

//std
#include <deque>

//poseStateTime lib
#include "vqStateTs.h"
#include "pose3d.h"

/** \brief Window state size
 * 
 * Number of vq states which form the window state
 * 
 **/
const unsigned int WINDOW_STATE_SIZE = 20; 

/** \brief Window state
 * 
 * CwState class represents a sliding window vector of time-stamped platform states (vq timestamped states).
 * The vector is implemented as a deque allowing push-back's/pop-front's for insert/removing platform state elements to it.
 *
 **/ 
class CwState
{
	public:
		/** \brief vector of vq-states
		 * 
		 * Vector of time-stamped platform states that forms the window state
		 * 
		 **/
		std::deque<CvqStateTs> ws;
		
	public:
		/** \brief Constructor
		 * 
		 * Default constructor.
		 * 
		 **/		
		CwState();

		/** \brief Destructor
		 * 
		 * Destructor.
		 * 
		 **/		
		~CwState();

		/** \brief Sets from a vector
		 * 
		 * Sets this->ws from a dlib::vector expression
		 * 
		 **/
		void set(const dlib::matrix<double,VQ_STATE_DIMENSION*WINDOW_STATE_SIZE,1> & wsVec);
		
		/** \brief Gets a vector 
		 * 
		 * Gets a dlib::vector expression of the this->ws deque. 
		 * wsVec is a preallocated vector that will be set with values of wState
		 * 
		 **/
		void getVector(dlib::matrix<double,VQ_STATE_DIMENSION*WINDOW_STATE_SIZE,1> & wsVec) const;
		
            /** \brief Checks if time stamps are within window period
            *
            * Checks if time stamps are within window period
            * Returns TRUE if (t0 < te) AND (t0 within this->ws) AND (te winthin this->ws)
            * Otherwise, returns FALSE
            * 
            **/
            bool tsInWsPeriod(const CtimeStamp & t0, const CtimeStamp & te) const;
            
            /** \brief Gets rigid body transform between ws states at t0 and te. 
             * 
             * Gets rigid body transform between ws states at t0 and te. 
             * Returned pose is frame at te with respect to frame at t0.
             * Returned pose is parametrized as a xyz translation + rotation through the Cpose3d class
             * It is assumed that t0 and te have passed tsInWsPeriod() check.
             * 
             */
            void getTransform(const CtimeStamp & t0, const CtimeStamp & te, Cpose3d & pose) const;
            
		/** \brief print
		 * 
		 * Prints the window state
		 * 
		 */
		void print() const;
};
#endif
