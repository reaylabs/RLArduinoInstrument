# RLArduinoInstrument
The RLArduinoInstrument class extends the RLArduinoSerial class to provide automatic user interface menu
functions. When debug is true, the menu command menu is drawn via the serial port, otherwise only the
response to commands is sent.

## Class RLArduinoSerial
```C++
explicit RLArduinoInstrument(char terminator, int timeout = 1000);
void addCommand(command_t command, const char* description, error_t (*commandHandler)(command_t command));
error_t executeCommand(command_t command);
bool getDebug() {return g_debug;}
void printCommands();
void printMenu();
void printError(error_t error);
error_t requestFloat(float &value);
error_t requestLong(long &value);
error_t requestString(String &value);
void setDebug(bool debug) {g_debug = debug;}
```

## Simple Menu Example
```C++
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
