Below are the functions that play the biggest part in making the program run, the full
description for all functions can be found with comments in the respective file

main.cpp
--------
Lines 46-97: Implemented an overarching try-catch block to run everything in,
             rather than using multiple smaller blocks
             
functions.hpp
-------------
NONE

functions.cpp
-------------

Line 36: int calculateCycleTime(map<string, int>& cycleTimes, vector<string>& mdd,
                       		vector<int>& mdc, const int index)
	 calculate the metadata metrics by "mapping" metadata descriptors to their
	 corresponding components

Line 82: double calculateSleepTime(map<string, int>& cycleTimes, vector<string>& mdd,
                    		   vector<int>& mdc, const int index)
	 calculate the value for sleep / wait function

Line 180: void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes)
          gets the various component cycle times from the configuration file
          
Line 215: int calculateTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                            vector<int>& mdc, int index)
          calculate the metadata metrics by "mapping" metadata descriptors to their
          corresponding components
          
Line 356: int handleErrors(int e)
          handles all errors given the error code by displaying a corresponding
          message and terminates the program after logging

Line 412: void printTime(high_resolution_clock::time_point t1,
               	  	 high_resolution_clock::time_point t2,
               		 duration<double> time_span, const int lt, ofstream& fout)
	  output duration between two points for timestamp

Line 432: void processAndLog(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   	     vector<int>& mdcy, const string lfp, const int lt,
                   	     const int count, const int sm, const int i,
                   	     high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
                   	     duration<double> time_span, ofstream& fout, PCB PCBmain,
                   	     const int mbs, int& mult, sem_t semaphore, const int hdq, const int pq,
                   	     int& hdc, int& pc)
	  logs all data to the appropriate location in the prescribed format,
	  changes PCB process state, and deals with multithreading and semaphore locks

Line 810: void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           	     string& mdfp, string& lfp, int& lt, int& sm, string& units,
                           	     int& mbs, int& hdq, int& pq)
          uses modular functions to read the entire configuration file

Line 897: void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                                vector<int>& cycles, int& count)
          uses a modular function to read the entire metadata file
