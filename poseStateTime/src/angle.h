#ifndef angle_h
#define angle_h

#include <math.h>
#include <iostream>

static const bool inRADIANS = 0;
static const bool inDEGREES = 1;

/**
 * \brief Angle class
 * 
 * Angle class holds and manipulates angles.
 * The angle value is always assumed to be stored in memory in radians, in the interval (-pi, pi], and default set/get operations are also done in radians.
 * However, function members generally allow interaction through angle values in degrees by setting a boolean parameter to "inDEGREES".
 *
*/
class Cangle
{
	protected:
		/**
		 * \brief Angle value
		 * 
		 * Angle is always stored in radians in the interval (-pi,pi].
		 * 
		*/
		double angle; 

	public:
		/**
		 * \brief Default constructor
		 * 
		 * Default constructor. Angle value is set to zero.
		 * 
		*/				
		Cangle();

		/**
		 * \brief Constructor with argument
		 * 
		 * Constructor with argument. Angle value is set to value. 
		 * By default value is provided in radians. If it is provided in degrees, pass rd boolean parameter  to "inDEGREES".
		 * 
		*/				
		Cangle(const double value, const bool rd=inRADIANS);

		/**
		 * \brief Destructor
		 * 
		 * Destructor.
		 * 
		*/		
		~Cangle();
		
		/**
		 * \brief Set angle value
		 * 
		 * Sets angle value. By default value is provided in radians. If it is provided in degrees, pass rd boolean parameter to "inDEGREES".
		 * After assignation, it calls mPi2Pi().
		 * 
		*/						
		void setAngle(const double value, const bool rd=inRADIANS);

		/**
		 * \brief Get angle value
		 * 
		 * Gets angle value. By default value is returned in radians. If degrees are preferred, indicate it passing rd boolean parameter to "inDEGREES".
		 * 
		*/						
		double getAngle(const bool rd=inRADIANS) const; 
		
		/**
		 * \brief Sets weighted mean 
		 * 
		 * Sets this angle as the weighted mean of angles a1 and a2, with respective weights w1 and w2. By default angles are provided in radians. If degrees are preferred, indicate it passing rd boolean parameter to "inDEGREES".
		 * 
		*/						
		void wMeanAngle(const double a1, const double w1, const double a2, const double w2, const bool rd=inRADIANS);

		/**
		 * \brief Equality operator
		 * 
		 * Returns true if this and beta are equal.
		 * 
		*/						
		bool operator==(const Cangle beta); 

            /**
             * \brief Assignement operator
             * 
             * Same as set function, but always in radians. 
             * 
            */                                  
            void operator=(const double beta);
            
		/**
		 * \brief Assignement operator
		 * 
		 * Same as set function, but always in radians. 
		 * 
		*/						
		void operator=(const Cangle beta);
		
		/**
		 * \brief Increment operator
		 * 
		 * Increments this angle by beta. 
		 * 
		*/						
		void operator+=(const Cangle beta);

		/**
		 * \brief Decrement operator
		 * 
		 * Decrements this angle by beta. Values in degrees are not supported here!
		 * Result in (-pi, pi].
		 * 
		*/						
		void operator-=(const Cangle beta);

		/**
		 * \brief Sum operator
		 * 
		 * Sums this and beta and returns a new angle. ret = this + beta
		 * Return value in (-pi, pi].
		 * 
		*/						
		Cangle operator+(const Cangle beta) const;
		
		/**
		 * \brief Substraction operator
		 * 
		 * Substracts beta to this and returns a new angle. ret = this - beta
		 * Return value in (-pi, pi].
		 * 
		*/						
		Cangle operator-(const Cangle beta) const;

	protected:
		/**
		 * \brief Fits angle in (-pi,pi]
		 * 
		 * Fits angle in the (-pi,pi] interval.
		 * 
		*/						
		void mPi2Pi(); 
};
#endif
