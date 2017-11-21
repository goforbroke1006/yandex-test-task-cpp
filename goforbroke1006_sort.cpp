// RUN ON LINUX
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/MEGA/rand_num_base_128Mb 32

// RUN ON WINDOWS
// g++ -std=c++11 -o ./build/goforbroke1006_sort.exe goforbroke1006_sort.cpp -lpsapi && "build/goforbroke1006_sort.exe some-file.txt 32"
// cl /EHsc /nologo /W4 goforbroke1006_sort.cpp /link /out:./build/goforbroke1006_sort.exe && "build/goforbroke1006_sort.exe some-file.txt 32"

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#elif __linux__
#include "stdio.h"
#endif

#include "string.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ctime>

#include <algorithm>
#include <fstream>

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

int get_file_size(const char *fn) {
    std::ifstream in(fn, std::ifstream::ate | std::ifstream::binary);
    return (int) in.tellg();
}

int fileGetLinesCount(std::string filePath) {
    std::ifstream *inFile = new ifstream(filePath); 
    const int r = std::count(std::istreambuf_iterator<char>(*inFile), std::istreambuf_iterator<char>(), '\n');
    delete(inFile);
    return r;
}

void splitToSmallFiles(const char* bigInputFilename, int sizeLimitInMb) {
    //
    //const int lc = fileGetLinesCount(bigInputFilename);
    const unsigned long lc = 12803740;
    cout << "Main base file, lines count: " << lc << endl;

    int counter = 0;
    int smallFileIndex = 0;

    char fnm[256] = {};
    strcat(fnm, bigInputFilename);
    strcat(fnm, "-%d");
    cout << "File mask: " << fnm << endl;

    char sfn[256];
    sprintf(sfn, fnm, smallFileIndex);
    cout << "Small filename: " << sfn << endl;

//    ifstream* in = new ifstream();
//    in->open(bigInputFilename,);
//  std::ifstream in;
//  in.open (bigInputFilename, std::ifstream::in);

    ifstream in( bigInputFilename );

    cout << "Open base file before splitting > " << bigInputFilename << endl;

    fstream out(sfn);

ofstream out;
    out.open(sfn, std::ios_base::app);

    cout << "Write part: " << sfn << endl;

    //if (in.is_open()) {
        for (std::string line; std::getline(in, line); ) {
cout << (get_file_size(sfn) * BYTES_TO_MEGABYTES) << endl;

            if (get_file_size(sfn) * BYTES_TO_MEGABYTES > 1) { // 0.9f * sizeLimitInMb
                out.close();
cout << "Close part: " << sfn << endl;

                smallFileIndex++;
                sprintf(sfn, fnm, smallFileIndex);
                out = fstream(sfn);

                cout << "Write part: " << sfn << endl;
            }

            out << line << endl;
        }
    //}

    in.close();
}

int main(int argc, char *argv[]) {
    const char* filename = argv[1];
    const int RAMLimit = atoi(argv[2]);

    float start_time =  clock()/1000.0;

    while (true) {
        if (getRAMUsage() >= RAMLimit) return -1;

        //cout << "Hello, Wildfowl!!" << endl;
        splitToSmallFiles(filename, RAMLimit);
        //
        break;
    }

    float end_time = clock()/1000.0;

    cout << "start time: " << start_time << endl;
    cout << "end time: " << end_time << endl;
    cout << "spent time: " << end_time - start_time << endl;

    return 0;
}
