#ifndef timeStamp_H
#define timeStamp_H

//C, std
#include <time.h>
#include <sys/time.h>
#include <cmath>
#include <iostream>

//number of digits to print time stamp values
const unsigned int TIME_STAMP_DIGITS = 10;

/**
 *
 * \brief CtimeStamp implements basic funcitionalities for time stamps
 * 
 * CtimeStamp implements basic funcitionalities for time stamps
 *
*/
class CtimeStamp
{
	protected:
		/**
		* \brief Time stamp
		*
		* Time. Expressed in seconds from 1st jan 1970.
		*
		*/		
		double timeStamp;

	public:
		/**
		* \brief Constructor
		*
		* Constructor
		*
		*/				
		CtimeStamp();

		/**
		* \brief Destructor
		*
		* Destructor
		*
		*/						
		~CtimeStamp();
		
		/**
		* \brief Time stamp to now
		*
		* Sets time stamp to now
		*
		*/										
		void setToNow();
		
		/**
		* \brief Set time stamp
		*
		* Sets time stamp to a given value passed as a timeval struct
		*
		*/												
		void set(timeval ts);
		
		/**
		* \brief Set time stamp
		*
		* Sets time stamp to a given value passed as a two-integer (seconds and nanoseconds)
		*
		*/												
		void set(const unsigned long int sec, const unsigned long int nanosec);
		
		/**
		* \brief Set time stamp
		*
		* Sets time stamp to a given value passed as a double (seconds)
		*
		*/														
		void set(const double ts);

		/**
		* \brief Get time stamp
		*
		* Returns time stamp
		*
		*/																
		double get() const;
		
		/**
		* \brief Get time stamp (only seconds)
		*
		* Returns seconds of time stamp
		*
		*/																
		unsigned long int getSeconds() const;
		
		/**
		* \brief Get time stamp (only nano seconds)
		*
		* Returns nanoseconds part of time stamp
		*
		*/																
		unsigned long int getNanoSeconds() const;
			
		/**
		 * \brief Prints time stamp to a given ostream 
		 * 
		 */
		void print(std::ostream & ost = std::cout) const; 

            /** \brief Assignement operator
            * 
            * Assignement operator
            * 
            */                
            void operator=(const double ts);
            
            /** \brief Assignement operator
            * 
            * Assignement operator
            * 
            */                
            void operator=(const CtimeStamp ts);
            
            /** \brief comparison operator
            * 
            * Comparison operator
            * 
            */                
            bool operator<(const CtimeStamp ts) const;    
            
            /** \brief comparison operator
            * 
            * Comparison operator
            * 
            */                
            bool operator<=(const CtimeStamp ts) const;                        

            /** \brief difference operator
            * 
            * difference operator
            * 
            */                
            double operator-(const CtimeStamp ts) const;
            
};	
#endif
