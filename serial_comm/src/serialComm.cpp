#include "serialComm.h"

CserialComm::CserialComm()
{
      //keep old tty settings
      tcgetattr(serialCommId, &ttySettingsOld);
}

CserialComm::~CserialComm()
{

}

int CserialComm::openSerial(const std::string & name)
{
      int retValue;
      
      //OPEN
      retValue = open(name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK ); 
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
            //return SERIAL_SUCCESS;
      }
      
      //BASIC CONFIGURATION
      sleep(1);
      retValue = this->basicConfigure();
      if ( retValue == SERIAL_ERROR )
      {
            std::cout << "Error in basic configuration of serial port: " << name << std::endl;
            return SERIAL_ERROR;
      }
      else
      {
            std::cout << "Success on basic configuration of serial port: " << name << std::endl;
            //return SERIAL_SUCCESS;
      }      
      
      //FLUSH INPUT
      sleep(1);
      retValue = tcflush(serialCommId, TCIFLUSH);
      if ( retValue != 0 )
      {
            std::cout << "Error flushing input from serial port: " << name << std::endl;
            return SERIAL_ERROR;
      }
      else
      {
            std::cout << "Success on flushing input from serial port: " << name << std::endl;
            sleep(1);
            return SERIAL_SUCCESS;
      }      
}

int CserialComm::closeSerial()
{
      //set old tty settings and close
      tcsetattr(serialCommId, TCSANOW, &ttySettingsOld); 
      close(serialCommId);
      return SERIAL_SUCCESS;
}

int CserialComm::readSerial(unsigned char & inChar)
{
      std::vector<unsigned char> ch;
      if ( readSerial(1,ch,DEFAULT_TIMEOUT) == SERIAL_SUCCESS )
      {
            inChar = ch.at(0);
            return SERIAL_SUCCESS;
      }
      else 
      {
            return SERIAL_ERROR;
      }
}
 
int CserialComm::readSerial(const unsigned int nBytes, std::vector<unsigned char> & data, const double waitTime)
{
      bool loopError = false; 
      unsigned int nn = 0;//bytes already read
      unsigned char inChar;
      int selectResult;
      struct timeval timeout1, timeout2;
      fd_set fdSet;

      //clears data array
      data.clear();
      
      //sets timeout
      timeout1.tv_sec = floor(waitTime);
      timeout1.tv_usec = (unsigned int)( (waitTime - floor(waitTime))*1e6 );

      //reading loop
      while ( (nn < nBytes) && (!loopError) )
      {
            //reset fdSet for serial reception
            FD_ZERO(&fdSet);
            FD_SET(serialCommId, &fdSet);
            
            //sets timeout
            timeout2.tv_sec = timeout1.tv_sec;
            timeout2.tv_usec = timeout1.tv_sec;
                              
            //blocking read up to an input arrives to some channel indicated by fdset
            selectResult = select(serialCommId+1, &fdSet, NULL, NULL, &timeout2);

            //select returns 0 if timeout, 1 if input available, -1 if error.
            switch (selectResult)
            {
                  case 0: 
                        loopError = true;  
                        std::cout << "CserialComm: Time Out Comm Error on serial Id=" << serialCommId << std::endl;
                        break;
                  case 1:
                        if( FD_ISSET(serialCommId,&fdSet) ) //check if input was at serialCommId
                        {
                              if ( read(serialCommId, &inChar, 1) == 1)
                              {
                                    data.push_back(inChar);
                                    nn++;
                                    //std::cout << "nn: " << nn << "; inChar: " << inChar << std::endl;
                              }
                              else
                              {
                                    loopError = true;  
                                    std::cout << "CserialComm: read() error on fd=" << serialCommId << std::endl;
                              }
                        }
                        break;
                  default: //-1 and others
                        loopError = true;  
                        std::cout << "CserialComm: read() error on fd=" << serialCommId << std::endl;
                        break;
            }
      }
      
      //return value 
      if (loopError) return SERIAL_ERROR;
      else return SERIAL_SUCCESS;
}

int CserialComm::writeSerial(const unsigned char byte)
{
      unsigned int retValue;
      
      retValue = write(serialCommId, &byte, 1);
      return SERIAL_SUCCESS;
}

int CserialComm::writeSerial(const std::vector<unsigned char> & data)
{
      unsigned int vSize, ii;
      
      vSize = data.size();
      for (ii=0; ii<vSize; ii++)
      {
            this->writeSerial( data.at(ii) );
      }
      return SERIAL_SUCCESS;
}

void CserialComm::printRaw() const
{

}

void CserialComm::printConfig() const
{
      std::cout << "ttySettings.c_cflag: " << ttySettingsOld.c_cflag << std::endl;
      std::cout << "ttySettings.c_iflag: " << ttySettingsOld.c_iflag << std::endl;
      std::cout << "ttySettings.c_oflag: " << ttySettingsOld.c_oflag << std::endl;
      std::cout << "ttySettings.c_oflag: " << ttySettingsOld.c_lflag << std::endl;
}

//http://en.wikibooks.org/wiki/Serial_Programming/termios
//http://ulisse.elettra.trieste.it/services/doc/serial/config.html
//see http://www.mkssoftware.com/docs/man5/struct_termios.5.asp
int CserialComm::basicConfigure()
{
      int retValue;
      termios ttySettings;
      
      //gets current config
      tcgetattr(serialCommId, &ttySettings);
      
      //Set desired options
      
      //baud rates
//       cfsetispeed(&ttySettings, B9600);
//       cfsetospeed(&ttySettings, B9600);
      
      //control modes: baud rate, ignores modem control line, enables receiver, character size mask 7, enables parity on input and output
      ttySettings.c_cflag = ( B9600 | CLOCAL | CREAD | CS8);// | PARENB);
//       ttySettings.c_cflag &= ~CSIZE; //Mask the character size bits
//       ttySettings.c_cflag |= ( B9600 | CLOCAL | CREAD | CS8);// | PARENB); 
      
      //8N1
//       ttySettings.c_cflag &= ~PARENB;
//       ttySettings.c_cflag &= ~CSTOPB;
//       ttySettings.c_cflag &= ~CSIZE;
//       ttySettings.c_cflag |= CS8;
      
      //input modes: Ignores break condition, enables parity checking, 
      ttySettings.c_iflag = 0x0;//( IGNBRK );//| INPCK ); 
//       ttySettings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
      
      //output modes 
      ttySettings.c_oflag = 0x0;
      
      //local modes
      ttySettings.c_lflag = 0x0;
//       ttySettings.c_lflag &= ~(ICANON | ECHO | ISIG);
//       ttySettings.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
      
      //        
      ttySettings.c_cc[VMIN]=1;
      ttySettings.c_cc[VTIME]=100;
      
      //Sets configuration immediately.
      retValue = tcsetattr(serialCommId, TCSANOW, &ttySettings); 
      if ( retValue < 0 )
      {
            //std::cout << "Error configuring serial communications; Id: " << serialCommId << std::endl << std::endl;
            return SERIAL_ERROR;
      }
      else
      {
            //std::cout << "Success configuring serial communications" << std::endl << std::endl;
            return SERIAL_SUCCESS;
      }

}
