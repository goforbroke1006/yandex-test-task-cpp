// RUN ON LINUX
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/MEGA/rand_num_base_128Mb 32

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

#define BYTES_TO_MEGABYTES ((float) 1 / 1024 / 1024)
#define KILOBYTES_TO_MEGABYTES ((float) 1 / 1024)

std::vector<std::string> partsNames;
std::vector<std::ifstream*> partsStreams;

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

float getRAMUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc));
    return (float)(pmc.WorkingSetSize + pmc.PrivateUsage) * BYTES_TO_MEGABYTES;
#elif __linux__
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != nullptr) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return (result * KILOBYTES_TO_MEGABYTES);
#endif
}

int RAMLimit;

void checkRAMUsage() {
    float usage = getRAMUsage();
    if (usage > RAMLimit) {
        std::cerr << "ERROR: out of memory limit - " << usage << " Mb, "
                  << "limit - " << RAMLimit << " Mb" << endl;
        std::terminate();
    }
}

const int get_file_size(const char *fn) {
    std::ifstream in(fn, std::ifstream::ate | std::ifstream::binary);
    return (int) in.tellg();
}

unsigned long fileGetLinesCount(const std::string &filePath) {
    auto *inFile = new ifstream(filePath);
    long r = std::count(std::istreambuf_iterator<char>(*inFile), std::istreambuf_iterator<char>(), '\n');
    delete (inFile);
    return static_cast<unsigned long>(r);
}

void splitToSmallFiles(const char *bigInputFilename, int sizeLimitInMb) {
    const unsigned long lc = fileGetLinesCount(bigInputFilename);
    cout << "Main base file, lines count: " << lc << endl;

    int smallFileIndex = 0;

    char fnm[256] = {};
    strcat(fnm, bigInputFilename);
    strcat(fnm, "-%d");

    char sfn[256];
    sprintf(sfn, fnm, smallFileIndex);

    ifstream in(bigInputFilename);
    cout << "Open base file before splitting > " << bigInputFilename << endl;

    remove(sfn);
    ofstream out;
    out.open(sfn, std::ios_base::app);

    cout << "Write part: " << sfn;
    partsNames.emplace_back(sfn);

    for (std::string line; std::getline(in, line);) {
        if (get_file_size(sfn) * BYTES_TO_MEGABYTES > sizeLimitInMb) {
            out.close();
            cout << " (finished) " << endl;

            smallFileIndex++;
            sprintf(sfn, fnm, smallFileIndex);
            remove(sfn);
            out.open(sfn, std::ios_base::app);

            cout << "Write part: " << sfn;

            partsNames.emplace_back(sfn);
        }

        out << line << endl;
    }
    out.close();
    cout << " (finished) " << endl;

    in.close();
}

std::vector<int> content;
int a;
int b;

void sortSmallFile(const char *fn) {
    content.clear();
    fileToArray(fn);

    cout << "Start sort: " << fn << endl;

    qsort(&content[0], content.size(), sizeof(int), [](const void *one, const void *two) {
        a = (*(int *) one);
        b = (*(int *) two);

        comparisonsCount++;

        if (a == b)
            return 0;

        swapsCount++;

        if (a < b) return -1;
        return 1;
    });

    checkRAMUsage();

    if (remove(fn) != 0)
        cerr << "Error " << fn << " file deleting!" << endl;

    arrayToFile(fn);

    content.clear();
}

void fileToArray(const char *fn) {
    ifstream inFile(fn);
    for (string line; getline(inFile, line);) {
        content.push_back(
                atoi(line.c_str())
        );
    }
    inFile.close();
    checkRAMUsage();
}

void arrayToFile(const char *fn) {
    ofstream out;
    out.open(fn, ios_base::app);
    for (auto it = content.begin(); it != content.end(); ++it) {
        out << *it << endl;
    }
    out.close();
    checkRAMUsage();
}

int main(int argc, char *argv[]) {
    float start_time = clock() / CLOCKS_PER_SEC;

    const char *filename = argv[1];
    RAMLimit = atoi(argv[2]);

    const auto initialConsumption = (int) getRAMUsage();
    cout << "Initial RAM usage: " << initialConsumption << " Mb" << endl;

    int partsSize = (RAMLimit - initialConsumption) / 2;
    if (partsSize <= 0) {
        std::cerr << "ERROR: unacceptable parts limit - " << partsSize << " Mb";
        std::terminate();
    }
    cout << "Base parts size: " << partsSize << " Mb" << endl;

    splitToSmallFiles(filename, partsSize);

    for (auto it = partsNames.begin(); it != partsNames.end(); ++it) {
        sortSmallFile((*it).c_str());
    }

    for (auto it = partsNames.begin(); it != partsNames.end(); ++it) {
        partsStreams.push_back(new ifstream((*it).c_str()));
    }

    char resFn[256] = {};
    strcat(resFn, filename);
    strcat(resFn, "-result");
    ofstream result;
    result.open(resFn);

    std::vector<int> sortBuffer(partsNames.size());

    string line;
    int finishedFilesCounter = 0;
    while (partsNames.size() > finishedFilesCounter) {

        // re-fill buffer
        for (unsigned i = 0; i < sortBuffer.size(); i++) {
            if (sortBuffer[i] == 0) {
                if (getline(*partsStreams[i], line))
                    sortBuffer[i] = atoi(line.c_str());
                else {
                    if (remove(partsNames[i].c_str()) != 0)
                        cerr << "Error " << partsNames[i] << " file deleting!" << endl;

                    sortBuffer.erase(sortBuffer.begin() + i);
                    partsNames.erase(partsNames.begin() + i);
                    partsStreams.erase(partsStreams.begin() + i);

                    finishedFilesCounter++;
                }

            }
        };

        int minIndex = 0;
        for (unsigned i = 1; i < sortBuffer.size(); i++) {
            comparisonsCount++;
            if (sortBuffer[i] < sortBuffer[minIndex]) minIndex = i;
        }

        result << sortBuffer[minIndex] << endl;
        sortBuffer[minIndex] = 0;
    }

    result.close();

    while (!partsStreams.empty()) {
        partsStreams.back()->close();
        delete partsStreams.back();
        partsStreams.pop_back();
    }

    while (!partsNames.empty()) {
        if (remove(partsNames.back().c_str()) != 0)
            cerr << "Error " << partsNames.back() << " file deleting!" << endl;
        partsNames.pop_back();
    }

    float end_time = clock() / CLOCKS_PER_SEC;

    cout << "Spent time: " << (end_time - start_time) << " seconds" << endl;
    cout << "Swaps count: " << swapsCount << endl;
    cout << "Comparisons count: " << comparisonsCount << endl;

    return 0;
}
