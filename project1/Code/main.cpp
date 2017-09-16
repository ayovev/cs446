#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>

using namespace std;

const char SPACE = ' ';

void getMetadataFilepath(ifstream& fin, string& mdfp);
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes);

int main(int argc, const char *argv[])
{
   // declare variables
   ifstream fin;
   string metadataFilepath;
   map<string, int> cycleTimes;
   
   //clear and open file stream
   fin.clear();
   fin.open(argv[1]);
   
   getMetadataFilepath(fin, metadataFilepath);
   
   //TESTING
   cout << metadataFilepath << endl;
      
////////////////////////////////////////////////////////////////////////////////
   
   getComponentCycleTimes(fin, cycleTimes);
   
   for(auto& x : cycleTimes)
   {
      cout << x.first << SPACE << x.second << endl;
   }
   
   // read in log file path and store in string
      
      
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
   string component;
   int cycleTime;
   
   fin >> component;
   fin.ignore(256, ':');
   fin >> cycleTime;
   
   cycleTimes.emplace(component, cycleTime);
   
   fin >> component;
   fin.ignore(256, ':');
   fin >> cycleTime;
   
   cycleTimes.emplace(component, cycleTime);
   
   fin >> component;
   fin.ignore(256, ':');
   fin >> cycleTime;
   
   cycleTimes.emplace(component, cycleTime);
      
}
