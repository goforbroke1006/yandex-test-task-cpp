// RUN ON LINUX
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/MEGA/rand_num_base_128Mb 32
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-128Mb.txt 32
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-12Mb.txt 15

// RUN ON WINDOWS
// g++ -std=c++11 -o ./build/goforbroke1006_sort.exe goforbroke1006_sort.cpp -lpsapi && "build/goforbroke1006_sort.exe some-file.txt 32"
// cl /EHsc /nologo /W4 goforbroke1006_sort.cpp /link /out:./build/goforbroke1006_sort.exe && "build/goforbroke1006_sort.exe some-file.txt 32"

#ifdef _WIN32

#include "windows.h"
#include "psapi.h"

#elif __linux__

//

#endif

#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include <fstream>
#include <list>

using namespace std;

#define KILOBYTES_TO_BYTES (1 * 1024)
#define MEGABYTES_TO_BYTES (1 * 1024 * 1024)

unsigned long long swapsCount = 0;
unsigned long long comparisonsCount = 0;

void fileToArray(const char *fn);

void arrayToFile(const char *fn);

#ifdef __linux__

int parseLine(char *line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    size_t i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    return atoi(p);
}

#endif

/**
 * Get RAM usage for current process
 * @return number in bytes
 */
unsigned long get_ram_usage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
    return (float)(pmc.WorkingSetSize + pmc.PrivateUsage);
#elif __linux__
    FILE *file = fopen("/proc/self/status", "r");
    int result = 0;
    char line[128];

    while (fgets(line, 128, file) != nullptr) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return (unsigned long) (result * KILOBYTES_TO_BYTES);
#endif
}

const unsigned long get_file_size(const char *fn) {
    std::ifstream in(fn, std::ifstream::ate | std::ifstream::binary);
    return (unsigned long) in.tellg();
}

void unlink_file(const char *filename) {
    ifstream ifs(filename);
    if (!ifs.good()) return;

    int r;
#ifdef _WIN32
    linesCount = _unlink(filename)
#elif __linux__
    char cmd[256] = {};
    strcat(cmd, "rm -f ");
    strcat(cmd, filename);
    r = system(cmd);
#endif
    if (r != 0)
        cerr << "Error " << filename << " file deleting!" << endl;
}

// ==========

std::vector<std::string> partsNames;
std::vector<std::string>::iterator nit;
std::vector<std::ifstream *> partsStreams;

unsigned long RAMLimit;
std::vector<int> content;
int a;
int b;
float RAMUsage = 0;
std::ifstream inFile;
std::ofstream outFile;
std::vector<int>::iterator it;
std::string line;
long linesCount = 0;

int smallFileIndex = 0;
char fnm[256] = {};
char sfn[256];

const int BUF_NULL = -1;

void checkRAMUsage() {
    RAMUsage = get_ram_usage();
    if (RAMUsage > RAMLimit) {
        std::cerr << "ERROR: out of memory limit - " << RAMUsage << " Mb, "
                  << "limit - " << RAMLimit << " Mb" << endl;
        std::terminate();
    }
}

unsigned long fileGetLinesCount(const std::string &filePath) {
    inFile = ifstream(filePath);
    linesCount = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');
    inFile.close();
    return static_cast<unsigned long>(linesCount);
}

void fileToArray(const char *fn) {
    inFile = ifstream(fn);
    for (; getline(inFile, line);) {
        content.push_back(atoi(line.c_str()));
    }
    inFile.close();

//    checkRAMUsage();
}

void arrayToFile(const char *fn) {
    outFile = ofstream();
    outFile.open(fn, ios_base::app);
    for (it = content.begin(); it != content.end(); ++it) {
        outFile << *it << endl;
    }
    outFile.close();

//    checkRAMUsage();
}

void splitToSmallFiles(const char *bigInputFilename, unsigned long sizeLimitInMb) {
//    cout << "Main base file, lines count: " << fileGetLinesCount(bigInputFilename) << endl;

    strcat(fnm, bigInputFilename);
    strcat(fnm, "-%d");

    sprintf(sfn, fnm, smallFileIndex);

    inFile = ifstream(bigInputFilename);
//    cout << "Open base file before splitting > " << bigInputFilename << endl;

    unlink_file(sfn);
    outFile = ofstream();
    outFile.open(sfn, std::ios_base::app);

//    cout << "Write part: " << sfn;
    partsNames.emplace_back(sfn);

    for (; std::getline(inFile, line);) {
        if (get_file_size(sfn) > sizeLimitInMb) {
            outFile.close();
//            cout << " (finished) " << endl;

            smallFileIndex++;
            sprintf(sfn, fnm, smallFileIndex);
            unlink_file(sfn);
            outFile.open(sfn, std::ios_base::app);

//            cout << "Write part: " << sfn;

            partsNames.emplace_back(sfn);
        }

        outFile << line << endl;
    }
    outFile.close();
//    cout << " (finished) " << endl;

    inFile.close();

//    checkRAMUsage();
}

void sortSmallFile(const char *fn) {
    content.clear();
    fileToArray(fn);

//    cout << "Start sort: " << fn << endl;

    qsort(&content[0], content.size(), sizeof(int), [](const void *one, const void *two) {
        a = (*(int *) one);
        b = (*(int *) two);

        comparisonsCount++;

        if (a == b) return 0;

        swapsCount++;

        if (a < b) return -1;
        return 1;
    });

//    checkRAMUsage();

    unlink_file(fn);

    arrayToFile(fn);

    content.clear();
}

unsigned long totalLines = 0;
unsigned long movedToResult = 0;
int processInPercents = 0;
char progressBar[50];

void displayMergeProcess() {
    processInPercents = (int) (((float) movedToResult / totalLines) * 100);
    for (int i = 0; i < 50; i++) {
        progressBar[i] = (i * 2 <= processInPercents) ? '-' : '.';
    }
    cout << "Merging [" << progressBar << "] [" << processInPercents << "%]" << '\r' << flush;
}

int main(int argc, char *argv[]) {
    float start_time = clock() / CLOCKS_PER_SEC;

    const char *filename = argv[1];
    RAMLimit = (unsigned long) atoi(argv[2]) * MEGABYTES_TO_BYTES;

    totalLines = fileGetLinesCount(filename);

    const auto initialConsumption = get_ram_usage();
//    cout << "Initial RAM usage: " << initialConsumption << " bytes" << endl;

    unsigned long partsSize = (RAMLimit - initialConsumption);
    if (partsSize <= 0) {
        std::cerr << "ERROR: unacceptable parts limit - " << partsSize << " bytes";
        std::terminate();
    }
//    cout << "Base parts size: " << partsSize << " bytes" << endl;

    splitToSmallFiles(filename, partsSize);

    for (nit = partsNames.begin(); nit != partsNames.end(); ++nit) {
        sortSmallFile((*nit).c_str());
//        checkRAMUsage();
    }

    for (nit = partsNames.begin(); nit != partsNames.end(); ++nit) {
        partsStreams.push_back(new ifstream((*nit).c_str()));
//        checkRAMUsage();
    }

    char resFn[256] = {};
    strcat(resFn, filename);
    strcat(resFn, "-result");
    ofstream result;
    result.open(resFn);

    std::vector<int> sortBuffer(partsNames.size());

    string line;
    unsigned i = 0;
    int minIndex = 0;
    while (!partsNames.empty()) {

        // re-fill buffer
        for (i = 0; i < sortBuffer.size(); i++) {
            if (sortBuffer[i] == BUF_NULL) {
                if (getline(*partsStreams[i], line))
                    sortBuffer[i] = atoi(line.c_str());
                else {
//                    cout << "Exhausted part: " << partsNames[i] << " (removing...)" << endl;

                    unlink_file(partsNames[i].c_str());

                    sortBuffer.erase(sortBuffer.begin() + i);
                    partsNames.erase(partsNames.begin() + i);
                    partsStreams.erase(partsStreams.begin() + i);
                }
            }
        };
//        checkRAMUsage();

        for (i = 1; i < sortBuffer.size(); i++) {
            comparisonsCount++;
            if (sortBuffer[i] != BUF_NULL && sortBuffer[i] < sortBuffer[minIndex]) minIndex = i;
        }
//        checkRAMUsage();

        if (sortBuffer[minIndex] == BUF_NULL) continue;

        result << sortBuffer[minIndex] << endl;
        sortBuffer[minIndex] = BUF_NULL;
        minIndex = 0;
        movedToResult++;

        displayMergeProcess();
    }
    cout << endl;

    result.close();

    while (!partsStreams.empty()) {
        partsStreams.back()->close();
        delete partsStreams.back();
        partsStreams.pop_back();
    }
//    checkRAMUsage();

    while (!partsNames.empty()) {
        unlink_file(partsNames.back().c_str());
        partsNames.pop_back();
    }
    checkRAMUsage();

    float end_time = clock() / CLOCKS_PER_SEC;

    cout << "Spent time: " << (end_time - start_time) << " seconds" << endl;
    cout << "Swaps count: " << swapsCount << endl;
    cout << "Comparisons count: " << comparisonsCount << endl;

    return 0;
}
