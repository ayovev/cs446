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
   string metadataFilepath, logFilepath, schedulingCode;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   
   vector<char> newMetadataCodes;
   vector<string> newMetadataDescriptors;
   vector<int> newMetadataCycles;
   
   int systemMemory, memoryBlockSize, count = 0, logType = 2;
   int speakerQuantity, printerQuantity, multiplier = 0;
   int speakerCounter = 0, printerCounter = 0;
   int processorQuantumNumber;
   int processNumber = 0;

   // struct object
   PCB PCBmain;

   // instantiate and initialize semaphore object
   sem_t semaphore;
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
                            systemMemory, memoryBlockSize, printerQuantity,
                            speakerQuantity, processorQuantumNumber, schedulingCode);

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
      
      if(schedulingCode == "FIFO")
      {
         // start time point
         t1 = chrono::high_resolution_clock::now();
         
         // keep track of the process number
         for(int i = 0; i < count; i++)
         {
            if(metadataCodes[i] == 'A' && metadataDescriptors[i] == "start" && metadataCycles[i] == 0)
            {
               processNumber++;
            }
            // process and log metadata vectors
            processAndLog(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                          logType, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                          memoryBlockSize, multiplier, semaphore, speakerQuantity, printerQuantity,
                          speakerCounter, printerCounter, processNumber);
         }
      }
      else if(schedulingCode == "SJF")
      {
         // setup new metadata vectors with SJF order
         shortestJobFirst(metadataDescriptors, metadataCodes, metadataCycles,
                          newMetadataDescriptors, newMetadataCodes, newMetadataCycles);
         
         // start time point
         t1 = chrono::high_resolution_clock::now();
         
         // keep track of the process number
         for(int i = 0; i < count; i++)
         {
            if(newMetadataCodes[i] == 'A' && newMetadataDescriptors[i] == "start" && newMetadataCycles[i] == 0)
            {
               processNumber++;
            }
            // process and log with new metadata vectors
            processAndLog(cycleTimes, newMetadataDescriptors, newMetadataCodes, newMetadataCycles,
                          logType, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                          memoryBlockSize, multiplier, semaphore, speakerQuantity, printerQuantity,
                          speakerCounter, printerCounter, processNumber);
         }
      }
      else if(schedulingCode == "PS")
      {
         // setup new metadata vectors with PS order
         priorityScheduling(metadataDescriptors, metadataCodes, metadataCycles,
                            newMetadataDescriptors, newMetadataCodes, newMetadataCycles);
                            
         // start time point
         t1 = chrono::high_resolution_clock::now();
         
         for(int i = 0; i < count; i++)
         {
            if(newMetadataCodes[i] == 'A' && newMetadataDescriptors[i] == "start" && newMetadataCycles[i] == 0)
            {
               processNumber++;
            }
            // process and log with new metadata vectors
            processAndLog(cycleTimes, newMetadataDescriptors, newMetadataCodes, newMetadataCycles,
                          logType, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                          memoryBlockSize, multiplier, semaphore, speakerQuantity, printerQuantity,
                          speakerCounter, printerCounter, processNumber);
         }
      }
      else if(schedulingCode == "RR")
      {
         roundRobin(metadataDescriptors, metadataCodes, metadataCycles,
                    newMetadataDescriptors, newMetadataCodes, newMetadataCycles);
         
         // start time point
         t1 = chrono::high_resolution_clock::now();
         
         for(int i = 0; i < count; i++)
         {
            if(metadataCodes[i] == 'A' && metadataDescriptors[i] == "start" && metadataCycles[i] == 0)
            {
               processNumber++;
            }
            // process and log with new metadata vectors
            processAndLog(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                          logType, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                          memoryBlockSize, multiplier, semaphore, speakerQuantity, printerQuantity,
                          speakerCounter, printerCounter, processNumber);
         }
      }
      else if(schedulingCode == "STR")
      {
         shortestTimeRemaining(metadataDescriptors, metadataCodes, metadataCycles,
                               newMetadataDescriptors, newMetadataCodes, newMetadataCycles);
         
         // start time point
         t1 = chrono::high_resolution_clock::now();
         
         for(int i = 0; i < count; i++)
         {
            if(metadataCodes[i] == 'A' && metadataDescriptors[i] == "start" && metadataCycles[i] == 0)
            {
               processNumber++;
            }
            // process and log with new metadata vectors
            processAndLog(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                          logType, systemMemory, i, t1, t2, time_span, fout, PCBmain,
                          memoryBlockSize, multiplier, semaphore, speakerQuantity, printerQuantity,
                          speakerCounter, printerCounter, processNumber);
         }
      }
      else
      {
         // throw an error
         throw -8;
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
