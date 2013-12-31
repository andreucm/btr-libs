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
const double DEFAULT_TIMEOUT = 3.; //seconds
enum byteParity {BYTE_8N1 = 1, BYTE_7E1, BYTE_7O1}; 
enum baudRate {BAUD_0 = 0, BAUD_50, BAUD_1200, BAUD_4800, BAUD_9600, BAUD_19200, BAUD_38400, BAUD_115200}; 


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
            int configureParity(const byteParity bp);
            int configureBaudRate(const baudRate br);
            int closeSerial();
            int readSerial(unsigned char & inChar); 
            int readSerial(std::vector<unsigned char> & data, const unsigned int nBytes, const double waitTime = DEFAULT_TIMEOUT, const bool untilEndByte = false, const unsigned char endByte = 0x0);
            int writeSerial(const unsigned char byte);
            int writeSerial(const std::vector<unsigned char> & data);
            void printRaw() const;
            void printOldConfig() const;

      protected:
            int basicConfigure();
            
};
#endif
