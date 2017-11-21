// RUN ON LINUX
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort ~/some-file 32

// RUN ON WINDOWS
// g++ -std=c++11 -o ./build/goforbroke1006_sort.exe goforbroke1006_sort.cpp -lpsapi && "build/goforbroke1006_sort.exe some-file.txt 32"
// cl /EHsc /nologo /W4 goforbroke1006_sort.cpp /link /out:./build/goforbroke1006_sort.exe && "build/goforbroke1006_sort.exe some-file.txt 32"

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#elif __linux__
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#endif

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ctime>

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

int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    const int RAMLimit = atoi(argv[2]);

    float start_time =  clock()/1000.0;

    while (true) {
        if (getRAMUsage() >= RAMLimit) return -1;

        cout << "Hello, Wildfowl!!" << endl;
        //
        break;
    }

    float end_time = clock()/1000.0;

    cout << "start time: " << start_time << endl;
    cout << "end time: " << end_time << endl;
    cout << "spent time: " << end_time - start_time << endl;

    return 0;
}
