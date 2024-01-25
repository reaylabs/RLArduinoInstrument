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
void RLArduinoInstrument::addCommand(command_t command, const char* description, error_t (*commandHandler)(command_t command))
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
  for (size_t i = 0; i < g_commmandArraySize; i++) {
    if (g_commandArray[i].command == command) {
     error = g_commandArray[i].commandHandler(command);
     break;
    }
  }
  return error;
}

//print commands
void RLArduinoInstrument::printCommands()
{
  for (size_t i = 0; i < g_commmandArraySize; i++) {
    Serial.print(g_commandArray[i].command);
    Serial.print(" ");
    Serial.println(g_commandArray[i].description);
  }
}

//print error
void RLArduinoInstrument::printError(error_t error)
{
  if (error == ERROR_NULL) return;
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
      default:
        Serial.println("Unknown Error");
        break;  
    }
  } else {
    Serial.println("E"+ String(error));
  }
}

//print menu
void RLArduinoInstrument::printMenu()
{
  if (g_debug) {
    Serial.println("\n*** Commands ***");
    for (int i = 0; i < g_commmandArraySize; i++)
    {
      Serial.print(g_commandArray[i].command);
      Serial.print(": ");
      Serial.println(g_commandArray[i].description);
    }
    Serial.print("Enter Command: ");
  }
}

error_t RLArduinoInstrument::requestFloat(float &value)
{
  if (g_debug) {
    Serial.print("Value: ");
  }
  if (waitForFloatWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
        return ERROR_NULL;
      } else {
        return ERROR_NONE;
      }
  } else {
      return ERROR_TIMEOUT;
  }
}

error_t RLArduinoInstrument::requestLong(long &value)
{
  if (g_debug) {
    Serial.print("Value: ");
  }
  if (waitForLongWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
        return ERROR_NULL;
      } else {
        return ERROR_NONE;
      }
  } else {
      return ERROR_TIMEOUT;
  }
}

error_t RLArduinoInstrument::requestString(String &value)
{
  if (g_debug) {
    Serial.print("Value: ");
  }
  if (waitForStringWithTimeout(&value, NULL)) {
      if(g_debug) {
        Serial.println(value);
        return ERROR_NULL;
      } else {
        return ERROR_NONE;
      }
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
