#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

using namespace std;

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
int handleErrors(const int e, map<string, int>& cycleTimes, vector<string>& mdd,
                 vector<char>& mdco, vector<int>& mdcy, const string logFilepath,
                 const int logType, const int count, const int sm);
void log(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
         vector<int>& mdcy, const string logFilepath, const int logType, 
         const int count, const int sm);
void logToFile(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
               vector<int>& mdcy, const string logFilepath, const int logType, 
               const int count, const int sm);
void logToMonitor(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                  vector<int>& mdcy, const string logFilepath, const int logType, 
                  const int count, const int sm);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes, 
                           string& mdfp, string& lfp, int& lt, int& sm, string& units);
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                      vector<int>& cycles, int& count);