/*
RLArduinoInstumentDemo.ino

Description
  This example shows how to use the RLArduinoInstrument class 

Author
  Robert Reay

Revision History
  01-24-2024 : Initial Code
*/

#include "RLArduinoInstrument.h"

RLArduinoInstrument instrument('\n', 10000); 

error_t cmdGetFloat(command_t command) {
  float value;
  error_t error = instrument.requestFloat(value);
  value = value + 10;
  if (instrument.getDebug()) {
    Serial.print("New Value: ");
  }
  Serial.println(value);
  return error;
}

error_t cmdGetLong(command_t command) {
  long value;
  error_t error = instrument.requestLong(value);
  value = value + 10;
  if (instrument.getDebug()) {
    Serial.print("New Value: ");
  }
  Serial.println(value);
  return error;
}

error_t cmdGetString(command_t command) {
  String value;
  error_t error = instrument.requestString(value);
  value = value + "_TEST";
  if (instrument.getDebug()) {
    Serial.print("New Value: ");
  }
  Serial.println(value);
  return error;
}

error_t cmdToggleDebug(command_t command) {
  instrument.setDebug(!instrument.getDebug());
  return ERROR_NONE;
}

void addCommands() {
  instrument.addCommand(0,"Get Float Value", cmdGetFloat);
  instrument.addCommand(1,"Get Long Value", cmdGetLong);
  instrument.addCommand(2,"Get String Value", cmdGetString);
  instrument.addCommand(3,"Toggle Debug", cmdToggleDebug);
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  addCommands();
  instrument.setDebug(true);
  instrument.printMenu();
}

void loop() {
  command_t cmd;
  error_t error;
  bool debug = instrument.getDebug();
  if (instrument.longAvailable(true)) 
  {
    cmd = instrument.getLong();
    if (debug){
      Serial.println(cmd);
    }
    error = instrument.executeCommand(cmd);
    instrument.printError(error);
    instrument.printMenu();
  }
}