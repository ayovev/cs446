#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<chrono>

using namespace std;
using namespace std::chrono;

struct PCB
{
   int processState;
};

// function prototypes
int calculateCycleTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                       vector<int>& mdc, const int index);
double calculateSleepTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                          vector<int>& mdc, const int index);
void checkConfigurationFile(ifstream& fin, const char *argv[]);
void checkMetadataFile(ifstream& fin, const string mdfp);
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes);
void getLogType(ifstream& fin, int& lt);
void getLogFilepath(ifstream& fin, string& lfp);
void getLogTypeAndFilepath(ifstream& fin, string& lfp, int& lt);
void getMetadataFilepath(ifstream& fin, string& mdfp);
void getSystemMemory(ifstream& fin, int& sm, string& units);
int handleErrors(const int e);
void myWait(int ms);
void printTime(high_resolution_clock::time_point t1, 
               high_resolution_clock::time_point t2,
               duration<double> time_span, const int logType, ofstream& fout);
void processAndLog(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   vector<int>& mdcy, const string logFilepath, const int logType, 
                   const int count, const int sm, const int i,
                   high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
                   duration<double> time_span, ofstream& fout, PCB PCBmain);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes, 
                           string& mdfp, string& lfp, int& lt, int& sm, string& units);
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                      vector<int>& cycles, int& count);