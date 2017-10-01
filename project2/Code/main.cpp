#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>
#include<iomanip>
#include<thread>

// unix usleep function
#include<unistd.h>

// windows Sleep function -- WINDOWS TESTING PURPOSES ONLY
#include<windows.h>

#include"functions.hpp"
#include"MemoryFunction.h"

using namespace std;
using namespace std::chrono;

const int MONITOR = 0;
const int OUTPUT_FILE = 1;
const int MONITOR_AND_OUTPUT_FILE = 2;
const char SPACE = ' ';
const char NEWLINE = '\n';
const char LEFT_PARENTHESE = '(';
const char RIGHT_PARENTHESE = ')';
const char HYPHEN = '-';

const int START = 0;
const int READY = 1;
const int RUNNING = 2;
const int WAITING = 3;
const int EXIT = 4;

struct PCB
{
   int processState;
};
             
double getSleepTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                    vector<int>& mdc, int index);
                    
void output(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
            vector<int>& mdcy, string logFilepath, int logType, int count);

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
      
      // // log results
      // log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
      //     logFilepath, logType, count);
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e, cycleTimes, metadataDescriptors, metadataCodes,
                          metadataCycles, logFilepath, logType, count);
   }
////////////////////////////////////////////////////////////////////////////////
   // declare variables
   ofstream fout;
   
   // clear and open filestream
   fout.clear();
   fout.open(logFilepath);
   
   // capture snapshot of starting point in time
   t1 = chrono::high_resolution_clock::now();
   
   // loop through all pieces of metadata in the metadata file
   for(int i = 0; i < count; i++)
   {
      // check metadata code and output data accordingly
      if(metadataCodes[i] == 'S')
      {
         Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
         
         t2 = chrono::high_resolution_clock::now();
         
         time_span = duration_cast<duration<double>>(t2 - t1);
         
         fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
         
         fout << "Simulator program ";
         
         if(metadataDescriptors[i] == "start")
         {
            fout << "starting" << endl;
         }
         else if(metadataDescriptors[i] == "end")
         {
            fout << "ending";
         }
      }
      
      // check metadata code and output data accordingly
      if(metadataCodes[i] == 'A')
      {
         if(metadataDescriptors[i] == "start")
         {
            for(int j = 0; j < 2; j++)
            {
               Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
               
               t2 = high_resolution_clock::now();
               
               time_span = duration_cast<duration<double>>(t2 - t1);
               
               fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
               
               if(j == 0)
               {
                  fout << "OS: preparing process 1" << endl;
               }
               else if(j == 1)
               {
                  fout << "OS: starting process 1" << endl;
               }
            }
         }
         else if(metadataDescriptors[i] == "end")
         {
            Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
            
            t2 = high_resolution_clock::now();
            
            time_span = duration_cast<duration<double>>(t2 - t1);
            
            fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
            
            fout << "OS: removing process 1" << endl;
         }
      }
      
      // check metadata code and output data accordingly
      if(metadataCodes[i] == 'P')
      {
         for(int j = 0; j < 2; j++)
         {
            Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
            
            t2 = high_resolution_clock::now();
            
            time_span = duration_cast<duration<double>>(t2 - t1);
            
            fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
            
            if(j == 0)
            {
               fout << "Process 1: start processing action" << endl;
            }
            else if(j == 1)
            {
               fout << "Process 1: end processing action" << endl;
            }
         }
      }
      
      // check metadata code and output data accordingly
      if(metadataCodes[i] == 'M')
      {
         if(metadataDescriptors[i] == "allocate")
         {
            for(int j = 0; j < 2; j++)
            {
               Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
               
               t2 = high_resolution_clock::now();
               
               time_span = duration_cast<duration<double>>(t2 - t1);
               
               fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
               
               if(j == 0)
               {
                  fout << "Process 1: allocating memory" << endl;
               }
               else if(j == 1)
               {
                  fout << "memory allocated at 0x" 
                       << setfill('0') << setw(8) 
                       << allocateMemory(systemMemory) << endl;
               }
            }
         }
         else if(metadataDescriptors[i] == "block")
         {
            for(int j = 0; j < 2; j++)
            {
               Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
               
               t2 = high_resolution_clock::now();
               
               time_span = duration_cast<duration<double>>(t2 - t1);
               
               fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
            
               if(j == 0)
               {
                  fout << "Process 1: start memory blocking" << endl;
               }
               else if(j == 1)
               {
                  fout << "Process 1: end memory blocking" << endl;
               }
            }
         }
      }
      
      // check metadata code and output data accordingly
      if(metadataCodes[i] == 'O' || metadataCodes[i] == 'I')
      {
         for(int j = 0; j < 2; j++)
         {
            Sleep(getSleepTime(cycleTimes, metadataDescriptors, metadataCycles, i));
            
            t2 = high_resolution_clock::now();
            
            time_span = duration_cast<duration<double>>(t2 - t1);
            
            fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
            
            if(j == 0)
            {
               fout << "Process 1: start " << metadataDescriptors[i];
               if(metadataCodes[i] == 'O')
               {
                  fout << " output" << endl;
               }
               else if(metadataCodes[i] == 'I')
               {
                  fout << " input" << endl;
               }
            }
            else if(j == 1)
            {
               fout << "Process 1: end " << metadataDescriptors[i];
               if(metadataCodes[i] == 'O')
               {
                  fout << " output" << endl;
               }
               else if(metadataCodes[i] == 'I')
               {
                  fout << " input" << endl;
               }
            }
         }
      }
   }
   
   // otherwise exit program successfully
   return EXIT_SUCCESS;
}

void process(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdc,
             vector<int>& cycles, PCB PCBmain, const int count, const int sm)
{}
   
double getSleepTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                    vector<int>& mdc, int index)
{
   if(mdd[index] == "start" || mdd[index] == "end")
   {
      return (double)0;
   }
   else if(mdd[index] == "run")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Processor"]);
   }
   else if(mdd[index] == "hard drive")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Hard drive"]);
   }
   else if(mdd[index] == "keyboard")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Keyboard"]);
   }
   else if(mdd[index] == "mouse")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Mouse"]);
   }
   else if(mdd[index] == "monitor")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Monitor"]);
   }
   else if(mdd[index] == "speaker")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Speaker"]);
   }
   else if(mdd[index] == "block")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Memory"]);
   }
   else if(mdd[index] == "allocate")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Memory"]);
   }
   else if(mdd[index] == "printer")
   {
      return (double)((double)mdc[index] * (double)cycleTimes["Printer"]);
   }
   else
   {
      return EXIT_FAILURE;
   }
}