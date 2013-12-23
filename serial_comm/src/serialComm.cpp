#include "serialComm.h"

CserialComm::CserialComm()
{

}


CserialComm::~CserialComm()
{

}


int CserialComm::openSerialComm(const std::string & name)
{
      int retValue;
      retValue = open(name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK); 
      if ( retValue < 0 )
      {
            std::cout << "Error opening serial port: " << name << std::endl;
            return SERIAL_ERROR;
      }
      else
      {
            std::cout << "Success opening serial port: " << name << "; Id: " << retValue << std::endl;
            this->portName = name;      
            this->serialCommId = retValue;
            return SERIAL_SUCCESS;
      }
}


int CserialComm::closeSerialComm()
{
      tcsetattr(serialCommId, TCSANOW, &ttySettingsOld); 
      close(serialCommId);
      return SERIAL_SUCCESS;

}


int CserialComm::configure()
{
      int retValue;
      
      //gets current config
      tcgetattr(serialCommId, &ttySettingsOld);
      
      //Set desired options
      //control modes: baud rate, ignores modem control line, enables receiver, character size mask 7, enables parity on input and output
      ttySettings.c_cflag = ( B9600 | CLOCAL | CREAD | CS7 | PARENB); 
      
      //input modes: Ignores break condition, enables parity checking, 
      ttySettings.c_iflag = ( IGNBRK | INPCK ); 
      
      //output modes 
      ttySettings.c_oflag = 0x0;
      
      //local modes
      ttySettings.c_lflag = 0x0;
      
      //Sets configuration immediately.
      retValue = tcsetattr(serialCommId, TCSANOW, &ttySettings); 
      if ( retValue < 0 )
      {
            std::cout << "Error configuring serial communications; Id: " << serialCommId << std::endl << std::endl;
            return -1;
      }
      else
      {
            std::cout << "Success configuring serial communications" << std::endl << std::endl;
      }

}


char CserialComm::read()
{

}

 
int CserialComm::read(const unsigned int nBytes, std::vector<char> & data)
{

}


int CserialComm::write(const char byte)
{

}


int CserialComm::write(const std::vector<char> & data)
{

}


void CserialComm::printRaw() const
{

}

