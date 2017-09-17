#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

#include"functions.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
   // declare variables
   ifstream fin;
   ofstream fout;
   string metadataFilepath, logFilepath;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   int count = 0;
   int logType = 2; // test value
   
   // clear and open file stream
   fin.clear();
   fin.open(argv[1]);
   
   readConfigurationFile(fin, cycleTimes, metadataFilepath, logFilepath);
   
   // close file stream
   fin.close();
   
////////////////////////////////////////////////////////////////////////////////
   
   // clear and open file stream
   fin.clear();
   fin.open(metadataFilepath);
   
   readMetadataFile(fin, metadataDescriptors, metadataCodes, metadataCycles, count);
      
   // close file stream
   fin.close();
   
////////////////////////////////////////////////////////////////////////////////
   
   cout << "TAKE OUT ABOVE OUTPUT BEFORE SUBMITTING!!" << endl << endl;
   
   logToMonitor(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
                logFilepath, logType, count);
                
////////////////////////////////////////////////////////////////////////////////
                
   fout.clear();
   fout.open(logFilepath);
   
   
   return EXIT_SUCCESS;
}