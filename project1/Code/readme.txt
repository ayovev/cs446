main.cpp
--------
Lines 23-60: Implemented an overarching try-catch block to run everything in,
             rather than using multiple smaller blocks
             
functions.hpp
-------------
NONE

functions.cpp
-------------
Line 22: void getMetadataFilepath(ifstream& fin, string& mdfp)
         gets the filepath of the metadata file from the configuration file
         
Line 36: void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes)
         gets the various component cycle times from the configuration file
         
Line 70: void getLogTypeAndFilepath(ifstream& fin, string& lfp, int& lt)
         uses modular functions to get log type and filepath from configuration file
         
Line 78: void getLogType(ifstream& fin, int& lt)
         gets log type (monitor, file, both) from configuration file
         
Line 108: void getLogFilepath(ifstream& fin, string& lfp)
          gets the path that the log file should be output to
          
Line 117: void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                                     string& mdfp, string& lfp, int& lt)
          uses modular functions to read the entire configuration file
          
Line 128: void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                           vector<int>& cycles)
          reads in a single piece of metadata from the metadata file
          
Line 195: void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                                vector<int>& cycles, int& count)
          uses a modular function to read the entire metadata file
          
Line 215: int calculateTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                            vector<int>& mdc, int index)
          calculate the metadata metrics by "mapping" metadata descriptors to their
          corresponding components
          
Line 261: void logToMonitor(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                            vector<int>& mdcy, string logFilepath, int logType, int count)
          logs all data to the monitor in the prescribed example format
          
Line 298: void logToFile(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                         vector<int>& mdcy, string logFilepath, int logType, int count)
          logs all data to the given file in the prescribed example format
          
Line 341: void log(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   vector<int>& mdcy, string logFilepath, int logType, int count)
          checks the logtype variable and uses modular functions to log accordingly
          
Line 365: void checkConfigurationFile(ifstream& fin, const char *argv[])
          checks the configuration file for any potential errors
          
Line 389: void checkMetadataFile(ifstream& fin, string mdfp)
          checks the metadata file for any potential errors
          
Line 414: int handleErrors(int e)
          handles all errors given the error code by displaying a corresponding
          message and terminating the program