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
#define KYLOBYTES_TO_MEGABYTES ((float) 1 / 1024)

std::list<std::string> smallFilesNames;
unsigned long long swapsCount = 0;

size_t getArrLength(int *content);

void arrayToFile(const char *fn, int *content);

int *fileToArray(const char *fn);

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
    return (result * KYLOBYTES_TO_MEGABYTES);
#endif
}

char *get_file_full_path(const char *fn) {
#ifdef _WIN32
    //    TCHAR full_path[MAX_PATH];
    //    GetFullPathName(_T("foo.dat"), MAX_PATH, full_path, NULL);
#elif __linux__
    return realpath(fn, nullptr);
#endif
}

int get_file_size(const char *fn) {
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
    smallFilesNames.emplace_back(sfn);

    for (std::string line; std::getline(in, line);) {
        if (get_file_size(sfn) * BYTES_TO_MEGABYTES > sizeLimitInMb) {
            out.close();
            cout << " (finished) " << endl;

            smallFileIndex++;
            sprintf(sfn, fnm, smallFileIndex);
            remove(sfn);
            out.open(sfn, std::ios_base::app);

            cout << "Write part: " << sfn;

            smallFilesNames.emplace_back(sfn);
        }

        out << line << endl;
    }
    out.close();
    cout << " (finished) " << endl;

    in.close();
}

void sortSmallFile(const char *fn) {
    int *content = fileToArray(fn);

    size_t size = getArrLength(content); // TODO: fix size calculation!!!
    std::qsort(content, size, sizeof *content, [](const void *a, const void *b) {
//        int arg1 = *static_cast<const int *>(a);
        int *arg1 = (int *) a;
//        int arg2 = *static_cast<const int *>(b);
        int *arg2 = (int *) b;

        cout << "Compare " << *arg1 << " and " << *arg2 << endl;

        if (*arg1 < *arg2) return -1;
        if (*arg1 > *arg2) return 1;
        return 0;
    });

    if (remove(fn) != 0)
        cerr << "Error " << fn << " file deleting!" << endl;

    arrayToFile(fn, content);

    delete[] content;
}

int *fileToArray(const char *fn) {
    const long len = get_file_size(fn);
    int *content = new int[len];
    ifstream inFile(fn);
    long tmpcounter = 0;
    for (string line; getline(inFile, line);) {
        content[tmpcounter] = atoi(line.c_str());
    }
    inFile.close();
    return content;
}

void arrayToFile(const char *fn, int *content) {
    ofstream out;
    out.open(fn, ios_base::app);

    for (long i = 0; i < getArrLength(content); ++i) {
        out << content[i] << endl;
    }

    out.close();
}

size_t getArrLength(int *content) { return sizeof(content) / sizeof(*content); }


void removePartFiles() {
    while (!smallFilesNames.empty()) {
        if (remove(smallFilesNames.back().c_str()) != 0)
            cerr << "Error " << smallFilesNames.back() << " file deleting!" << endl;
        smallFilesNames.pop_back();
    }
}

int main444(int argc, char *argv[]) {
    sortSmallFile("/home/goforbroke/rnb-10Mb.txt-0");
}

int main(int argc, char *argv[]) {
    float start_time = clock() / CLOCKS_PER_SEC;

    const char *filename = argv[1];
    const int RAMLimit = atoi(argv[2]);

    const auto initialConsumption = (int) getRAMUsage();
    cout << "Initial RAM usage: " << initialConsumption << " Mb" << endl;

    int partsSize = RAMLimit - initialConsumption - 1;
    cout << "Base parts size: " << partsSize << " Mb" << endl;

    if (getRAMUsage() >= RAMLimit) return -1;
    splitToSmallFiles(filename, partsSize);
    if (getRAMUsage() >= RAMLimit) return -1;

    // TODO: sort small files
    sortSmallFile(smallFilesNames.front().c_str());

    // TODO: check small files for bounds of ranges

    // TODO: merge small files to result file

    //removePartFiles();

    float end_time = clock() / CLOCKS_PER_SEC;
    cout << "Spent time: " << (end_time - start_time) << " seconds" << endl;
    cout << "Swaps count: " << swapsCount << endl;

    return 0;
}
