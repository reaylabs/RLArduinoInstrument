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
```