/*
RLArduinoInstrument.h

Description
  Instrument class for the RL1400 hardware built on the RLArduinoSerial class. 

Author
  Robert Reay

Revision History
  01-24-2024 : Initial Code
*/

#ifndef _RL_ARDUINO_INSTRUMENT_H_
#define _RL_ARDUINO_INSTRUMENT_H_

#include "Arduino.h"
#include "RLArduinoSerial.h"
#include <SPI.h>

//Error definitions
#define ERROR_NONE 0
#define ERROR_COMMAND_NOT_FOUND 1
#define ERROR_TIMEOUT 2
#define ERROR_INVALID_VALUE 3

//Menu type definitions
typedef uint8_t error_t;
typedef uint16_t command_t;
typedef uint16_t index_t;

//Command structure
struct SerialCommand {
  command_t command;
  const char* description;
  error_t (*commandHandler)(index_t index);
};

class RLArduinoInstrument:public RLArduinoSerial {
  public:
    //funcions
    explicit RLArduinoInstrument(char terminator, int timeout = 1000);
    void addCommand(command_t command, const char* description, error_t (*commandHandler)(index_t index));
    error_t executeCommand(command_t command);
    bool getDebug() {return g_debug;}
    void printCommands();
    void printCommandDescription(index_t index);
    void printError(error_t error);
    void printMenu();
    void printPrompt();
    template <typename T> 
    void printResult(error_t error, index_t index,  T value);
    error_t requestFloat(float &value, index_t index);
    error_t requestLong(long &value, index_t index);
    error_t requestString(String &value, index_t index);
    void setDebug(bool debug) {g_debug = debug;}
  private:
    //global variables
    SerialCommand* g_commandArray = nullptr;
    size_t g_commmandArraySize = 0;
    bool g_debug = true;
    //functions
    void sortCommands();
};


#endif // _RL_ARDUINO_INSTRUMENT_H_