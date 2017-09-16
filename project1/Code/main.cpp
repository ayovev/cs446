#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

using namespace std;

const char SPACE = ' ';
const char NEWLINE = '\n';
const char LEFT_PARENTHESE = '(';
const char RIGHT_PARENTHESE = ')';

void getMetadataFilepath(ifstream& fin, string& mdfp);
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes);
void getLogFilepath(ifstream& fin, string& lfp);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp);
void readOneMeta(ifstream& fin, vector<char>& mdc, vector<string>& mdd, 
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<char>& mdc, vector<string>& mdd, 
                      vector<int>& cycles);

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
   
   readMetadataFile(fin, metadataCodes, metadataDescriptors, metadataCycles);
      
   // close file stream
   fin.close();
   
////////////////////////////////////////////////////////////////////////////////

   fout.clear();
   fout.open(logFilepath);
   
   
   
   
   fout.close()
   
   
   
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

void readOneMeta(ifstream& fin, vector<char>& mdc, vector<string>& mdd, 
                 vector<int>& cycles)
{
   char mdcTemp, lp, rp, mddAppend;
   string mddTemp;
   int cyclesTemp;
   
   fin >> mdcTemp >> lp;
   mdc.push_back(mdcTemp);
   
   // prime while loop
   fin >> mddAppend;
   while(mddAppend != ')')
   {
      mddTemp += mddAppend;
      if(mddTemp == "hard")
      {
         mddTemp += SPACE;
      }
      fin >> mddAppend;
   }
   mdd.push_back(mddTemp);
   rp = mddAppend;
   
   fin >> cyclesTemp;
   cycles.push_back(cyclesTemp);
   
   
}

void readMetadataFile(ifstream& fin, vector<char>& mdc, vector<string>& mdd, 
                      vector<int>& cycles)
{
   fin.ignore(256, NEWLINE);
   
   char c;   
   int count = 0;
   
   while(c != '.')
   {
      readOneMeta(fin, mdc, mdd, cycles);
      count++;
      fin >> c;
   }
   
   // TESTING - output number of metadata items
   cout << count << endl;
   
   // TESTING - output metadata
   for( int i = 0; i < count; i++ )
   {
      cout << mdc[i] << LEFT_PARENTHESE 
           << mdd[i] << RIGHT_PARENTHESE 
           << cycles[i] << endl;
   }
}