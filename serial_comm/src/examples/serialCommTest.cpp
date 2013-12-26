#include "serialComm.h"

const std::string portName = "/dev/ttyACM0";

//main loop 
int main(int argc, char *argv[])
{            
      //loop management
      int retVal;
      unsigned int ii;
      
      //serial comm
      std::vector<unsigned char> dataReceived;
      CserialComm mySerial;      
      
      //variables to tackle user input
      unsigned char userChar = ' '; //user input
      termios stdInOldSettings, stdInNewSettings;                
      fd_set fdSet;
      
      //open and configure serial comms
      retVal = mySerial.openSerial(portName);
      
      //configure byte parity on serial comms
      retVal = mySerial.configureParity(BYTE_8N1);
      
      //configure baud rate
      retVal = mySerial.configureBaudRate(BAUD_9600);
      
      //configure user input. Important! ( please see http://www.cplusplus.com/forum/general/5304/ )
      tcgetattr( STDIN_FILENO, &stdInOldSettings );
      stdInNewSettings = stdInOldSettings;
      stdInNewSettings.c_lflag &= (~ICANON & ~ECHO);
      tcsetattr( STDIN_FILENO, TCSANOW, &stdInNewSettings );
      sleep(1); //relax to allow devices to be ready
      
      while (userChar != 'e')
      {
            //prompts new command
            std::cout << std::endl << "New command: " << std::endl;

            //configures fdSet for keyboard entry
            FD_ZERO (&fdSet);
            FD_SET (STDIN_FILENO, &fdSet);

            //waits for an input from keyboard
            retVal = select (STDIN_FILENO+1, &fdSet,NULL,NULL,NULL);

            //reads user byte
            retVal = read(STDIN_FILENO,&userChar,1);
            
            //sends command to device
            retVal = mySerial.writeSerial(userChar);
            
            //read response
            retVal = mySerial.readSerial(18,dataReceived,3);
      
            //print response
            for (ii=0; ii<dataReceived.size(); ii++) std::cout << dataReceived.at(ii); 
      }

      //close serial
      mySerial.closeSerial();
      
      //resets default settings to stdin
      tcsetattr( STDIN_FILENO, TCSANOW, &stdInOldSettings );
      
      //return
      return SERIAL_SUCCESS;
}
