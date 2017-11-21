// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// http://qaru.site/questions/13923/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

// RUN ON LINUX
// g++ -std=c++11 -o ./build/check_mem_usage check_mem_usage.cpp && ./build/check_mem_usage

// RUN ON WINDOWS
// g++ -std=c++11 -o ./build/check_mem_usage.exe check_mem_usage.cpp -lpsapi && "build/check_mem_usage.exe"
// cl /EHsc /nologo /W4 check_mem_usage.cpp /link /out:./build/check_mem_usage.exe

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#elif __linux__
#include "stdio.h"
#endif

#include "stdlib.h"
#include "string.h"
#include <iostream>
<<<<<<< HEAD
#include <iomanip>
#include <chrono>
=======
#include <iomanip>
#include <ctime>
>>>>>>> 0382e8df3fc85c006ee5f70ff71f783f597e9031

using namespace std;

#define BYTES_TO_MEGABYTES ((float) 1 / 1024 / 1024)
#define KYLOBYTES_TO_MEGABYTES ((float) 1 / 1024)

int parseLine(char* line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

float getRAMUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
    return (float)(pmc.WorkingSetSize + pmc.PrivateUsage) * BYTES_TO_MEGABYTES;
#elif __linux__
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return (result * KYLOBYTES_TO_MEGABYTES);
#endif
}

<<<<<<< HEAD
int main() {
    auto begin_time =  chrono::high_resolution_clock::now();
=======
int main() {
    float start_time =  clock()/1000.0;
>>>>>>> 0382e8df3fc85c006ee5f70ff71f783f597e9031
    unsigned long l = 100000;
    int* r = new int[l];
    for (unsigned long i = 0; i < l; ++i) {
        r[i] = rand();
    }
<<<<<<< HEAD
    unsigned long mas[l];
    for (unsigned long k=0; k < l; ++k) {
        mas[k] = rand();
    }
    //cout << getValue() << " Mb" << endl;
    cout << std::fixed << setw(11) << setprecision(6) << getValue() << " Mb" << endl;

    auto end_time = chrono::high_resolution_clock::now();

    cout << "spent time: " << chrono::duration_cast<chrono::milliseconds>(end_time-begin_time).count()<< "ms" << endl;
=======

    //cout << getRAMUsage() << " Mb" << endl;
    cout << std::fixed << setw(11) << setprecision(6) << getRAMUsage() << " Mb" << endl;

    float end_time = clock()/1000.0;

    cout << "start time: " << start_time << endl;
    cout << "end time: " << end_time << endl;
    cout << "spent time: " << end_time - start_time << endl;
>>>>>>> 0382e8df3fc85c006ee5f70ff71f783f597e9031

    delete(r);

    return 0;
}
