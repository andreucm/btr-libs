#ifndef vqState_H
#define vqState_H

//pose_state_time lib
#include "quaternion.h"

/** \brief dimensionality of vq state 
 **/
const unsigned int VQ_STATE_DIMENSION = 6;

/** \brief Platform state
* 
* CvqState class. Velocity-Quaternion state composed by:
*   - vv: xyz linear velocities
*   - qt: quaternion representing the body orientation
*
* When a state vector is required as a single column vector, this class returns a 6-dimensional vector (real part of the quaternion is not returned).
* When the quaternion is set from a 6-vector, only imaginary part must be provided. The real part is computed to fulfill |qt|=1
* 
**/
class CvqState
{
	public:
		/** \brief Linear Velocity
		 * 
		 * linear velocity wrt the world coordinate frame
		 * 
		 **/
		dlib::matrix<double,3,1> vv;
		
		/** \brief Rotation represented with a quaternion
		 * 
		 * quaternion representation of the orientation wrt the world coordiate frame  
		 * 
		 **/
		Cquaternion qt;
		
	public:
		/** \brief Default constructor
		 * 
		 * Default constructor.
		 * 
		 **/		
		CvqState();

		/** \brief Constructor with arguments
		 * 
		 * constructor with object arguments
		 * 
		 **/		
		CvqState(const dlib::matrix<double,3,1> vel, const Cquaternion & quat);

            /** \brief Constructor with raw arguments
            * 
            * constructor with raw arguments
            * 
            **/            
            CvqState(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3); 
                
		/** \brief Destructor
		 * 
		 * Destructor.
		 * 
		 **/		
		virtual ~CvqState();

		/** \brief Set with raw arguments
		 * 
		 * Sets vv and imaginary part of the quaternion. 
		 * Real part of qt is set to fulfill norm(qt)=1
		 * 
		 **/
		void set(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3);
		
		/** \brief Get vector form
		 * 
		 * Sets vq argument as the VQ_STATE_DIMENSION-vector composed by full vv and imaginary part of the quaternion (qt(1:3))
		 * 
		 */
            void getVector(dlib::matrix<double,VQ_STATE_DIMENSION,1> & vq) const;
		
		/** \brief Mahalanobis distance
		 * 
		 * Returns mahalanobis distance between this and vq, upon cov matrix covM
		 * 
		 **/
		double mahalanobisDistance(const CvqState & vq, const dlib::matrix<double,VQ_STATE_DIMENSION,VQ_STATE_DIMENSION> & covM) const; 
		
		/** \brief Returns linear body velocities
		 * 
		 * Returns linear velocities with respect to the body frame (orientation represented by this->qt)
		 * 
		 **/
		dlib::matrix<double,3,1> bodyVelocities() const;
                
            /** \brief Time integration
            * 
            * Integrates this state during dT to obtain a new xyz position given the initial xyz provided
            * 
            **/
//             void integrate(const dlib::matrix<double,3,1> & x0, double dT, dlib::matrix<double,3,1> & xNew) const;
			
		/** \brief Print
		 * 
		 * Prints the this vq state
		 * 
		 */
            virtual void print() const;
		
		/** \brief Assignement operator
		 * 
		 * Assignement operator from a CvqState object
		 * 
		 **/
		void operator=(const CvqState & vq);
		
		/** \brief Assignement operator
		 * 
		 * Assignement operator from a vector
		 * 
		 **/
		void operator=(const dlib::matrix<double,VQ_STATE_DIMENSION,1> & vq);
		
};
#endif
