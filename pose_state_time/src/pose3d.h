
#ifndef pose3d_H
#define pose3d_H

//std
#include <iostream>
#include <cmath>

//dlib
#include "dlib/matrix.h"

//pose_state_time lib
#include "rotation.h"

//constants
static const bool FROM_THIS_TO_REF = 0;
static const bool FROM_REF_TO_THIS = 1;

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
 * Cpose3d class implements data structure and functions to manipulate poses or coordinate frames in 3D space. 
 *
 * Poses are composed by location point and a rotation:
 *	- LOCATION: pt is a 3.vector expressing x,y,z coordinates of the point where the pose is placed.
 *    - ROTATION: rt is a Crotation object representing the rotation as Euler angles, rot matrix or quaternion
 *	
 * Conventions are:
 *    - Euler angles follow the ZYX convention, named heading, pitch and roll.
 *    - Rotation matrix hold the frame unit vectors in their columns. First column is X axis, second Y axis and third Z axis.
 *    - First element of quaternions, qq(0), is the real part. Then qq(1),qq(2),qq(3) are i,j,k respectively.
 *
 * ZYX convention for the Euler angles means:
 * 	- First rotation is heading (h) in z axis, in (-pi, pi]
 *	- Second rotation is pitch (p) in the new (once rotated) y axis, in (-pi/2, pi/2]
 *	- Third rotation is roll (r) in the new (twice rotated) x axis, in (-pi, pi]
 * With this convention, the rotation matrix mR is: mR=mh*mp*mr. The mR matrix has in each column the coordinates of "this" frame axis wrt the fixed/world frame.
 * 
 * Both point location and rotation are expressed with respect to an arbitrary frame, called by convention REFERENCE. 
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
		/** \brief Constructor without coordinates
		 * 
		 * Constructor without coordinates. xx,yy,zz are initialized to (0,0,0) and oreintation angles to 0.
		 * 
		*/		
		Cpose3d();
		
		/** \brief Constructor with coordinates
		 * 
		 * Constructor with coordinates. Position is initialized to (x0,y0,z0) point, and rotation given by Euler angles (heading, pitch, roll)
		 * By default, angles should be provided in radians. To pass angle values in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		Cpose3d(const double x0, const double y0, const double z0, const double eaH, const double eaP, const double eaR, const bool rd=inRADIANS);
		
		/** \brief Destructor
		 * 
		 * Destructor
		 * 
		*/		
		virtual ~Cpose3d();

		/** \brief Set pose from a Cpose argument
		 * 
		 * Set this pose from a Cpose argument
             * May imply updating this->rt.rM if it was not updated
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
             * May imply updating this->rt.rM if it was not updated
		 * 
		*/		
		void getHmat(dlib::matrix<double,4,4> & hM);
		
		/** \brief Get LookAt parameters
		 * 
		 * Get LookAt parameters: "eye", "at" and "up" 
             * May imply updating this->rt.rM if it was not updated
		 * 
		*/		
		void getLookAt(lookAtValues & lav);

// 		/** \brief Get pt-Fwd-Left vectors 
// 		 * 
// 		 * Sets vv as the concatenation of the components of this->pt , forward axiz and left axis.
//              * May imply updating this->rt.rM if it was not updated
// 		 * 
// 		*/		
// 		void getPtFwLf(double & vv[9]); 
		
		/** \brief Forward motion
		 * 
		 * Moves forward the position dd units. 
             * Forward direction is indicated by the forward vector, that is the first column of the rotation matrix
		 * 
		*/		
		void moveForward(double dd);

		/** \brief Transofrm point
		 *
		 * Transforms point coordinates, following targetFrame value:
		 *	- FROM_REF_TO_THIS: Assumes point aa is initially expressed in terms of reference frame and is transformed to be in terms of "this" frame
		 *	- FROM_THIS_TO_REF: Assumes point aa is initially expressed in terms of "this" frame and is transformed to be in terms of reference frame
		 * May imply updating this->rt.rM if it was not updated
		*/		
		void trf(dlib::matrix<double,3,1> & aa, const bool targetFrame);
		
		/** \brief Transofrm pose
             *
		 * Transforms pose coordinates, following targetFrame value:
		 *	- FROM_REF_TO_THIS: Assumes pose po is initially expressed in terms of reference frame and is transformed to be in terms of "this" frame
		 *	- FROM_THIS_TO_REF: Assumes pose po is initially expressed in terms of "this" frame and is transformed to be in terms of reference frame
		 * 
             * May imply updating this->rt.rM if it was not updated
		*/		
		void trf(Cpose3d & po, const bool targetFrame);

		/** \brief Transofrm pose
		 *
		 * Transforms position coordinates, following targetFrame value:
		 *	- FROM_REF_TO_THIS: Pose poIn is expressed in terms of reference frame and poOut in terms of "this" frame
		 *	- FROM_THIS_TO_REF: Pose poIn is expressed in terms of "this" frame and is poOut in terms of reference frame
		 *
             * May imply updating this->rt.rM  and poIn.rt.rM if they were not updated
		*/		
		void trf(Cpose3d & poIn, Cpose3d & poOut, const bool targetFrame);
		
		/** \brief Rotate this pose following the twist
		*
		* Rotates position following the twist. 
            * Twist is expressed as a group of three (heading,pitch,roll) angular increments.
		* 
		*/
		void twist(const double deltaH, const double deltaP, const double deltaR);

		/** \brief print pose
		 * 
		 * Prints pose in a single row, following the format "(x,y,z,h,p,r,qr,qi,qj,qk,m00,m01,m02,m10,..,m22)". 
             * Angles are in radians or degrees according rd boolean (radians by default).
             * Finishes the line with std::endl if endLine is set to true (default)
             * 
             * May imply updating this->rt.rM, this->rt.ea and/or this->rt.qt
		*/		
		void printPose(const bool rd=inRADIANS, const bool endLine = true);

		/** \brief Assignement operator
		 * 
		 * Calls setPose(Cpose3d & pose).
		 *
		*/																						
		void operator=(Cpose3d & pose);
};
#endif
