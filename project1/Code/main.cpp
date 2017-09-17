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
   
   fin.clear();
   fin.open(argv[1]);
   
   try
   {
      checkConfigurationFile(fin, argv);
   }
   catch(int e)
   {
      return handleErrors(e);
   }
   
   readConfigurationFile(fin, cycleTimes, metadataFilepath, logFilepath, logType);
   
   fin.close();
   
   fin.clear();
   fin.open(metadataFilepath);
   
   try
   {
      checkMetadataFile(fin, metadataFilepath);
   }
   catch(int e)
   {
      return handleErrors(e);
   }
   
   readMetadataFile(fin, metadataDescriptors, metadataCodes, metadataCycles, count);
   
   fin.close();
                
   log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
             logFilepath, logType, count);
   
   return EXIT_SUCCESS;
}