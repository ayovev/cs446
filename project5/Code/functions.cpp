#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>
#include<iomanip>
#include<thread>
#include<algorithm>
#include<pthread.h>
#include<semaphore.h>

#include"functions.hpp"

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
const char COLON = ':';

const int START = 0;
const int READY = 1;
const int RUNNING = 2;
const int WAITING = 3;
const int EXIT = 4;

// calculate the metadata metrics by "mapping" metadata descriptors to their
// corresponding components
int calculateCycleTime(map<string, int>& cycleTimes, vector<string>& mdd,
                       vector<int>& mdc, const int index)
{
   if(mdd[index] == "run")
   {
      return mdc[index] * cycleTimes["Processor"];
   }
   else if(mdd[index] == "hard drive")
   {
      return mdc[index] * cycleTimes["Hard drive"];
   }
   else if(mdd[index] == "keyboard")
   {
      return mdc[index] * cycleTimes["Keyboard"];
   }
   else if(mdd[index] == "mouse")
   {
      return mdc[index] * cycleTimes["Mouse"];
   }
   else if(mdd[index] == "monitor")
   {
      return mdc[index] * cycleTimes["Monitor"];
   }
   else if(mdd[index] == "speaker")
   {
      return mdc[index] * cycleTimes["Speaker"];
   }
   else if(mdd[index] == "block")
   {
      return mdc[index] * cycleTimes["Memory"];
   }
   else if(mdd[index] == "allocate")
   {
      return mdc[index] * cycleTimes["Memory"];
   }
   else if(mdd[index] == "printer")
   {
      return mdc[index] * cycleTimes["Printer"];
   }
   else
   {
      return EXIT_FAILURE;
   }
}

// calculate the value for sleep / wait function
double calculateSleepTime(map<string, int>& cycleTimes, vector<string>& mdd,
                    vector<int>& mdc, const int index)
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

// checks the configuration file for any potential errors
void checkConfigurationFile(ifstream& fin, const char *argv[])
{
   // declare variables
   string s = argv[1];
   int found = s.find(".conf");

   // checks extension of configuration file
   if(found == -1)
   {
      throw -1;
   }
   // checks for a valid filename (open file)
   if(fin.is_open() == false)
   {
      throw 0;
   }
   // checks if the file is empty
   if(!(fin >> s))
   {
      throw -3;
   }
}

// checks the metadata file for any potential errors
void checkMetadataFile(ifstream& fin, string mdfp)
{
   // declare variables
   string s;
   int found = mdfp.find(".mdf");

   // checks extension of metadata file
   if(found == -1)
   {
      throw -2;
   }
   // checks for a valid filename (open file)
   if(fin.is_open() == false)
   {
      throw 0;
   }
   // checks if the file is empty
   if(!(fin >> s))
   {
      throw -4;
   }
}

// gets the various component cycle times from the configuration file
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes)
{
   // declare variables
   string component;
   int cycleTime;

   // prime while loop
   fin >> component;

   while(component != "System")
   {
      // check if the component is a hard drive
      if(component == "Hard")
      {
         // append "drive" to key / component name
         string drive;
         fin >> drive;
         component.append(" ");
         component.append(drive);
      }

      // prime filestream and get the cycle time for the current component
      fin.ignore(256, ':');
      fin >> cycleTime;

      // add element to vector
      cycleTimes.emplace(component, cycleTime);

      // get next component
      fin >> component;
   }
}

void getCPUSchedulingCode(ifstream& fin, string& sc)
{
   // declare variable
   char c;
   
   // prime while loop
   fin >> c;
   
   // read character by character up to colon
   while(c != COLON)
   {
      fin >> c;
   }

   // read in scheduling code quantity
   fin >> sc;
}

void getIntegerData(ifstream& fin, int& id)
{
   // declare variable
   char c;

   // prime while loop
   fin >> c;

   // read character by character up to colon
   while(c != COLON)
   {
      fin >> c;
   }

   // read in integer value
   fin >> id;
}

// gets log type from configuration file
void getLogType(ifstream& fin, int& lt)
{
   // declare variables
   string s, temp;
   char c;
   
   while(c != COLON)
   {
      fin >> c;
   }

   // build string
   fin >> s;
   s.append(" ");
   fin >> temp;
   s.append(temp);
   s.append(" ");
   fin >> temp;
   s.append(temp);

   // check string against possible options to determine log type
   if(s == "Log to Monitor")
   {
      lt = 0;
   }
   else if(s == "Log to File")
   {
      lt = 1;
   }
   else if(s == "Log to Both")
   {
      lt = 2;
   }
}

// gets the path that the log file should be output to
void getLogFilepath(ifstream& fin, string& lfp)
{
   // prime filestream
   fin.ignore(256, ':');

   fin >> lfp;
}

// gets the filepath of the metadata file from the configuration file
void getMetadataFilepath(ifstream& fin, string& mdfp)
{
   // declare variable
   char c;

   // prime filestream
   fin.ignore(256, ':');
   fin.ignore(256, ':');
   fin.get(c);

   fin >> mdfp;
}

// handles all errors given the error code by displaying a corresponding
// message and terminating the program
int handleErrors(const int e)
{
   if(e == 0)
   {
      cout << "ERROR CODE 0; FILE NOT FOUND" << endl;
      return EXIT_FAILURE;
   }
   if(e == -1)
   {
      cout << "ERROR CODE -1; INVALID CONFIGURATION FILE EXTENSION" << endl;
      return EXIT_FAILURE;
   }
   if(e == -2)
   {
      cout << "ERROR CODE -2; INVALID METADATA FILE EXTENSION" << endl;
      return EXIT_FAILURE;
   }
   if(e == -3)
   {
      cout << "ERROR CODE -3, EMPTY CONFIGURATION FILE" << endl;
      return EXIT_FAILURE;
   }
   if(e == -4)
   {
      cout << "ERROR CODE -4, EMPTY METADATA FILE" << endl;
      return EXIT_FAILURE;
   }
   if(e == -5)
   {
      cout << "ERROR CODE -5; INVALID(LOWERCASE) OR MISSING METADATA CODE" << endl;
      return EXIT_FAILURE;
   }
   if(e == -6)
   {
      cout << "ERROR CODE -6; INVALID(TYPO) OR MISSING METADATA DESCRIPTOR" << endl;
      return EXIT_FAILURE;
   }
   if(e == -7)
   {
      cout << "ERROR CODE -7; INVALID(NEGATIVE) OR MISSING METADATA CYCLES" << endl;
      return EXIT_FAILURE;
   }
   if(e == -8)
   {
      cout << "ERROR CODE -8; INVALID SCHEDULING CODE" << endl;
      return EXIT_FAILURE;
   }
}

// custom wait function using thread library
void myWait(int ms)
{
  this_thread::sleep_for(milliseconds(ms));
}

int memoryFunction(const int sm, const int mbs, const int multiplier)
{
   return (multiplier * mbs) % sm;
}

// output duration between two points for timestamp
void printTime(high_resolution_clock::time_point t1,
               high_resolution_clock::time_point t2,
               duration<double> time_span, const int lt, ofstream& fout)
{
   t2 = chrono::high_resolution_clock::now();

   time_span = duration_cast<duration<double>>(t2 - t1);

   if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
   {
      cout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
   }
   if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
   {
      fout << fixed << setprecision(6) << time_span.count() << SPACE << HYPHEN << SPACE;
   }
}

void priorityScheduling(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                        vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy)
{
   // declare variables
   int priorityJob, numProcesses = 0;
   vector<int> processNumber, ioTasksPerProcess, startingIndex, endingIndex;
   
   // get number of processes in metadata file
   numProcesses = 0;
   for(int i = 0; i < mdd.size(); i++)
   {
      // check if current piece of metadata is A(start)0
      if(mdco[i] == 'A' && mdd[i] == "start" && mdcy[i] == 0)
      {
         // increment number of processes
         numProcesses++;
      }
   }
   int outer = numProcesses;
   
   for(int a = 0; a < outer; a++)
   {
      // if first iteration push S(start)0
      if(a == 0)
      {
         // begin constructing new metadata vectors
         newmdco.push_back(mdco[0]);
         newmdd.push_back(mdd[0]);
         newmdcy.push_back(mdcy[0]);
         
         // erase loaded metadata
         mdd.erase(mdd.begin());
         mdco.erase(mdco.begin());
         mdcy.erase(mdcy.begin());
      }
      
      // get number of processes in metadata file
      numProcesses = 0;
      for(int i = 0; i < mdd.size(); i++)
      {
         if(mdco[i] == 'A' && mdd[i] == "start" && mdcy[i] == 0)
         {
            processNumber.push_back(numProcesses);
            numProcesses++;
         }
      }
      
      // get number of I/O tasks and indices for each process in metadata file
      for(int c = 0; c < mdd.size(); c++)
      {
         // if metadata is A(start)0 start indexing
         if(mdco[c] == 'A' && mdd[c] == "start" && mdcy[c] == 0)
         {
            startingIndex.push_back(c);
            int numIOTasks = 0;
            int d = c + 1;
            
            // while metadata is not A(end)0 keep indexing
            while(mdco[d] != 'A' && mdd[d] != "end" && mdcy[d] != 0)
            {
               if(mdco[d] == 'I' || mdco[d] == 'O')
               {
                  numIOTasks++;
               }
               d++;
            }
            // end indexing
            ioTasksPerProcess.push_back(numIOTasks);
            endingIndex.push_back(d);
         }
      }
      
      // assign arbitrary shortest job
      priorityJob = processNumber[0];
      
      // loop through metadata to determine highest priority job
      for(int e = 0; e < ioTasksPerProcess.size(); e++)
      {
         // if current process has more I/O than previous process
         if(ioTasksPerProcess[e] > ioTasksPerProcess[e - 1])
         {
            // set high priority job
            priorityJob = processNumber[e];
         }
      }
      
      // added current shortest process to new metadata vectors
      for(int f = startingIndex[priorityJob]; f <= endingIndex[priorityJob]; f++)
      {
         newmdco.push_back(mdco[f]);
         newmdd.push_back(mdd[f]);
         newmdcy.push_back(mdcy[f]);
      }
      
      // erase loaded metadata
      mdd.erase(mdd.begin() + startingIndex[priorityJob], mdd.begin() + (endingIndex[priorityJob] + 1));
      mdco.erase(mdco.begin() + startingIndex[priorityJob], mdco.begin() + (endingIndex[priorityJob] + 1));
      mdcy.erase(mdcy.begin() + startingIndex[priorityJob], mdcy.begin() + (endingIndex[priorityJob] + 1));
      
      // if last iteration add S(end)0
      if(mdd.size() == 1)
      {
         newmdco.push_back(mdco[0]);
         newmdd.push_back(mdd[0]);
         newmdcy.push_back(mdcy[0]);
      }
      
      // reset temp storage vectors
      processNumber.clear();
      ioTasksPerProcess.clear();
      startingIndex.clear();
      endingIndex.clear();
   }
   
}

// logs all data to the monitor in the prescribed example format and changes
// PCB process state
void processAndLog(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   vector<int>& mdcy, const int lt, const int sm, const int i,
                   high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
                   duration<double> time_span, ofstream& fout, PCB PCBmain,
                   const int mbs, int& mult, sem_t semaphore, const int sq, const int pq,
                   int& sc, int& pc, int& processNumber)
{
   printTime(t1, t2, time_span, lt, fout);

   if(mdco[i] == 'S')
   {
      if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
      {
         cout << "Simulator program ";
      }
      if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
      {
         fout << "Simulator program ";
      }

      if(mdd[i] == "start")
      {
         PCBmain.processState = START;

         if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
         {
            cout << "starting" << endl;
         }
         if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
         {
            fout << "starting" << endl;
         }
      }
      else if(mdd[i] == "end")
      {
         PCBmain.processState = EXIT;

         if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
         {
            cout << "ending" << endl;
         }
         if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
         {
            fout << "ending";
         }
      }
   }

   // check metadata code and output data accordingly
   if(mdco[i] == 'A')
   {
      if(mdd[i] == "start")
      {
         PCBmain.processState = START;

         for(int j = 0; j < 2; j++)
         {
            if(j == 0)
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "OS: preparing process " << processNumber << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "OS: preparing process " << processNumber << endl;
               }
            }
            else if(j == 1)
            {
               myWait(calculateSleepTime(cycleTimes, mdd, mdcy, i));

               printTime(t1, t2, time_span, lt, fout);

               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "OS: starting process " << processNumber << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "OS: starting process " << processNumber << endl;
               }
            }
         }
      }
      else if(mdd[i] == "end")
      {
         PCBmain.processState = EXIT;

         if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
         {
            cout << "OS: removing process " << processNumber << endl;
         }
         if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
         {
            fout << "OS: removing process " << processNumber << endl;
         }
      }
   }

   // check metadata code and output data accordingly
   if(mdco[i] == 'P')
   {
      PCBmain.processState = RUNNING;

      for(int j = 0; j < 2; j++)
      {
         if(j == 0)
         {
            if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
            {
               cout << "Process " << processNumber << ": start processing action" << endl;
            }
            if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
            {
               fout << "Process " << processNumber << ": start processing action" << endl;
            }
         }
         else if(j == 1)
         {
            myWait(calculateSleepTime(cycleTimes, mdd, mdcy, i));

            printTime(t1, t2, time_span, lt, fout);

            if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
            {
               cout << "Process " << processNumber << ": end processing action" << endl;
            }
            if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
            {
               fout << "Process " << processNumber << ": end processing action" << endl;
            }
         }
      }
   }

   // check metadata code and output data accordingly
   if(mdco[i] == 'M')
   {
      PCBmain.processState = WAITING;

      if(mdd[i] == "allocate")
      {
         for(int j = 0; j < 2; j++)
         {
            if(j == 0)
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "Process " << processNumber << ": allocating memory" << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "Process " << processNumber << ": allocating memory" << endl;
               }
            }
            else if(j == 1)
            {
               myWait(calculateSleepTime(cycleTimes, mdd, mdcy, i));

               printTime(t1, t2, time_span, lt, fout);

               int temp = memoryFunction(sm, mbs, mult);

               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "memory allocated at 0x"
                       << setfill('0') << setw(8)
                       << hex << temp << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "memory allocated at 0x"
                       << setfill('0') << setw(8)
                       << hex << temp << endl;
               }
            }
         }
         mult++;
      }
      else if(mdd[i] == "block")
      {
         for(int j = 0; j < 2; j++)
         {
            if(j == 0)
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "Process " << processNumber << ": start memory blocking" << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "Process " << processNumber << ": start memory blocking" << endl;
               }
            }
            else if(j == 1)
            {
               myWait(calculateSleepTime(cycleTimes, mdd, mdcy, i));

               printTime(t1, t2, time_span, lt, fout);

               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << "Process " << processNumber << ": end memory blocking" << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << "Process " << processNumber << ": end memory blocking" << endl;
               }
            }
         }
      }
   }

   // check metadata code and output data accordingly
   if(mdco[i] == 'O' || mdco[i] == 'I')
   {
      // initialize threads for input / output only
      pthread_t tid;
      pthread_attr_t attr;
      pthread_attr_init(&attr);

      // lock critical section using semaphore
      sem_wait(&semaphore);

      // set PCB state
      PCBmain.processState = WAITING;

      for(int j = 0; j < 2; j++)
      {
         if(j == 0)
         {
            if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
            {
               cout << "Process " << processNumber << ": start " << mdd[i];
            }
            if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
            {
               fout << "Process " << processNumber << ": start " << mdd[i];
            }

            if(mdco[i] == 'O')
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  if(mdd[i] == "monitor")
                  {
                     cout << " output";
                  }
                  if(mdd[i] == "hard drive")
                  {
                     cout << " output";
                  }
                  if(mdd[i] == "printer")
                  {
                     cout << " output on ";
                     cout << "PRNTR " << pc % pq;
                     pc++;
                  }
                  if(mdd[i] == "speaker")
                  {
                     cout << " output on ";
                     cout << "SPKR " << sc % sq;
                     sc++;
                  }
                  cout << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  if(mdd[i] == "monitor")
                  {
                     fout << " output";
                  }
                  if(mdd[i] == "hard drive")
                  {
                     fout << " output";
                  }
                  if(mdd[i] == "printer")
                  {
                     fout << " output on ";
                     fout << "PRNTR " << pc % pq;
                     pc++;
                  }
                  if(mdd[i] == "speaker")
                  {
                     fout << " output on ";
                     fout << "SPKR " << sc % sq;
                     sc++;
                  }
                  fout << endl;
               }
            }
            else if(mdco[i] == 'I')
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  if(mdd[i] == "monitor")
                  {
                     cout << " input";
                  }
                  if(mdd[i] == "hard drive")
                  {
                     cout << " input ";
                  }
                  if(mdd[i] == "printer")
                  {
                     cout << " input on ";
                     cout << "PRNTR " << pc % pq;
                     pc++;
                  }
                  cout << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  if(mdd[i] == "monitor")
                  {
                     fout << " input";
                  }
                  if(mdd[i] == "hard drive")
                  {
                     fout << " input ";
                  }
                  if(mdd[i] == "printer")
                  {
                     fout << " input on ";
                     fout << "PRNTR " << pc % pq;
                     pc++;
                  }
                  fout << endl;
               }
            }
         }
         else if(j == 1)
         {
            int waitTime = calculateSleepTime(cycleTimes, mdd, mdcy, i);

            void* ptr = &waitTime;

            // create and join threads
            pthread_create(&tid, &attr, runner, ptr);
            pthread_join(tid, NULL);

            // calculate and print time
            printTime(t1, t2, time_span, lt, fout);

            if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
            {
               cout << "Process " << processNumber << ": end " << mdd[i];
            }
            if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
            {
               fout << "Process " << processNumber << ": end " << mdd[i];
            }

            if(mdco[i] == 'O')
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << " output" << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << " output" << endl;
               }
            }
            else if(mdco[i] == 'I')
            {
               if(lt == MONITOR || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  cout << " input" << endl;
               }
               if(lt == OUTPUT_FILE || lt == MONITOR_AND_OUTPUT_FILE)
               {
                  fout << " input" << endl;
               }
            }
         }
      }
      // unlock semaphore after breaking out of critical section
      sem_post(&semaphore);
   }
}

// uses modular functions to read the entire configuration file
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp, int& lt, int& sm,
                           int& mbs, int& pq, int& sq, int& pqn, string& sc)
{
   // get metadata filepath
   getMetadataFilepath(fin, mdfp);
   
   // get processor quantum number
   getIntegerData(fin, pqn);
   
   // get CPU scheduling code
   getCPUSchedulingCode(fin, sc);

   // get cycle times for each component
   getComponentCycleTimes(fin, cycleTimes);
   
   // get system memory size
   getIntegerData(fin, sm);

   // get memory block size
   getIntegerData(fin, mbs);

   // get printer quantity
   getIntegerData(fin, pq);

   // get speaker quantity
   getIntegerData(fin, sq);

   // get logtype
   getLogType(fin, lt);

   // get log file path (if any exists)
   getLogFilepath(fin, lfp);
}

// reads in a single piece of metadata from the metadata file
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc,
                 vector<int>& cycles)
{
   // declare variables
   string mddTemp;
   char mdcTemp, lp, rp, mddAppend;
   int cyclesTemp = -999;

   // get metadata code
   fin >> mdcTemp >> lp;

   // check if the metadata code is lowercase or an invalid character
   if(mdcTemp < 'A' || mdcTemp > 'Z')
   {
      throw -5;
   }

   // add element to vector
   mdc.push_back(mdcTemp);

   // prime while loop
   fin >> mddAppend;
   while(mddAppend != ')')
   {
      // construct the metadata descriptor string character by character
      mddTemp += mddAppend;

      // check if the component is a hard drive
      if(mddTemp == "hard")
      {
         mddTemp += SPACE;
      }
      fin >> mddAppend;
   }

   // check for an invalid metadata descriptor
   if(!(mddTemp == "start" || mddTemp == "end" || mddTemp == "run" ||
      mddTemp == "hard drive" || mddTemp == "keyboard" || mddTemp == "printer" ||
      mddTemp == "monitor" || mddTemp == "allocate" || mddTemp == "block" ||
      mddTemp == "mouse" || mddTemp == "speaker"))
   {
      throw -6;
   }

   // add element to vector
   mdd.push_back(mddTemp);
   rp = mddAppend;

   // check if the number of cycles is missing
   if(fin.peek() == ';')
   {
      throw -7;
   }

   fin >> cyclesTemp;

   // check if the number of cycles is negative
   if(cyclesTemp < 0)
   {
      throw -7;
   }

   // add element to vector
   cycles.push_back(cyclesTemp);
}

// uses a modular function to read the entire metadata file
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, vector<int>& cycles, int& count)
{
   // declare variables
   char c;
   count = 0;

   // prime filestream
   fin.ignore(256, NEWLINE);

   while(c != '.')
   {
      readOneMeta(fin, mdd, mdc, cycles);
      count++;
      fin >> c;
   }
}

void roundRobin(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy)
{
   // declare variables
   int numProcesses = 0;
   vector<int> processNumber, tasksPerProcess, startingIndex, endingIndex;
   
   // get number of processes in metadata file
   numProcesses = 0;
   for(int i = 0; i < mdd.size(); i++)
   {
      // check if current piece of metadata is A(start)0
      if(mdco[i] == 'A' && mdd[i] == "start" && mdcy[i] == 0)
      {
         // increment number of processes
         numProcesses++;
      }
   }
   int outer = numProcesses;
   
   // loop for the number of processes
   for(int a = 0; a < outer; a++)
   {      
      // get number of processes in metadata file
      numProcesses = 0;
      for(int b = 0; b < mdd.size(); b++)
      {
         if(mdco[b] == 'A' && mdd[b] == "start" && mdcy[b] == 0)
         {
            processNumber.push_back(numProcesses);
            numProcesses++;
         }
      }
      
      // get number of tasks and indices for each process in metadata file
      for(int c = 0; c < mdd.size(); c++)
      {
         // if metadata is A(start)0 start indexing
         if(mdco[c] == 'A' && mdd[c] == "start" && mdcy[c] == 0)
         {
            startingIndex.push_back(c);
            int numTasks = 0;
            int d = c + 1;
            
            // while metadata is not A(end)0 keep indexing
            while(mdco[d] != 'A' && mdd[d] != "end" && mdcy[d] != 0)
            {
               numTasks++;
               d++;
            }
            // end indexing
            tasksPerProcess.push_back(numTasks);
            endingIndex.push_back(d);
         }
      }
   }
}

// threading runner function
void* runner(void* total)
{
   int* a = (int*)total;
   myWait(*a);
   pthread_exit(NULL);
}

void shortestJobFirst(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                      vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy)
{
   // declare variables
   int shortestJob, numProcesses = 0;
   vector<int> processNumber, tasksPerProcess, startingIndex, endingIndex;
   
   // get number of processes in metadata file
   numProcesses = 0;
   for(int i = 0; i < mdd.size(); i++)
   {
      // check if current piece of metadata is A(start)0
      if(mdco[i] == 'A' && mdd[i] == "start" && mdcy[i] == 0)
      {
         // increment number of processes
         numProcesses++;
      }
   }
   int outer = numProcesses;
   
   // loop for the number of processes
   for(int a = 0; a < outer; a++)
   {
      // if first iteration push S(start)0
      if(a == 0)
      {
         // begin constructing new metadata vectors
         newmdco.push_back(mdco[0]);
         newmdd.push_back(mdd[0]);
         newmdcy.push_back(mdcy[0]);
         
         // erase loaded metadata
         mdd.erase(mdd.begin());
         mdco.erase(mdco.begin());
         mdcy.erase(mdcy.begin());
      }
      
      // get number of processes in metadata file
      numProcesses = 0;
      for(int b = 0; b < mdd.size(); b++)
      {
         if(mdco[b] == 'A' && mdd[b] == "start" && mdcy[b] == 0)
         {
            processNumber.push_back(numProcesses);
            numProcesses++;
         }
      }
      
      // get number of tasks and indices for each process in metadata file
      for(int c = 0; c < mdd.size(); c++)
      {
         // if metadata is A(start)0 start indexing
         if(mdco[c] == 'A' && mdd[c] == "start" && mdcy[c] == 0)
         {
            startingIndex.push_back(c);
            int numTasks = 0;
            int d = c + 1;
            
            // while metadata is not A(end)0 keep indexing
            while(mdco[d] != 'A' && mdd[d] != "end" && mdcy[d] != 0)
            {
               numTasks++;
               d++;
            }
            // end indexing
            tasksPerProcess.push_back(numTasks);
            endingIndex.push_back(d);
         }
      }
      
      // assign arbitrary shortest job
      shortestJob = processNumber[0];
      
      // loop through metadata to determine shortest job
      for(int e = 0; e < tasksPerProcess.size(); e++)
      {
         // if current process is shorter than previous process
         if(tasksPerProcess[e] < tasksPerProcess[e - 1])
         {
            // set shortest job
            shortestJob = processNumber[e];
         }
      }
      
      // added current shortest process to new metadata vectors
      for(int f = startingIndex[shortestJob]; f <= endingIndex[shortestJob]; f++)
      {
         newmdco.push_back(mdco[f]);
         newmdd.push_back(mdd[f]);
         newmdcy.push_back(mdcy[f]);
      }
      
      // erase loaded metadata
      mdd.erase(mdd.begin() + startingIndex[shortestJob], mdd.begin() + (endingIndex[shortestJob] + 1));
      mdco.erase(mdco.begin() + startingIndex[shortestJob], mdco.begin() + (endingIndex[shortestJob] + 1));
      mdcy.erase(mdcy.begin() + startingIndex[shortestJob], mdcy.begin() + (endingIndex[shortestJob] + 1));
      
      // if last iteration add S(end)0
      if(mdd.size() == 1)
      {
         newmdco.push_back(mdco[0]);
         newmdd.push_back(mdd[0]);
         newmdcy.push_back(mdcy[0]);
      }
      
      // reset temp storage vectors
      processNumber.clear();
      tasksPerProcess.clear();
      startingIndex.clear();
      endingIndex.clear();
   }
}

void shortestTimeRemaining(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                           vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy)
{
   // declare variables
   int numProcesses = 0;
   vector<int> processNumber, tasksPerProcess, startingIndex, endingIndex;
   
   // get number of processes in metadata file
   numProcesses = 0;
   for(int i = 0; i < mdd.size(); i++)
   {
      // check if current piece of metadata is A(start)0
      if(mdco[i] == 'A' && mdd[i] == "start" && mdcy[i] == 0)
      {
         // increment number of processes
         numProcesses++;
      }
   }
   int outer = numProcesses;
   
   // loop for the number of processes
   for(int a = 0; a < outer; a++)
   {      
      // get number of processes in metadata file
      numProcesses = 0;
      for(int b = 0; b < mdd.size(); b++)
      {
         if(mdco[b] == 'A' && mdd[b] == "start" && mdcy[b] == 0)
         {
            processNumber.push_back(numProcesses);
            numProcesses++;
         }
      }
      
      // get number of tasks and indices for each process in metadata file
      for(int c = 0; c < mdd.size(); c++)
      {
         // if metadata is A(start)0 start indexing
         if(mdco[c] == 'A' && mdd[c] == "start" && mdcy[c] == 0)
         {
            startingIndex.push_back(c);
            int numTasks = 0;
            int d = c + 1;
            
            // while metadata is not A(end)0 keep indexing
            while(mdco[d] != 'A' && mdd[d] != "end" && mdcy[d] != 0)
            {
               numTasks++;
               d++;
            }
            // end indexing
            tasksPerProcess.push_back(numTasks);
            endingIndex.push_back(d);
         }
      }
   }
}