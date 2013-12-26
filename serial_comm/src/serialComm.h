#ifndef serialComm_H
#define serialComm_H

//includes
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
// #include <sys/stat.h>
#include <fcntl.h>
// #include <stdlib.h>
#include <unistd.h>


//constants
const int SERIAL_ERROR = -1;
const int SERIAL_SUCCESS = 1;
const double DEFAULT_TIMEOUT = 3.;


/** \brief Class for serial communications read/write
 * 
 * Class for serial communications read/write
 * 
 **/
class CserialComm
{
      protected:
            termios ttySettingsOld; //termios variable to keep old serial port configs 
            std::string portName;//file name of the serial port
            int serialCommId; //file id for serial comm's
      
      public:
            CserialComm();
            ~CserialComm();
            int openSerial(const std::string & name);
            int closeSerial();
            int readSerial(unsigned char & inChar); 
            int readSerial(const unsigned int nBytes, std::vector<unsigned char> & data, const double waitTime = DEFAULT_TIMEOUT);
            int writeSerial(const unsigned char byte);
            int writeSerial(const std::vector<unsigned char> & data);
            void printRaw() const;
            void printConfig() const;

      protected:
            int basicConfigure();
            
};
#endif
