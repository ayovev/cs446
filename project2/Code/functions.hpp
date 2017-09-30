#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

using namespace std;

// function prototypes
void getMetadataFilepath(ifstream& fin, string& mdfp);
void getComponentCycleTimes(ifstream& fin, map<string, int>& cycleTimes);
void getSystemMemory(ifstream& fin, int& sm, string& units);
void getLogType(ifstream& fin, int& lt);
void getLogFilepath(ifstream& fin, string& lfp);
void getLogTypeAndFilepath(ifstream& fin, string& lfp, int& lt);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes, 
                           string& mdfp, string& lfp, int& lt, int& sm, string& units);
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc, 
                      vector<int>& cycles, int& count);
int calculateTime(map<string, int>& cycleTimes, vector<string>& mdd, 
                  vector<int>& mdc, int index);
void logToMonitor(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco, 
                  vector<int>& mdcy, string logFilepath, int logType, int count);
void logToFile(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
               vector<int>& mdcy, string logFilepath, int logType, int count);
void log(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
         vector<int>& mdcy, string logFilepath, int logType, int count);
void checkConfigurationFile(ifstream& fin, const char *argv[]);
void checkMetadataFile(ifstream& fin, string mdfp);
int handleErrors(int e, map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                 vector<int>& mdcy, string logFilepath, int logType, int count);
