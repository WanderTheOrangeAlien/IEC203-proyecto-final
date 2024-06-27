#include "serial_util.h"
#include <HardwareSerial.h>
#include <string.h>
#include <stdio.h>

#define SERIAL_UTIL_DEBUG

SerialUtil::SerialUtil(){

    debug = false;
    usingBuiltInSerial = true;
    includeEndChar = false;
    endChar = NEW_LINE;

}

SerialUtil::SerialUtil(bool _debug){
    debug = _debug;
    usingBuiltInSerial = true;
    includeEndChar = false;
    endChar = NEW_LINE;
}
/// @brief 
/// @param SerialPort HardwareSerial class instance from which you are going to read/write. Leave as NULL if you want to use the built-in serial 
/// @param _endChar End character
/// @param _includeEndChar Wheter or not to include the end character in the buffer
/// @param _debug Debug flag
SerialUtil::SerialUtil(HardwareSerial* _serialPort,EndCharacter _endChar, bool _includeEndChar, bool _debug){

    usingBuiltInSerial = false;
    includeEndChar = _includeEndChar;
    debug = _debug;
    serialPort = _serialPort;

    switch (_endChar)
    {
    case NEW_LINE:
        endChar = '\n';
        break;

    case CARRIAGE_RETURN:
        endChar = '\r'; 
        break;
    
    case NL_AND_CR:
        endChar = '\n';
        break;
    
    default:
        endChar = '\n';
        break;
    }
}

/// @brief 
/// @param _endChar 
/// @param _includeEndChar 
/// @param _debug 
SerialUtil::SerialUtil(EndCharacter _endChar, bool _includeEndChar, bool _debug){

    usingBuiltInSerial = true;
    includeEndChar = _includeEndChar;
    debug = _debug;

    switch (_endChar)
    {
    case NEW_LINE:
        endChar = '\n';
        break;

    case CARRIAGE_RETURN:
        endChar = '\r'; 
        break;
    
    case NL_AND_CR:
        endChar = '\n';
        break;
    
    default:
        endChar = '\n';
        break;
    }
}



/// @brief Listens for incoming data from the serial port, stores the message and performs a callback 
/// @param buffer Char array to store the incoming datas
/// @param length The length of such array
/// @param callback The callback function to perform
void SerialUtil::getSerialInput(char* buffer, unsigned int length, void (*callback)()){

    /*
    This function gets called several times between consecutive incoming bytes
    the index must be static, so it wont be initialize between incoming bytes
    */

    static int index = 0;
    char c = '\0';

    while((usingBuiltInSerial ? Serial.available() : serialPort->available() ) > 0){

        //If the previous message ended (index == 0) and the buffer is not cleared
        if(index == 0 && buffer[0] != '\0'){
            if(debug) Serial.println("Performing memset");
            memset(buffer, 0, length);
        }

        if(usingBuiltInSerial){
            c = (char)Serial.read();
        }else{
            c = (char)serialPort->read();
        }
        

        if(c != endChar){
            buffer[index] = c;
            
            if(debug){
                Serial.print("["); Serial.print(index); Serial.print("]: "); 
                Serial.print(buffer[index]); Serial.print("   "); Serial.println(buffer[index],HEX);
            }
            
            index++;             
        }else{
            
            if(debug){
                Serial.print("["); Serial.print(index); Serial.print("]: "); 
                Serial.print(buffer[index]); Serial.print("   "); Serial.println(buffer[index],HEX);

            }
      
            if(includeEndChar){
                buffer[index] = endChar;
            }else{
                buffer[index] = '\0';
            }

            if(debug){
                int bytesLeft = usingBuiltInSerial ? Serial.available() : serialPort->available();


                if(bytesLeft > 0){
                    Serial.print("There is still "); Serial.print(bytesLeft); Serial.print(" bytes left: "); 
                    
                    c = usingBuiltInSerial ? Serial.read() : serialPort->read();

                    Serial.print(c); Serial.print("  "); Serial.println(c,HEX);
                }
            }
            
            callback();
            
            index = 0;
            return;
        }

             

    }
}