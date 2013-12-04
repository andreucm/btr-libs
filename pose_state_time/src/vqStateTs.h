#ifndef vqStateTs_H
#define vqStateTs_H

//pose_state_yime lib
#include "timeStamp.h"
#include "vqState.h"

/** \brief Time stamped vqState
* 
* Time stamped vqState
* 
**/
class CvqStateTs : public CvqState
{
      public: 
            /** \brief Time stamp
            * 
            * Time stamp
            * 
            **/
            CtimeStamp ts;
      
      public: 
            /** \brief Default constructor
            * 
            * Default constructor
            * 
            **/
            CvqStateTs();
            
            /** \brief Constructor with object arguments
            * 
            * Constructor with object arguments
            * 
            **/
            CvqStateTs(const dlib::matrix<double,3,1> & vel, const Cquaternion & quat, const CtimeStamp & time);
            
            /** \brief Constructor with raw arguments
            * 
            * constructor with raw arguments
            * 
            **/            
            CvqStateTs(const double vx, const double vy, const double vz, const double q1, const double q2, const double q3, const double time);
            
            /** \brief Destructor
            * 
            * Destructor.
            * 
            **/            
            virtual ~CvqStateTs();
            
            /** \brief Assignement operator
            * 
            * Assignement operator from a CvqStateTs object
            * 
            **/
            void operator=(const CvqStateTs & vqt);

            /** \brief Print
            * 
            * Prints to stdout
            * 
            * 
            **/
            virtual void print() const;
};
#endif
