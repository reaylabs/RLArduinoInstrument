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

#define terminator '\n'
#define timeout 10000 //milli-seconds

RLArduinoInstrument instrument(terminator, timeout); 

error_t cmdGetAdcVoltage(index_t index) {
  error_t error = ERROR_NONE;
  float voltage = 2.345;
  // error = getAdcVoltage(&voltage);
  instrument.printResult(error,index, voltage);
  return error;
}

error_t cmdSetDacVoltage(index_t index) {
  float value;
  error_t error = instrument.requestFloat(value, index);
  if (value < 0 || value > 5.0) error = ERROR_INVALID_VALUE;
  if (!error) {
    //error = setDacVoltage(value);
  }
  return error;
}

error_t cmdSetLedRed(index_t index) {
  //setLedRed();
  instrument.printCommandDescription(index);
  return ERROR_NONE;
}

error_t cmdSetSampleRate(index_t index) {
  //Set the sample rate for the AD7791 ADC
  error_t error = ERROR_NONE;
  uint8_t filterReg = 0x04;
  long value;
  bool debug = instrument.getDebug();
  if (debug) {
    instrument.printCommandDescription(index);
    Serial.println("0: 120Hz");
    Serial.println("1: 100Hz");
    Serial.println("2: 33.3Hz");
    Serial.println("3: 20Hz");
    Serial.println("4: 16.6Hz");
    Serial.println("5: 16.7Hz");
    Serial.println("6: 13.3Hz");
    Serial.println("7: 9.5Hz");
  }
  //get the value
  instrument.requestLong(value,index);
  //check limits
  if (value < 0 || value > 7) {
    error = ERROR_INVALID_VALUE;
  }
  if (!error) {
    filterReg &= 0xF8; //clear lower 3 bits
    filterReg |= value;
  }
  if (debug) {
    Serial.print("Filter Register: 0x");
    Serial.println(filterReg, HEX);
  }
  return error;
}

error_t cmdToggleDebug(index_t index) {
  instrument.setDebug(!instrument.getDebug());
  instrument.printCommandDescription(index);
  return ERROR_NONE;
}

void addCommands() {
  instrument.addCommand(0,"Set Dac Voltage", cmdSetDacVoltage);
  instrument.addCommand(1,"Get Adc Voltage", cmdGetAdcVoltage);
  instrument.addCommand(2,"Set LED Red", cmdSetLedRed);
  instrument.addCommand(3,"Set Filter Register", cmdSetSampleRate);
  instrument.addCommand(4,"Toggle Debug", cmdToggleDebug);
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