#ifndef serial_util_h
#define serial_util_h

#include "Arduino.h"
#include <HardwareSerial.h>


typedef enum EndCharacter{
    NEW_LINE,
    CARRIAGE_RETURN,
    NL_AND_CR
} EndCharacter;

class SerialUtil{

    public:
        SerialUtil();
        SerialUtil(bool);
        SerialUtil(HardwareSerial*,EndCharacter,bool,bool);
        SerialUtil(EndCharacter,bool,bool);
        void getSerialInput(char* buffer, unsigned int length, void (*callback)());

    private:
        bool debug;
        bool includeEndChar;
        char endChar; 
        HardwareSerial* serialPort;
        bool usingBuiltInSerial;


};

#endif