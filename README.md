# RLArduinoInstrument
The RLArduinoInstrument class extends the RLArduinoSerial class to provide automatic user interface menu
functions. When debug is true, the user prompts are sent via the serial port so the instrument can be
debugged view a serial monitor. When debug is false no prompts are written so the instrument can be
driven programmatically.

## Class RLArduinoInstrument
```C++
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
    void printLabelValue(error_t error, String label,  T value);
    void printLabelValue(error_t error, String label,  float value, int precision = 3);
    void printString(String value);
    void printlnString(String value);
    template <typename T> 
    void printResult(error_t error, index_t index,  T value);
    void printResult(error_t error, index_t index,  float value, int precision = 3);
    void printResult(index_t index);
    error_t requestFloat(float &value, String prompt,int precision = 3);
    error_t requestFloat(float &value, index_t index,int precision = 3);
    error_t requestLong(long &value, index_t index);
    error_t requestLong(long &value, String prompt);
    error_t requestString(String &value, index_t index);
    error_t requestString(String &value,String prompt);
    void setDebug(bool debug) {g_debug = debug;}
  private:
    //global variables
    SerialCommand* g_commandArray = nullptr;
    size_t g_commmandArraySize = 0;
    bool g_debug = true;
    //functions
    void sortCommands();
};
```