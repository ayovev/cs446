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
   string metadataFilepath, logFilepath;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   int count = 0, logType = 2; // logType test value
   
   // clear and open file stream
   fin.clear();
   fin.open(argv[1]);
   
   readConfigurationFile(fin, cycleTimes, metadataFilepath, logFilepath);
   
   // close file stream
   fin.close();
   
   // clear and open file stream
   fin.clear();
   fin.open(metadataFilepath);
   
   readMetadataFile(fin, metadataDescriptors, metadataCodes, metadataCycles, count);
      
   // close file stream
   fin.close();
                
   log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
             logFilepath, logType, count);
   
   return EXIT_SUCCESS;
}