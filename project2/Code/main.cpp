#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>
#include<iomanip>
#include<thread>

#include"functions.hpp"
#include"MemoryFunction.h"

using namespace std;
using namespace std::chrono;

const int START = 0;
const int READY = 1;
const int RUNNING = 2;
const int WAITING = 3;
const int EXIT = 4;

struct PCB
{
   int processState;
};

int main(int argc, const char *argv[])
{
   // declare variables, initialize count and set default logtype (Log to Both)
   ifstream fin;
   string metadataFilepath, logFilepath, units;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   int systemMemory, count = 0, logType = 2;
   PCB PCBmain;
   
   high_resolution_clock::time_point t1;
   high_resolution_clock::time_point t2;
   duration<double> time_span;
   
   try
   {
      //clear input filestream and open configuration file
      fin.clear();
      fin.open(argv[1]);
      
      // check for valid configuration file
      checkConfigurationFile(fin, argv);
      
      // process configuration file
      readConfigurationFile(fin, cycleTimes, metadataFilepath, logFilepath, logType, 
                            systemMemory, units);
      
      // close configuration file
      fin.close();
      
      //clear input filestream and open metadata file
      fin.clear();
      fin.open(metadataFilepath);
      
      // check for valid metadata file
      checkMetadataFile(fin, metadataFilepath);
      
      // process configuration file
      readMetadataFile(fin, metadataDescriptors, metadataCodes, metadataCycles, count);
      
      // close metadata file
      fin.close();
          
      t1 = chrono::high_resolution_clock::now();
      for(int i = 0; i < count; i++)
      {
         if(metadataCodes[i] == 'O' || metadataCodes[i] == "I")
         {
            // threading
         }
         logToMonitor(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                      logFilepath, logType, count, systemMemory, i, t1, t2, time_span);
      }
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e);
   }
   
   // otherwise exit program successfully
   return EXIT_SUCCESS;
}