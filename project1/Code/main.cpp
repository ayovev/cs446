#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>

using namespace std;

const char SPACE = ' ';

void getMetadataFilepath(ifstream& fin, string& mdfp);
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes);
void getLogFilepath(ifstream& fin, string& lfp);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp);

int main(int argc, const char *argv[])
{
   // declare variables
   ifstream fin;
   string metadataFilepath, logFilepath;
   map<string, int> cycleTimes;
   
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
   
   fin.ignore(256, '\n');
   
   string s;
   
   fin >> s;
   cout << s << endl;
   
   // close file stream
   fin.close();
   
   
   
   return EXIT_SUCCESS;
}

void getMetadataFilepath(ifstream& fin, string& mdfp)
{
   char c;
   
   fin.ignore(256, ':');
   fin.ignore(256, ':');
   fin.get(c);
   
   fin >> mdfp;
}

void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes)
{
   // declare variables
   string component, check;
   int cycleTime;
   
   // prime while loop
   fin >> component;
   check = component;
      
   while(check != "Log:")
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
      
      fin.ignore(256, ':');
      fin >> cycleTime;
      
      cycleTimes.emplace(component, cycleTime);
      
      fin >> component;
      check = component;
   }
}

void getLogFilepath(ifstream& fin, string& lfp)
{
   fin.ignore(256, ':');
   
   fin >> lfp;
}

void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp)
{
   getMetadataFilepath(fin, mdfp);
   
   // TESTING - print metadata filepath
   cout << mdfp << endl;
   
   getComponentCycleTimes(fin, cycleTimes);
   
   // TESTING - print map keys and values
   for(auto& x : cycleTimes)
   {
      cout << x.first << SPACE << x.second << endl;
   }
   
   // read in log file path and store in string
   getLogFilepath(fin, lfp);
   
   cout << lfp << endl;
}