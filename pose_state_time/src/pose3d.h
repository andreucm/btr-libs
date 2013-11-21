
#ifndef position3d_H
#define position3d_H

//std
#include <iostream>
#include <cmath>

//dlib
#include "dlib/matrix.h"

//poseStateTime
#include "rotation.h"

//constants
static const bool TOTHIS = 0;
static const bool TOREF = 1;

/** \brief LookAt struct
 *
 * This struct groups the parameters to represent positions (viewpoints) in some common openGL functions. 
 * Parameters are as follows: 
 *	- ex,ey,ez: eye location. 
 *	- ax,ay,az: target point location ('at').
 *	- ux,uy,uz: up vector (normalized) of the viewpoint position.
 *
*/
struct lookAtValues
{
	double ex,ey,ez; //eye location
	double ax,ay,az; //target point ('at')
	double ux,uy,uz; //up vector
};

/** \brief Positions in 3D space: point+orientation
 *
 * Cposition3d class implements data structure and functions to manipulate positions in a 3D space. It can be used also to represent coordinate frames.
 *
 * Positions are composed by location and orientation:
 *	- LOCATION: x,y,z are the coordinates where the position is placed. Inherited from Cpoint3d class. 
 *
 *    - ORIENTATION: This class holds three representations of the orientation:
 *		- Euler angles ZYX representation: heading (hh), pitch (pp) and roll(rr)
 *		- Full rotation matrix
 *          - Quaternion
 *	
 * ZYX convention for the Euler angles means:
 * 	- First rotation is heading (h) in z axis, in (-pi, pi]
 *	- Second rotation is pitch (p) in the new (once rotated) y axis, in (-pi/2, pi/2]
 *	- Third rotation is roll (r) in the new (twice rotated) x axis, in (-pi, pi]
 *
 * With this convention, the rotation matrix mR is: mR=mh*mp*mr. The mR matrix has in each column the coordinates of "this" frame axis wrt the fixed/world frame.
 * 
 * To synchronize this three representations, an integer "pStatus" is used with the following "OR" values:
 *	- OLD_ANGLES: Euler representation has not been updated from the last matrix / quaternion operation
 *	- OLD_MATRIX: Matrix representation has not been updated from the last euler angle / quaternion operation
 *    - OLD_QUATERNION: Quaternion representation has not been updated from the last euler angle / matrix operation
 *	- UPDATED: All three representations are synchronized
 *
*/
class Cpose3d
{
	public:
            /** \brief location point
             * 
             * x,y,z coordinates of the frame center with respect to the reference frame
             * 
             */
            dlib::matrix<double,3,1> pt;
            
		/** \brief Rotation
		 * 
		 * Rotation of this frame (this pose) with respect to the reference frame
		 * 
		*/
		Crotation rt; 

      public:
		/**
		 * \brief Constructor without coordinates
		 * 
		 * Constructor without coordinates. xx,yy,zz are initialized to (0,0,0) and oreintation angles to 0.
		 * 
		*/		
		Cpose3d();
		
		/**
		 * \brief Constructor with coordinates
		 * 
		 * Constructor with coordinates. Position is initialized to (x0,y0,z0) point, and rotation given by Euler angles (heading, pitch, roll)
		 * By default, angles should be provided in radians. To pass angle values in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		Cpose3d(double x0, double y0, double z0, double eaH, double eaP, double eaR, bool rd=inRADIANS);
		
		/**
		 * \brief Destructor
		 * 
		 * Destructor
		 * 
		*/		
		virtual ~Cpose3d();

		/** \brief Set pose from a Cpose argument
		 * 
		 * Set this pose from a Cpose argument
             * May imply updating pose.rt.rM if it was not updated
		 * 
		*/		
		void setPose(Cpose3d & pose);

		/** \brief Set pose from ptQuat vector
		 * 
		 * Set position from ptQuat vector. 
             * ptQuat is a column 6-vector as: [x y z qi qj qk]', so real part of quaternion is set to fulfill |rt.qt|=1
		 * 
		*/		
		void setPose(const dlib::matrix<double,6,1> & ptQuat);

		/** \brief Set pose from Homogeneous matrix
		 * 
		 * Set position from Homogeneous matrix provided by argument hM
		 * 
		*/		
            void setPose(const dlib::matrix<double,4,4> & hM);
            
            /** \brief Get pose as a ptQuat vector
             * 
             * Get pose as a ptQuat vector
             * ptQuat is set as a column 6-vector as: [x y z qi qj qk]'
             * May imply updating pose.rt.qt if it was not updated
             * 
            */                      
            void getPtQuat(dlib::matrix<double,6,1> & ptQuat);
            
		/** \brief Get 4x4 homogeneous matrix
		 * 
		 * Get 4x4 homogeneous matrix
             * May imply updating pose.rt.rM if it was not updated
		 * 
		*/		
		void getHomMat(dlib::matrix<double,4,4> & hM);
		
		/** \brief Get LookAt parameters
		 * 
		 * Get LookAt parameters: "eye", "at" and "up" 
             * May imply updating pose.rt.rM if it was not updated
		 * 
		*/		
		void getLookAtValues(lookAtValues & lav);

		/** \brief Get pt-Fwd-Left vectors 
		 * 
		 * Sets vv as the concatenation of the components of this->pt , forward axiz and left axis.
             * May imply updating pose.rt.rM if it was not updated
		 * 
		*/		
		void getPtFwLf(double & vv[9]); 
		
		/** \brief Forward motion
		 * 
		 * Moves forward the position dd units. 
             * Forward direction is indicated by the forward vector, that is the first column of the rotation matrix
		 * 
		*/		
		void moveForward(double dd);
/*******************************************************************************/
		/**
		 * \brief Transofrm point
		 *
		 * Transforms point coordinates, following targetFrame value:
		 *	- TOTHIS: Point qq is initially expressed in terms of reference frame and is transformed to be in terms of "this" frame
		 *	- TOREF:  Point qq is initially expressed in terms of "this" frame and is transformed to be in terms of reference frame
		 * 
		*/		
		void coordTr(Cpoint3d *qq, bool targetFrame);
		
		/**
		 * \brief Transofrm position
		 *
		 * Transforms position coordinates, following targetFrame value:
		 *	- TOTHIS: Position ff is initially expressed in terms of reference frame and is transformed to be in terms of "this" frame
		 *	- TOREF:  Position ff is initially expressed in terms of "this" frame and is transformed to be in terms of reference frame
		 * 
		*/		
		void coordTr(Cposition3d *ff, bool targetFrame);

		/**
		 * \brief Transofrm position
		 *
		 * Transforms position coordinates, following targetFrame value:
		 *	- TOTHIS: Position ffIn is expressed in terms of reference frame and ffOut in terms of "this" frame
		 *	- TOREF:  Position ffIn is expressed in terms of "this" frame and is ffOut in terms of reference frame
		 * 
		*/		
		void coordTr(Cposition3d *ffIn, Cposition3d *ffOut, bool targetFrame);
		
		/**
		* \brief Rotate position following the twist
		*
		* Rotates position following the twist. Twist is expressed as a group of three (heading,pitch,roll) angular increments.
		* 
		*/
		void twist(double deltaH, double deltaP, double deltaR);

		//not debugged. May be they can be definetively replaced by deltaHeading(double alpha)
		//void incTheta(const double & deltaThRads);
		//double deltaTheta(Cposition3d & posB);

		/**
		 * \brief print position
		 * 
		 * Prints position in a single row, following the format "(x,y,z,h,p,r)". Angles are in radians or degrees according rd boolean (rad by default).
		*/		
		void printPosition(bool rd=inRADIANS);

		/**
		 * \brief print orientation
		 * 
		 * Prints orientation in a single row, following the format "h=hh; p=pp; r=rr)". Angles are in radians or degrees according rd boolean (rad by default).
		*/		
		void printAttitude(bool rd=inRADIANS);

		/**
		 * \brief print orientation
		 * 
		 * Prints the 3x3 rotation matrix. 
		*/		
		void printRmatrix();

		/**
		 * \brief Assignement operator
		 * 
		 * Calls setFullPose(pose).
		 *
		*/																						
		void operator=(Cposition3d & pose);

};
#endif
