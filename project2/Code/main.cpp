#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>
#include<iomanip>
#include<thread>

// windows Sleep function -- WINDOWS TESTING PURPOSES ONLY
#include<windows.h>

// unix usleep function
#include<unistd.h>

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

void process(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
             vector<int>& mdcy, const string logFilepath, const int logType, 
             const int count, const int sm, const int i,
             high_resolution_clock::time_point t1,
             high_resolution_clock::time_point t2,
             duration<double> time_span);
             
void printTime(high_resolution_clock::time_point t1, 
               high_resolution_clock::time_point t2,
               duration<double> time_span);
               
               
void myWait(int ms);

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
      
      // log results
      /*log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
          logFilepath, logType, count, systemMemory);*/
          
      t1 = chrono::high_resolution_clock::now();
      for(int i = 0; i < count; i++)
      {
         process(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                 logFilepath, logType, count, systemMemory, i, t1, t2, time_span);
      }
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e, cycleTimes, metadataDescriptors, metadataCodes,
                          metadataCycles, logFilepath, logType, count, systemMemory);
   }
   
   // otherwise exit program successfully
   return EXIT_SUCCESS;
}

const int MONITOR = 0;
const int OUTPUT_FILE = 1;
const int MONITOR_AND_OUTPUT_FILE = 2;
const char SPACE = ' ';
const char NEWLINE = '\n';
const char LEFT_PARENTHESE = '(';
const char RIGHT_PARENTHESE = ')';
const char HYPHEN = '-';

void process(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
             vector<int>& mdcy, const string logFilepath, const int logType, 
             const int count, const int sm, const int i,
             high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
             duration<double> time_span)
{
   printTime(t1, t2, time_span);
   
   if(mdco[i] == 'S')
   {
      cout << "Simulator program ";
      
      if(mdd[i] == "start")
      {
         cout << "starting" << endl;
      }
      else if(mdd[i] == "end")
      {
         cout << "ending";
      }
   }
   
   // check metadata code and output data accordingly
   if(mdco[i] == 'A')
   {
      if(mdd[i] == "start")
      {
         for(int j = 0; j < 2; j++)
         {
            if(j == 0)
            {
               cout << "OS: preparing process 1" << endl;
            }
            else if(j == 1)
            {
               Sleep(calculateSleepTime(cycleTimes, mdd, mdcy, i));
               
               printTime(t1, t2, time_span);
               
               cout << "OS: starting process 1" << endl;
            }
         }
      }
      else if(mdd[i] == "end")
      {
         cout << "OS: removing process 1" << endl;
      }
   }
   
   // check metadata code and output data accordingly
   if(mdco[i] == 'P')
   {
      for(int j = 0; j < 2; j++)
      {
         if(j == 0)
         {
            cout << "Process 1: start processing action" << endl;
         }
         else if(j == 1)
         {
            Sleep(calculateSleepTime(cycleTimes, mdd, mdcy, i));
            
            printTime(t1, t2, time_span);
            
            cout << "Process 1: end processing action" << endl;
         }
      }
   }
   
   // check metadata code and output data accordingly
   if(mdco[i] == 'M')
   {
      if(mdd[i] == "allocate")
      {
         for(int j = 0; j < 2; j++)
         {
            if(j == 0)
            {
               cout << "Process 1: allocating memory" << endl;
            }
            else if(j == 1)
            {
               Sleep(calculateSleepTime(cycleTimes, mdd, mdcy, i));
               
               printTime(t1, t2, time_span);
               
               cout << "memory allocated at 0x" 
                    << setfill('0') << setw(8) 
                    << allocateMemory(sm) << endl;
            }
         }
      }
      else if(mdd[i] == "block")
      {
         for(int j = 0; j < 2; j++)
         {            
            if(j == 0)
            {
               cout << "Process 1: start memory blocking" << endl;
            }
            else if(j == 1)
            {
               Sleep(calculateSleepTime(cycleTimes, mdd, mdcy, i));
               
               printTime(t1, t2, time_span);
               
               cout << "Process 1: end memory blocking" << endl;
            }
         }
      }
   }
   
   // check metadata code and output data accordingly
   if(mdco[i] == 'O' || mdco[i] == 'I')
   {
      for(int j = 0; j < 2; j++)
      {         
         if(j == 0)
         {
            cout << "Process 1: start " << mdd[i];
            if(mdco[i] == 'O')
            {
               cout << " output" << endl;
            }
            else if(mdco[i] == 'I')
            {
               cout << " input" << endl;
            }
         }
         else if(j == 1)
         {
            Sleep(calculateSleepTime(cycleTimes, mdd, mdcy, i));
            
            printTime(t1, t2, time_span);
            
            cout << "Process 1: end " << mdd[i];
            if(mdco[i] == 'O')
            {
               cout << " output" << endl;
            }
            else if(mdco[i] == 'I')
            {
               cout << " input" << endl;
            }
         }
      }
   }
}

void printTime(high_resolution_clock::time_point t1, 
               high_resolution_clock::time_point t2,
               duration<double> time_span)
{
   t2 = chrono::high_resolution_clock::now();
   
   time_span = duration_cast<duration<double>>(t2 - t1);
   
   cout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
}

// void myWait(int ms)
// {
//    std::this_thread::sleep_for(milliseconds(ms));
// }