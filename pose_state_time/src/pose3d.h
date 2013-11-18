
#ifndef position3d_H
#define position3d_H

//std
#include <iostream>
#include <cmath>

//dlib
#include "dlib/matrix.h"

//poseStateTime
#include "angle.h"

//constants
static const unsigned int OLD_ANGLES = 0x01;
static const unsigned int OLD_MATRIX = 0x02;
static const unsigned int OLD_QUATERNION = 0x04;
static const unsigned int UPDATED = 0x0;
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
	protected:
            /** \brief location point
             * 
             * Location: x,y,z
             * 
             */
            dlib::matrix<double,3,1> xyz;
            
		/** \brief Heading
		 * 
		 * Heading angle. First rotation, in z axis. Heading is in (-pi, pi].
		 * 
		*/
		Cangle hh; 

		/** \brief Pitch
		 * 
		 * Pitch angle. Second rotation, in the new (once rotated) y axis. Pitch is in (-pi/2, pi/2].
		 * 
		*/
		Cangle pp; 

		/** \brief Roll
		 * 
		 * Roll angle. Third rotation, in the new (twice rotated) x axis. Roll is in (-pi, pi].
		 * 
		*/
		Cangle rr; 
		
		/** \brief Rotation matrix
		 * 
		 * Full rotation matrix representing the orientation of the position referenced to a given coordinate frame
		 * It holds in its columns the normalized coordinates of the three axis (forward, left, up) in terms of a given coordinate frame
		 * 
		*/
		Matrix3f mR;
            
            /** \brief Unit quaternion
             * 
             * quaternion representation of the orientation wrt the world coordiate frame  
             * 
             **/
            Cquaternion qt;            

		/**
		 * \brief Status value to synchronize rotation representations
		 * 
		 * pStatus allows internal synchronization of euler angles, matrix and quaternion rotation representations. It has the following values:
             *    - OLD_ANGLES: Euler representation has not been updated from the last matrix / quaternion operation
             *    - OLD_MATRIX: Matrix representation has not been updated from the last euler angle / quaternion operation
             *    - OLD_QUATERNION: Quaternion representation has not been updated from the last euler angle / matrix operation
             *    - UPDATED: All three representations are synchronized
		 * 
		*/
		unsigned int pStatus;
		
	public:
		/**
		 * \brief Constructor without coordinates
		 * 
		 * Constructor without coordinates. xx,yy,zz are initialized to (0,0,0) and oreintation angles to 0.
		 * 
		*/		
		Cposition3d();
		
		/**
		 * \brief Constructor with coordinates
		 * 
		 * Constructor with coordinates. Position is initialized to (x0,y0,z0) location and (h0,p0,r0) orientation angles (heading, pitch and roll)
		 * By default, angles should be provided in radians. To pass angle values in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		Cposition3d(double cx, double cy, double cz, double ah, double ap, double ar, bool rd=inRADIANS);
		
		/**
		 * \brief Destructor
		 * 
		 * Destructor
		 * 
		*/		
		virtual ~Cposition3d();

		/**
		 * \brief Set heading
		 * 
		 * Sets heading angle. By default, heading should be provided in radians. To pass heading in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		void setH(double ah, bool rd=inRADIANS); 
		
		/**
		 * \brief Set pitch
		 * 
		 * Sets pitch angle. By default, pitch should be provided in radians. To pass pitch in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		void setP(double ap, bool rd=inRADIANS); 
		
		/**
		 * \brief Set roll
		 * 
		 * Sets roll angle. By default, roll should be provided in radians. To pass roll in degrees, set rd parameter to "inDEGREES".
		 * 
		*/		
		void setR(double ar, bool rd=inRADIANS); 
		
		/**
		 * \brief Set Euler angles
		 * 
		 * Sets heading, pitch and roll angles. By default, angles should be provided in radians. To pass angles in degrees, set rd parameter to "inDEGREES".
		 * Heading, pitch and roll follows respectively Euler convention ZYX. 
		 * 
		*/		
		void setHPR(double ah, double ap, double ar, bool rd=inRADIANS);

		/**
		 * \brief Set full position
		 * 
		 * Sets point coordinates and euler angles heading, pitch and roll. By default, angles should be provided in radians. To pass angles in degrees, set rd parameter to "inDEGREES".
		 * Heading, pitch and roll follows respectively Euler convention ZYX. 
		 * 
		*/		
		void setFullPose(double cx, double cy, double cz, double ah, double ap, double ar, bool rd=inRADIANS);
		
		/**
		 * \brief Set position by coordinates
		 * 
		 * Sets full position from pose coordinates
		 * 
		*/		
		void setFullPose(Cposition3d *pose);

		/**
		 * \brief Set position by coordinates
		 * 
		 * Sets full position from pose coordinates
		 * 
		*/		
		void setFullPose(Cposition3d & pose);

		/**
		 * \brief Set position by matrix
		 * 
		 * Sets full position from matrix values
		 * 
		*/		
		void setFullPoseByMatrix(Cposition3d *pose);

		/**
		 * \brief Set position by matrix
		 * 
		 * Sets full position from matrix values
		 * 
		*/		
		void setFullPoseByMatrix(Cposition3d & pose);

		/**
		 * \brief Set position by vectors 
		 * 
		 * Sets x,y,z and sets rotation matrix with the forward and left vectors expressed as eigen vectors. Up vector is then derived as: up = fw x lft 
		 * 
		*/		
		void setFullPose( Vector3f &xyzV, Vector3f &fwV, Vector3f &lfV);

		/**
		 * \brief Set position by vectors 
		 * 
		 * Sets x,y,z and sets rotation matrix with the forward and left vectors expressed by its coordinate values. Up vector is then derived as: up = fw x lft.
		 * 
		*/		
		void setFullPose(double cx, double cy, double cz, double fwx, double fwy, double fwz, double lfx, double lfy, double lfz);

		/**
		 * \brief Set position by Homogeneous matrix
		 * 
		 * Sets x,y,z and rotation  matrix mR from the homogeneous matrix H.
		 * 
		*/		
		void setHomogeneous(Matrix4f *H);
		
		/**
		 * \brief Set orientation from quaternion
		 * 
		 * Sets rotation matrix from quaternion
		 * 
		*/		
		void setQuaternion(const double qReal, const double qi, const double qj, const double qk);
		
		/**
		 * \brief Get heading 
		 * 
		 * Gets heading value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
		 * 
		*/		
		double getH(bool rd=inRADIANS); 

		/**
		 * \brief Get pitch
		 * 
		 * Gets pitch value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
		 * 
		*/		
		double getP(bool rd=inRADIANS); 
		
		/**
		 * \brief Get roll
		 * 
		 * Gets roll value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
		 * 
		*/		
		double getR(bool rd=inRADIANS);
		
		/**
		 * \brief Get homogeneous matrix
		 * 
		 * Sets H matrix as the homogeneous transformation matrix representing this position.
		 * 
		*/		
		void getHomogeneous(Matrix4f *H); /**returns to H the homogeneous coordinate version of mR*/
		
		/**
		 * \brief Get LookAt
		 * 
		 * Sets lav data struct with the "look at" parameters  representing this position.
		 * 
		*/		
		void getLookAtValues(lookAtValues *lav);

		/**
		 * \brief Get xyzFwdLeft vectors 
		 * 
		 * Sets vv as the concatenation of the components of point location, forward and left vectors. vv should have 9 memory free double positions.
		 * 
		*/		
		void getXYZFwLf(double *vv); 
		
		/**
		 * \brief Get quaternion
		 * 
		 * Gets quaternion parameters from orientation matrix
		 * 
		*/		
		void getQuaternion(double &qReal, double &qi, double &qj, double &qk);
		
		/**
		 * \brief Forward motion
		 * 
		 * Moves forward the position dd units. Forward direction is indicated by the forward vector, that is the first column of the rotation matrix
		 * 
		*/		
		void moveForward(double dd);

		/**
		 * \brief Axis rotation
		 * 
		 * Rotate position by alpha radians along the axis defined by (ux,uy,uz). Axis coordinated are referenced to the same frame as position
		 * To express rotation in degrees, set rd boolean to "inDEGREES"
		 * 
		*/		
		void rotateUaxis(double alpha, double ux, double uy, double uz, bool rd=inRADIANS);
		
		/**
		 * \brief z-Axis rotation
		 * 
		 * Rotate position by alpha radians along the position z-axis. This implements a vehicle head-turn motion.
		 * To express rotation in degrees, set rd boolean to "inDEGREES"
		 * 
		*/		
		void turnZaxis(double alpha, bool rd=inRADIANS);

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

	protected:
		/**
		 * \brief Update rotation matrix from Euler angles
		 * 
		 * Computes rotation matrix from current Euler angles.
		 *
		*/																						
		void updateRmatrix();

		/**
		 * \brief Update Euler angles from rotation matrix
		 * 
		 * Computes Euler angles from current rotation matrix.
		 *
		*/																						
		void updateEulerAngles();
};
#endif
