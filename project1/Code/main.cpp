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
   // declare variables, initialize count and set default logtype (Log to Both)
   ifstream fin;
   string metadataFilepath, logFilepath;
   map<string, int> cycleTimes;
   vector<char> metadataCodes;
   vector<string> metadataDescriptors;
   vector<int> metadataCycles;
   int count = 0, logType = 2;
   
   //clear input filestream and open configuration file
   fin.clear();
   fin.open(argv[1]);
   
   // check that the configuration file opens without errors
   try
   {
      checkConfigurationFile(fin, argv);
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e);
   }
   
   // process configuration file
   readConfigurationFile(fin, cycleTimes, metadataFilepath, logFilepath, logType);
   
   // close configuration file
   fin.close();
   
   // clear input filestream and open metadata file
   fin.clear();
   fin.open(metadataFilepath);
   
   // check that the metadata file opens without errors
   try
   {
      checkMetadataFile(fin, metadataFilepath);
   }
   // handle errors if any are thrown
   catch(int e)
   {
      // if error occurs, exit program with error code
      return handleErrors(e);
   }
   
   try
   {
      // process metadata file
      readMetadataFile(fin, metadataDescriptors, metadataCodes, metadataCycles, count);
   }
   catch(int e)
   {
      return handleErrors(e);
   }
   
   
   // close metadata file
   fin.close();
   
   // log results 
   log(cycleTimes, metadataDescriptors, metadataCodes, metadataCycles,
       logFilepath, logType, count);
   
   return EXIT_SUCCESS;
}