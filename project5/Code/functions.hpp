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
void getCPUSchedulingCode(ifstream& fin, string& sc);
void getIntegerData(ifstream& fin, int& id);
void getLogType(ifstream& fin, int& lt);
void getLogFilepath(ifstream& fin, string& lfp);
void getMetadataFilepath(ifstream& fin, string& mdfp);
int handleErrors(const int e);
int memoryFunction(const int sm, const int mbs, const int multiplier);
void myWait(int ms);
void printTime(high_resolution_clock::time_point t1,
               high_resolution_clock::time_point t2,
               duration<double> time_span, const int logType, ofstream& fout);
void priorityScheduling(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                        vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy);
void processAndLog(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   vector<int>& mdcy, const int lt, const int sm, const int i,
                   high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
                   duration<double> time_span, ofstream& fout, PCB PCBmain,
                   const int mbs, int& mult, sem_t semaphore, const int sq, const int pq,
                   int& sc, int& pc, int& processNumber);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp, int& lt, int& sm,
                           int& mbs, int& pq, int& sq, int& pqn, string& sc);
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc,
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc,
                      vector<int>& cycles, int& count);
void roundRobin(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy);
void* runner(void* total);
void shortestJobFirst(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                      vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy);
void shortestTimeRemaining(vector<string>& mdd, vector<char>& mdco, vector<int>& mdcy,
                           vector<string>& newmdd, vector<char>& newmdco, vector<int>& newmdcy);