/*
RLArduinoInstrument.cpp

Description
  Instrument class for the RL1400 hardware built on the RLArduinoSerial class. 

Author
  Robert Reay

Revision History
  01-24-2024 : Initial Code
*/


#include "RLArduinoInstrument.h"

//Constructor
RLArduinoInstrument::RLArduinoInstrument(char terminator, int timeout):RLArduinoSerial(terminator,timeout)
{
}

//Add a command to the command array
void RLArduinoInstrument::addCommand(command_t command, const char* description, error_t (*commandHandler)(index_t index))
{
  g_commmandArraySize++;
  g_commandArray = (SerialCommand*)realloc(g_commandArray, g_commmandArraySize * sizeof(SerialCommand));
  g_commandArray[g_commmandArraySize - 1].command=command;
  g_commandArray[g_commmandArraySize - 1].description=description;
  g_commandArray[g_commmandArraySize - 1].commandHandler=commandHandler;
  sortCommands();
}

//execute command
error_t RLArduinoInstrument::executeCommand(command_t command)
{
  error_t error = ERROR_COMMAND_NOT_FOUND;
  for (size_t index = 0; index < g_commmandArraySize; index++) {
    if (g_commandArray[index].command == command) {
     error = g_commandArray[index].commandHandler(index);
     break;
    }
  }
  if(error == ERROR_COMMAND_NOT_FOUND) {
    Serial.println(c_noData);
  }
  return error;
}

//print
template void RLArduinoInstrument::print<unsigned short>(unsigned short);
template void RLArduinoInstrument::print<unsigned int>(unsigned int);
template void RLArduinoInstrument::print<long>(long);
template void RLArduinoInstrument::print<char const*>(char const*);
template void RLArduinoInstrument::print<String>(String);
template <typename T> 
void RLArduinoInstrument::print(T value)
{
  if (g_debug) {
    Serial.print(value);
  }
}
void RLArduinoInstrument::print(float value, int precision)
{
  if (g_debug) {
    Serial.print(value, precision);
  }
}

//println
template void RLArduinoInstrument::println<unsigned short>(unsigned short);
template void RLArduinoInstrument::println<unsigned int>(unsigned int);
template void RLArduinoInstrument::println<long>(long);
template void RLArduinoInstrument::println<char const*>(char const*);
template void RLArduinoInstrument::println<String>(String);
template <typename T> 
void RLArduinoInstrument::println(T value)
{
  if (g_debug) {
    Serial.println(value);
  }
}
void RLArduinoInstrument::println(float value, int precision)
{
  if (g_debug) {
    Serial.println(value, precision);
  }
}

//print commands
void RLArduinoInstrument::printCommands()
{
if (g_debug) {
    Serial.println("\n*** Commands ***");
    for (size_t i = 0; i < g_commmandArraySize; i++) {
      Serial.print(g_commandArray[i].command);
      Serial.print(": ");
      Serial.println(g_commandArray[i].description);
    }
  }
}


//print prompt and value
template void RLArduinoInstrument::printLabelValue<long>(error_t, String label, long);
template void RLArduinoInstrument::printLabelValue<String>(error_t, String label, String);

template <typename T> 
void RLArduinoInstrument::printLabelValue(error_t error, String label,  T value)
{
  if (g_debug) {
    Serial.print(label);
    Serial.print(": ");
  }
  if (error == ERROR_NONE) {
    Serial.println(value);
  }
}
void RLArduinoInstrument::printLabelValue(error_t error, String label, float value, int precision)
{
  if (g_debug) {
    Serial.print(label);
    Serial.print(": ");
  }
  if (error == ERROR_NONE) {
    Serial.println(value, precision);
  }
}

//print result value
template void RLArduinoInstrument::printResult<long>(error_t, index_t, long);
template void RLArduinoInstrument::printResult<byte>(error_t, index_t, byte);
template void RLArduinoInstrument::printResult<String>(error_t, index_t, String);

template <typename T> 
void RLArduinoInstrument::printResult(error_t error, index_t index,  T value)
{
 
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
    Serial.print(": ");
  }
  if (error == ERROR_NONE) {
    Serial.println(value);
  }
}

void RLArduinoInstrument::printResult(error_t error, index_t index, float value, int precision)
{
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
    Serial.print(": ");
  }
  if (error == ERROR_NONE) {
    Serial.println(value, precision);
  }
}

//print no data command
void RLArduinoInstrument::printResult( index_t index)
{
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
  }
  Serial.println(c_noData);
}

//print command description
void RLArduinoInstrument::printCommandDescription(index_t index)
{
  if (g_debug) {
    Serial.println(g_commandArray[index].description);
  }
}
//print error
void RLArduinoInstrument::printError(error_t error)
{
  if (g_debug) {
    switch (error) {
      case ERROR_NONE:
        Serial.println("OK");
        break;
      case ERROR_COMMAND_NOT_FOUND:
        Serial.println("Command Not Found");
        break;
      case ERROR_TIMEOUT:
        Serial.println("Timeout");
        break;
      case ERROR_INVALID_VALUE:
        Serial.println("Invalid Value");
        break;
      default:
        Serial.println("Unknown Error");
        break;  
    }
  } else {
    Serial.println(error);
  }
}

//print menu
void RLArduinoInstrument::printMenu()
{
   if(g_debug)
   {
    printCommands();
    printPrompt();
   }
}

//Print the prompt for a new command
void RLArduinoInstrument::printPrompt()
{
  if (g_debug) {
    Serial.print("Enter Command: ");
  }
}


error_t RLArduinoInstrument::requestFloat(float &value, index_t index, int precision)
{
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
    Serial.print(": ");
  }
  if (waitForFloatWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value, precision);
      }
      return ERROR_NONE;
  } else {
      return ERROR_TIMEOUT;
  }
}


error_t RLArduinoInstrument::requestFloat(float &value, String prompt, int precision)
{
  if (g_debug) {
    Serial.print(prompt);
    Serial.print(": ");
  }
  if (waitForFloatWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value, precision);
        
      }
      return ERROR_NONE;
  } else {
      return ERROR_TIMEOUT;
  }
}


error_t RLArduinoInstrument::requestLong(long &value, index_t index) 
{
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
    Serial.print(": ");
  }
  if (waitForLongWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
      }
      return ERROR_NONE;
  } else {
      return ERROR_TIMEOUT;
  }
}


error_t RLArduinoInstrument::requestLong(long &value, String prompt) 
{
  if (g_debug) {
    Serial.print(prompt);
    Serial.print(": ");
  }
  if (waitForLongWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
        
      }
      return ERROR_NONE;
  } else {
      return ERROR_TIMEOUT;
  }
}


error_t RLArduinoInstrument::requestString(String &value, index_t index)
{
  if (g_debug) {
    Serial.print(g_commandArray[index].description);
    Serial.print(": ");
  }
  if (waitForStringWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
      }
      return ERROR_NONE;
  } else {
    return ERROR_TIMEOUT;
  }
}


error_t RLArduinoInstrument::requestString(String &value, String prompt)
{
  if (g_debug) {
    Serial.print(prompt);
    Serial.print(": ");
  }
  if (waitForStringWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
      }
      return ERROR_NONE;
  } else {
    return ERROR_TIMEOUT;
  }
}

//sort the command array by command
void RLArduinoInstrument::sortCommands()
{
  for (size_t i = 0; i < g_commmandArraySize; i++) {
    for (size_t j = i + 1; j < g_commmandArraySize; j++) {
      if (g_commandArray[i].command > g_commandArray[j].command) {
        SerialCommand temp = g_commandArray[i];
        g_commandArray[i] = g_commandArray[j];
        g_commandArray[j] = temp;
      }
    }
  }
}
