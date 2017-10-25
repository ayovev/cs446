#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>
#include<iomanip>
#include<thread>
#include<pthread.h>
#include<semaphore.h>

#include"functions.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, const char *argv[])
{
   // declare variables, initialize count and set default logtype (Log to Both)
   ifstream fin;
   ofstream fout;
   string metadataFilepath, logFilepath, units;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   int systemMemory, memoryBlockSize, count = 0, logType = 2;
   int hardDriveQuantity, printerQuantity, multiplier = 0;
   int hardDriveCounter = 0, printerCounter = 0;

   // struct object
   PCB PCBmain;

   // semaphore object
   sem_t semaphore;

   // initialize semaphore
   sem_init(&semaphore, 0, 1);

   // timing variables
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
                            systemMemory, units, memoryBlockSize, hardDriveQuantity,
                            printerQuantity);

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

      if(logType == 1 || logType == 2)
      {
         // clear and open output filestream
         fout.clear();
         fout.open(logFilepath);
      }

      // start time point
      t1 = chrono::high_resolution_clock::now();

      // loop through metadata pieces
      for(int i = 0; i < count; i++)
      {
         processAndLog(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                       logFilepath, logType, count, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                       memoryBlockSize, multiplier, semaphore, hardDriveQuantity, printerQuantity,
                       hardDriveCounter, printerCounter);
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
