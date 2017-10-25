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
void getHarddriveQuantity(ifstream& fin, int& hdq);
void getLogType(ifstream& fin, int& lt);
void getLogFilepath(ifstream& fin, string& lfp);
void getLogTypeAndFilepath(ifstream& fin, string& lfp, int& lt);
void getMetadataFilepath(ifstream& fin, string& mdfp);
void getMemoryBlockSize(ifstream& fin, int& mbs);
void getPrinterQuantity(ifstream& fin, int& pq);
void getSystemMemory(ifstream& fin, int& sm, string& units);
int handleErrors(const int e);
int memoryFunction(const int sm, const int mbs, const int multiplier);
void myWait(int ms);
void printTime(high_resolution_clock::time_point t1,
               high_resolution_clock::time_point t2,
               duration<double> time_span, const int logType, ofstream& fout);
void processAndLog(map<string, int>& cycleTimes, vector<string>& mdd, vector<char>& mdco,
                   vector<int>& mdcy, const string lfp, const int lt,
                   const int count, const int sm, const int i,
                   high_resolution_clock::time_point t1, high_resolution_clock::time_point t2,
                   duration<double> time_span, ofstream& fout, PCB PCBmain,
                   const int mbs, int& mult, sem_t semaphore, const int hdq, const int pq,
                   int& hdc, int& pc);
void readConfigurationFile(ifstream& fin, map<string, int>& cycleTimes,
                           string& mdfp, string& lfp, int& lt, int& sm, string& units,
                           int& mbs, int& hdq, int& pq);
void readOneMeta(ifstream& fin, vector<string>& mdd, vector<char>& mdc,
                 vector<int>& cycles);
void readMetadataFile(ifstream& fin, vector<string>& mdd, vector<char>& mdc,
                      vector<int>& cycles, int& count);
void* runner(void* total);
