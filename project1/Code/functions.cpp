#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

#include"functions.hpp"

using namespace std;

const int MONITOR = 0;
const int OUTPUT_FILE = 1;
const int MONITOR_AND_OUTPUT_FILE = 2;
const char SPACE = ' ';
const char NEWLINE = '\n';
const char LEFT_PARENTHESE = '(';
const char RIGHT_PARENTHESE = ')';
const char HYPHEN = '-';

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

// gets the various component cycle times from the configuration file
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes)
{
   // declare variables
   string component;
   int cycleTime;
   
   // prime while loop
   fin >> component;
      
   while(component != "Log:")
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

// uses modular functions to get log type and filepath from configuration file
void getLogTypeAndFilepath(ifstream& fin, string& lfp, int& lt)
{
   getLogType(fin, lt);
   
   getLogFilepath(fin, lfp);
}

// gets log type from configuration file
void getLogType(ifstream& fin, int& lt)
{
   // declare variables
   string s, temp;
   
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

// uses modular functions to read the entire configuration file
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp, int& lt)
{
   getMetadataFilepath(fin, mdfp);
   
   getComponentCycleTimes(fin, cycleTimes);
   
   getLogTypeAndFilepath(fin, lfp, lt);
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
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                      vector<int>& cycles, int& count)
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

// calculate the metadata metrics by "mapping" metadata descriptors to their
// corresponding components
int calculateTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                  vector<int>& mdc, int index)
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

// logs all data to the monitor in the prescribed example format
void logToMonitor(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                  vector<int>& mdcy, string logFilepath, int logType, int count)
{
   cout << endl << "Configuration File Data" << endl;   
   for(auto& x : cycleTimes)
   {
      cout << x.first << " = " << x.second << " ms / cycle" << endl;
   }
   
   cout << "Logged to: ";
   
   if(logType == MONITOR)
   {
      cout << "monitor" << endl << endl;
   }
   else if(logType == OUTPUT_FILE)
   {
      cout << logFilepath << endl << endl;
   }
   else if(logType == MONITOR_AND_OUTPUT_FILE)
   {
      cout << "monitor and " << logFilepath << endl << endl;
   }
   
   cout << "Meta-Data Metrics" << endl;
   for(int index = 2; index < count - 2; index++)
   {      
      cout << mdco[index] << LEFT_PARENTHESE
           << mdd[index] << RIGHT_PARENTHESE
           << mdcy[index] << SPACE << HYPHEN << SPACE
           << calculateTime(cycleTimes, mdd, mdcy, index)
           << endl;
   }
   cout << endl;
}

// logs all data to the given file in the prescribed example format
void logToFile(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
               vector<int>& mdcy, string logFilepath, int logType, int count)
{
   // declare variable
   ofstream fout;
   
   // clear and open filestream
   fout.clear();
   fout.open(logFilepath);
   
   fout << "Configuration File Data" << endl;   
   for(auto& x : cycleTimes)
   {
      fout << x.first << " = " << x.second << " ms / cycle" << endl;
   }
   
   fout << "Logged to: ";
   
   if(logType == MONITOR)
   {
      fout << "monitor" << endl << endl;
   }
   else if(logType == OUTPUT_FILE)
   {
      fout << logFilepath << endl << endl;
   }
   else if(logType == MONITOR_AND_OUTPUT_FILE)
   {
      fout << "monitor and " << logFilepath << endl << endl;
   }
   
   fout << "Meta-Data Metrics" << endl;
   for(int index = 2; index < count - 2; index++)
   {      
      fout << mdco[index] << LEFT_PARENTHESE
           << mdd[index] << RIGHT_PARENTHESE
           << mdcy[index] << SPACE << HYPHEN << SPACE
           << calculateTime(cycleTimes, mdd, mdcy, index)
           << endl;
   }
}

// checks the logtype variable and uses modular functions to log accordingly
void log(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
         vector<int>& mdcy, string logFilepath, int logType, int count)
{
   if(logType == MONITOR)
   {
      logToMonitor(cycleTimes, mdd, mdco, mdcy,
                   logFilepath, logType, count);
   }
   else if(logType == OUTPUT_FILE)
   {
      logToFile(cycleTimes, mdd, mdco, mdcy,
                logFilepath, logType, count);
   }
   else if(logType == MONITOR_AND_OUTPUT_FILE)
   {
      logToMonitor(cycleTimes, mdd, mdco, mdcy,
                   logFilepath, logType, count);
                   
      logToFile(cycleTimes, mdd, mdco, mdcy,
                logFilepath, logType, count);
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

// handles all errors given the error code by displaying a corresponding
// message and terminating the program
int handleErrors(int e)
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
}