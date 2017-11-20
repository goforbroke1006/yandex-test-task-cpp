// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
// g++ -std=c++11 -o ./build/check_mem_usage check_mem_usage.cpp && ./build/check_mem_usage

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#elif __linux__
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#endif

#include <iostream>
#include <iomanip>

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

float getValue() {
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

int main() {
    unsigned long l = 100000;
    int* r = new int[l];
    for (unsigned long i = 0; i < l; ++i) {
        r[i] = rand();
    }

    //cout << getValue() << " Mb" << endl;
    cout << std::fixed << setw(11) << setprecision(6) << getValue() << " Mb" << endl;

    delete(r);

    return 0;
}
