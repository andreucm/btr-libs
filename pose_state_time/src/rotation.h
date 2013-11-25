
#ifndef rotation_H
#define rotation_H

//std
#include <iostream>
#include <cmath>

//dlib
#include "dlib/matrix.h"

//poseStateTime lib
#include "angle.h"
#include "quaternion.h"

//constants
static const unsigned short OLD_EULER = 0x01;
static const unsigned short OLD_MATRIX = 0x02;
static const unsigned short OLD_QUATERNION = 0x04;
static const unsigned short UPDATED = 0x0;
static const double EPSILON = 1e-12;

/** \brief Rotations in 3D space
 *
 * Crotation class implements orientation of frames and poses in 3D space. 
 * The class holds three representations of the orientation:
 *          - Euler angles ZYX representation: heading (hh), pitch (pp) and roll(rr)
 *          - Full rotation matrix
 *          - Quaternion
 *    
 * ZYX convention for the Euler angles means:
 *    - First rotation is heading (h) in z axis, in (-pi, pi]
 *    - Second rotation is pitch (p) in the new (once rotated) y axis, in (-pi/2, pi/2]
 *    - Third rotation is roll (r) in the new (twice rotated) x axis, in (-pi, pi]
 *
 * With this convention, the rotation matrix mR is: mR=mh*mp*mr. The mR matrix has in each column the coordinates of "this" frame axis wrt the fixed/world frame.
 * 
 * To synchronize this three representations, an integer "pStatus" is used with the following "OR" values:
 *    - OLD_EULER: Euler representation has not been updated from the last matrix / quaternion operation
 *    - OLD_MATRIX: Matrix representation has not been updated from the last euler angle / quaternion operation
 *    - OLD_QUATERNION: Quaternion representation has not been updated from the last euler angle / matrix operation
 *    - UPDATED: All three representations are synchronized
 *
*/
class Crotation
{
      protected:
            /** \brief Euler Angles
             * 
             * Euler angles following ZYX convention: heading, pitch roll
             *    - Heading is in (-pi, pi]. First rottaion on Z axis
             *    - Pitch is in (-pi/2, pi/2]. Second rotation, in the new (once rotated) Y axis.
             *    - Roll is in (-pi, pi]. Third rotation, in the new (twice rotated) X axis.
             * 
            */
            Cangle ea[3]; 

            /** \brief Rotation matrix
             * 
             * Full rotation matrix representing the orientation of the frame referenced to a given coordinate frame
             * It holds in its columns the normalized coordinates of the three axis (forward, left, up) in terms of a given coordinate frame
             * 
            */
            dlib::matrix<double,3,3> rM;
            
            /** \brief Unit quaternion
             * 
             * quaternion representation of the orientation wrt the world coordiate frame  
             * 
            */
            Cquaternion qt;          

            /** \brief Status value to synchronize rotation representations
             * 
             * rStatus allows internal synchronization of euler angles, matrix and quaternion rotation representations. It has the following values:
             *    - OLD_EULER: Euler representation has not been updated from the last matrix / quaternion operation
             *    - OLD_MATRIX: Matrix representation has not been updated from the last euler angle / quaternion operation
             *    - OLD_QUATERNION: Quaternion representation has not been updated from the last euler angle / matrix operation
             *    - UPDATED: All three representations are synchronized
             * 
            */
            unsigned short rStatus;
            
      public:
            /** \brief Constructor without arguments
             * 
             * Constructor without coordinates. Orientation aligned to refrence frame ( ea=0, rM=I3, qt=(1,0,0,0) )
             * 
            */          
            Crotation();
            
            /** \brief Constructor with euler angles
             * 
             * Constructor with euler angles (euler convention ZYX), provided with the order: heading, pitch and roll
             * By default, angles should be provided in radians. To pass angle in degrees, set rd parameter to "inDEGREES".
             * 
            */          
            Crotation(const double hh, const double pp, const double rr, const bool rd=inRADIANS);

            /** \brief Constructor with rM matrix
             * 
             * Constructor with rotation matrix
             * 
            */          
            Crotation(const dlib::matrix<double,3,3> & mat);
            
            /** \brief Constructor with quaternion
             * 
             * Constructor with quaterinion
             * 
            */          
            Crotation(const Cquaternion & quat);

            /** \brief Destructor
             * 
             * Destructor
             * 
            */          
            virtual ~Crotation();

            /** \brief Set rotation by Euler angles
             * 
             * Set euler angles (euler convention ZYX), provided with the order: heading, pitch and roll 
             * By default, angles should be provided in radians. To pass angle in degrees, set rd parameter to "inDEGREES".
             * Pitch (pp) angle required in [-pi,pi] interval ( [-90,90] if inDEGREES )
             * 
            */          
            void setEuler(const double hh, const double pp, const double rr, const bool rd=inRADIANS);
            
            /** \brief Set rotation by matrix
             * 
             * Sets rotation from a matrix
             * 
            */          
            void setMatrix(const dlib::matrix<double,3,3> & mat);

            /** \brief Set rotation by quaternion
             * 
             * Sets rotation from a quaternion
             * 
            */          
            void setQuaternion(const Cquaternion & quat);

            /** \brief Set rotation by quaternion
             * 
             * Sets rotation from a quaternion providing only imaginary vector. 
             * It assumes unit quaternion to set the real part
             * 
            */          
            void setQuaternion(const dlib::matrix<double,3,1> & qijk);
            
            /** \brief Get heading 
             * 
             * Gets heading value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
             * May imply updating euler angles if they were not updated
             * 
            */          
            double head(bool rd=inRADIANS); 

            /** \brief Get pitch
             * 
             * Gets pitch value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
             * May imply updating euler angles if they were not updated
             * 
            */          
            double pitch(bool rd=inRADIANS); 
            
            /** \brief Get roll
             * 
             * Gets roll value. By default angle is returned in radians. Set rd parameter to "inDEGREES" is returned value is required in degrees.
             * May imply updating euler angles if they were not updated
             * 
            */          
            double roll(bool rd=inRADIANS);
            
            /** \brief Get rotation matrix
             * 
             * Gets the rotation matrix, returned at mat argument
             * May imply updating matrix if it was not updated
             * 
            */          
            void getMatrix(dlib::matrix<double,3,3> & mat); 
            
            /** \brief Get quaternion
             * 
             * Gets the quaternion, returned at quat argument
             * May imply updating quaternion if it was not updated
             * 
            */          
            void getQuaternion(Cquaternion & quat);
            
            /** \brief Get quaternion
             * 
             * Gets the quaternion, returned as a 3-vector of the imaginary components
             * May imply updating quaternion if it was not updated
             * 
            */          
            void getQuaternion(dlib::matrix<double,3,1> & quat);
            
            /** \brief Get quaternion
             * 
             * Gets the quaternion, returned as a 4-vector. First component is real, then ijk
             * May imply updating quaternion if it was not updated
             * 
            */          
            void getQuaternion(dlib::matrix<double,4,1> & quat);

            /** \brief Gets axis angle representation
             * 
             * Gets axis angle representation 
             * May require quaternion update and/or normalization
             * 
             **/
            void getAxisAngle(dlib::matrix<double,3,1> &axis, double & angle);
            
            
            /** \brief Axis rotation
             * 
             * Rotate position by alpha radians along the axis defined by axis[3](ux,uy,uz). 
             * Axis coordinates (ux,uy,uz) are referenced to the same frame as this rotation
             * To express rotation in degrees, set rd boolean to "inDEGREES"
             * 
            */          
            void rotateUaxis(const double alpha, double ux, double uy, double uz, bool rd=inRADIANS);
            
            /** \brief Heading rotation
             * 
             * Rotate frame by alpha radians along "this" z-axis, implementing a vehicle heading turn.
             * To express rotation in degrees, set rd boolean to "inDEGREES"
             * 
            */          
            void turnHeading(const double alpha, bool rd=inRADIANS);
            
            /** \brief print rotation
             * 
             * Prints rotation as:
             *    - Euler: (h,p,r)". Angles are in radians or degrees according rd boolean (radians by default).
             *    - Matrix: row-wise: (rM00, rM01, rM02, rM10 ...rM22)
             *    - Quaternion: (qR, qi, qj, qk)
             * May imply updating ea, rM or qt if they were not updated
             * 
            */          
            void print(const bool rd=inRADIANS);

            /** \brief Assignement operator
             * 
             * Copy is done through matrix, then updating eulers and quaternion
             * May imply updating rt.rM if it was not updated
             *
            */                                                                                                                                  
            void operator=(Crotation & rt);

      protected:
            /** \brief Set rotation by Euler angles. Does not update status
             * 
             * Set euler angles (euler convention ZYX), provided with the order: heading, pitch and roll 
             * By default, angles should be provided in radians. To pass angle in degrees, set rd parameter to "inDEGREES".
             * Does not update the status
             * 
            */          
            void setEulerWithoutStatusUpdate(const double hh, const double pp, const double rr, const bool rd=inRADIANS);             
            
            /** \brief Update Euler angles from rotation matrix
             * 
             * Computes Euler angles from current rotation matrix.
             *
            */                                                                                                                                  
            void updateEuler();
            
            /** \brief Update rotation matrix from Euler angles
             * 
             * Computes rotation matrix from current Euler angles.
             *
            */                                                                                                                                  
            void updateMatrix();         
            
            /** \brief Update quaternion from rotation matrix
             * 
             * Computes quaternion from rotation matrix
             *
            */                                                                                                                                  
            void updateQuaternion();         
            
};
#endif
