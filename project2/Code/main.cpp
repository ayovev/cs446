#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

#include"functions.hpp"

using namespace std;

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
      
      // log results
      log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
          logFilepath, logType, count);
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e, cycleTimes, metadataDescriptors, metadataCodes,
                          metadataCycles, logFilepath, logType, count);
   }
   // otherwise exit program successfully
   return EXIT_SUCCESS;
}