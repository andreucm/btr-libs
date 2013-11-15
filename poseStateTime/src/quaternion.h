#ifndef quaternion_H
#define quaternion_H

//std
#include <iostream>
#include <cmath>

//dlib
#include "dlib/matrix.h"

/**
* 
* Cquaternion class
* 
**/
class Cquaternion
{
	public:
		/**
		 * 
		 * quaternion components as a 4-vector. qq(0) is the real part
		 * 
		 **/
		dlib::matrix<double,4,1> qq;
		
	public:
		/**
		 * 
		 * Default constructor. q(0)=1, q(123)=0
		 * 
		 **/		
		Cquaternion();

		/**
		 * 
		 * Constructor with arguments as doubles (all components).
		 * 
		 **/		
		Cquaternion(const double q0, const double q1, const double q2, const double q3);
		
		/**
		 * 
		 * Constructor with arguments as doubles (only imaginary components).
		 * Real part is set to fulfill norm(qq)=1
		 * 
		 **/		
		Cquaternion(const double q1, const double q2, const double q3);		

		/**
		 * 
		 * Constructor with argument as a 4-vector (all components).
		 * 
		 **/		
		Cquaternion(const dlib::matrix<double,4,1> qin);
		
		/**
		 * 
		 * Constructor with argument as a 3-vector (only imaginary components).
		 * Real part is set to fulfill norm(qq)=1
		 * 
		 **/		
		Cquaternion(const dlib::matrix<double,3,1> qin);		

		/**
		 * 
		 * Destructor.
		 * 
		 **/		
		~Cquaternion();

            /**
             * 
             * sets quaternion components with arguments as doubles
             * 
             **/                    
            void set(const double q0, const double q1, const double q2, const double q3);
            
            /**
		 * 
		 * sets quaternion components with arguments as doubles (only imaginary components).
		 * Real part is set to fulfill norm(qq)=1
		 * 
		 **/				
		void set(const double q1, const double q2, const double q3);
		
            /**
             * 
             * sets quaternion components with argument as a 4-vector
             * 
             **/
            void set(const dlib::matrix<double,4,1> qin);
            
            /**
		 * 
		 * sets quaternion components with argument as a 3-vector (only imaginary components).
		 * Real part is set to fulfill norm(qq)=1
		 * 
		 **/
		void set(const dlib::matrix<double,3,1> qin);
            
            /**
             * 
             * Sets quaternion as the representation of the rotation given by rMat
             * 
             **/
            void set(const dlib::matrix<double,3,3> &rMat);
            
            /**
             * 
             * Gets the the rotation matrix represented by this quaternion
             * Matrix is returned in rM argument
             * 
             **/                                            
            void getMatrix(dlib::matrix<double,3,3> & rM) const;
            
            /**
             * 
             * Returns the rotation matrix represented by this quaternion
             * 
             **/                                
            dlib::matrix<double,3,3> getMatrix() const;

            /**
             * 
             * Gets axis angle representation from unit quaternion
             * May require quaternion mormalization
             * 
             **/
            void getAxisAngle(dlib::matrix<double,3,1> &axis, double & angle);
            		
		/**
		 * 
		 * Conjugates this quaternion
		 * 
		 **/				
		void conjugate();
		
		/**
		 * 
		 * Normalizes this quaternion
		 * 
		 **/				
		void normalize();
		
		/**
		 * 
		 * Product of this*qin. Sets result to qRes and returns void.
		 * 
		 **/				
		void product(const Cquaternion & qin, Cquaternion & qRes) const;
		
		/**
		 * 
		 * Product of this*qin. Returns result.
		 * 
		 **/				
		Cquaternion product(const Cquaternion & qin) const;
				
		/**
		 * 
		 * Prints this quaternion
		 * 
		 **/						
		void print() const;
		
		/**
		 * 
		 * Operator (). direct accessing to quaternion component
		 * 
		 **/
		double operator()(unsigned int ii) const;

		/**
		 * 
		 * Assignement operator
		 * 
		 **/
		void operator=(const Cquaternion & qin);		
            
            /**
             * 
             * Product operator. Result is this*qin
             * 
             **/
            Cquaternion operator*(const Cquaternion & qin);                    
};
#endif

