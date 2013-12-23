#ifndef serialComm_H
#define serialComm_H

//includes
#include <iostream>
#include <string>
#include <vector>
#include <termios.h>
#include <sys/types.h>
// #include <sys/time.h>
// #include <sys/stat.h>
#include <fcntl.h>
// #include <stdlib.h>
// #include <unistd.h>


//constants
const int SERIAL_ERROR = -1;
const int SERIAL_SUCCESS = 1;


/** \brief Class for serial communications read/write
 * 
 * Class for serial communications read/write
 * 
 **/
class CserialComm
{
      protected:
            termios ttySettings; //termios variable to configure serial port
            termios ttySettingsOld; //termios variable to keep old serial port configs 
            fd_set fdSet;//file descriptors set to be used by blocking select()
            std::string portName;//file name of the serial port
            int serialCommId; //file id for serial comm's

      public:
            CserialComm();
            ~CserialComm();
            int openSerialComm(const std::string & name);
            int closeSerialComm();
            int configure();
            char read(); 
            int read(const unsigned int nBytes, std::vector<char> & data);
            int write(const char byte);
            int write(const std::vector<char> & data);
            void printRaw() const;
};
#endif
