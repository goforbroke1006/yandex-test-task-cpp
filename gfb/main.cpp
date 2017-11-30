// RUN ON LINUX
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/MEGA/rand_num_base_128Mb 32
// g++ -std=c++11 -pthread -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-10Mb.txt 15
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-128Mb.txt 32
// g++ -std=c++11 -o ./build/goforbroke1006_sort goforbroke1006_sort.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-12Mb.txt 15


// g++ -std=c++11 -pthread -o ./build/goforbroke1006_sort goforbroke1006_sort/main.cpp && ./build/goforbroke1006_sort /home/goforbroke/rnb-10Mb.txt 15

// RUN ON WINDOWS
// g++ -std=c++11 -o ./build/goforbroke1006_sort.exe goforbroke1006_sort.cpp -lpsapi && "build/goforbroke1006_sort.exe some-file.txt 32"
// cl /EHsc /nologo /W4 goforbroke1006_sort.cpp /link /out:./build/goforbroke1006_sort.exe && "build/goforbroke1006_sort.exe some-file.txt 32"

//#include <ctime>
//#include <cstdlib>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <thread>
//#include "gfb_measure.h"
//#include "gfb_ram_control.h"
//#include "gfb_file.h"
//#include "gfb_base_file_mng.h"

#include <cstdio>
#include <cstdlib>
#include <thread>
#include "gfb_ram_control.h"

using namespace std;

//unsigned long long swapsCount = 0;
//unsigned long long comparisonsCount = 0;

//std::vector<std::string> partsNames;
//std::vector<std::string>::iterator nit;
//std::vector<std::ifstream *> partsStreams;

unsigned long RAMLimit;
//std::vector<int> content;
//int a;
//int b;
//float RAMUsage = 0;
//std::ifstream inFile;
//std::ofstream outFile;
//std::vector<int>::iterator it;
//std::string line;
//long linesCount = 0;

//int smallFileIndex = 0;
//char fnm[256] = {};
//char sfn[256];

//unsigned long totalLines = 0;
//unsigned long splitToParts = 0;
//unsigned long sortingParts = 0;
//unsigned long movedToResult = 0;

//const int BUF_NULL = -1;

/**
 * Split bi file to small parts, which can get into RAM
 * @param bigInputFilename - full path to numbers file-base
 * @param sizeLimit - bytes count
 */
/*void splitToSmallFiles(const char *bigInputFilename, unsigned long sizeLimit) {
//    cout << "Main base file, lines count: " << fileGetLinesCount(bigInputFilename) << endl;

    strcat(fnm, bigInputFilename);
    strcat(fnm, "-%d");

    sprintf(sfn, fnm, smallFileIndex);

    inFile = ifstream(bigInputFilename);
//    cout << "Open base file before splitting > " << bigInputFilename << endl;

    gfb::unlink_file(sfn);
    outFile = ofstream();
    outFile.open(sfn, std::ios_base::app);

//    cout << "Write part: " << sfn;
    partsNames.emplace_back(sfn);

    for (; std::getline(inFile, line);) {
        if (gfb::get_file_size(sfn) > sizeLimit) {
            outFile.close();
//            cout << " (finished) " << endl;

            smallFileIndex++;
            sprintf(sfn, fnm, smallFileIndex);
            gfb::unlink_file(sfn);
            outFile.open(sfn, std::ios_base::app);

//            cout << "Write part: " << sfn;

            partsNames.emplace_back(sfn);
        }

        outFile << line << endl;

        splitToParts++;
    }
    outFile.close();
//    cout << " (finished) " << endl;

    inFile.close();
}*/

/*void sortSmallFile(const char *fn) {
    content.clear();
    gfb::fileToArray(fn, content);
    qsort(&content[0], content.size(), sizeof(int), [](const void *one, const void *two) {
        a = (*(int *) one);
        b = (*(int *) two);
        comparisonsCount++;
        if (a == b) return 0;
        swapsCount++;
        if (a < b) return -1;
        return 1;
    });
    gfb::unlink_file(fn);
    gfb::arrayToFile(content, fn);
    content.clear();
}*/


void displayMergeProcess(
        unsigned long t,
        int nc,
        unsigned long sp,
        unsigned long sr,
        unsigned long mv
) {
//    int pip = 0;
//    char bar[50];
//    int i = 0;
    while (true) {
        /*if (mv > 0 && mv <= t) {
            pip = (int) (((double) mv / t) * 100);
            for (i = 0; i < 50; i++) {
                bar[i] = (i * 2 <= pip) ? '-' : '.';
            }
            cout << "Merging [" << bar << "] [" << pip << "%]" << '\r' << flush;
        } else if (0 == mv && sr > 0 && sr <= nc) {
            pip = (int) (((double) sr / nc) * 100);
            for (i = 0; i < 50; i++) {
                bar[i] = (i * 2 <= pip) ? '-' : '.';
            }
            cout << "Sort [" << bar << "] [" << pip << "%]" << '\r' << flush;
        } else if (0 == sr && sp > 0 && sp <= t) {
            pip = (int) (((double) sp / t) * 100);
            for (i = 0; i < 50; i++) {
                bar[i] = (i * 2 <= pip) ? '-' : '.';
            }
            cout << "Splitting [" << bar << "] [" << pip << "%]" << '\r' << flush;
        }*/

//        usleep(0.25d * gfb::SECONDS_TO_MILLISECONDS);

//        if (t > 0 && mv >= t) break;
    }
}

//std::thread threadCheckRAM;
std::thread threadDisplay;

int main(int argc, char *argv[]) {
//    float start_time = clock() / CLOCKS_PER_SEC;

//    const char *filename = argv[1];
    RAMLimit = (unsigned long) atoi(argv[2]) * 1024 * 1024;

//    threadCheckRAM = std::thread(gfb::checkRAMUsage, RAMLimit);
//    threadCheckRAM.detach();
//
//    threadDisplay = std::thread(displayMergeProcess, totalLines, 5, splitToParts, sortingParts, movedToResult);
    threadDisplay = std::thread(displayMergeProcess, 0, 5, 0, 0, 0);
    threadDisplay.detach();

//    totalLines = fileGetLinesCount(filename);

    const auto initialConsumption = gfb::get_ram_usage();
    cout << "Initial RAM usage: " << initialConsumption << " bytes, "
         << ((double) initialConsumption / 1024 / 1024) << " Mb"
         << endl;

    /*if (RAMLimit < initialConsumption) {
        std::cerr << "ERROR: unacceptable parts limit: "
                  << (int) ((RAMLimit - initialConsumption) / 1024 / 1024) << " Mb" << endl;
        std::terminate();
    }

    return 0;

    unsigned long partsSize = (RAMLimit - initialConsumption);
    cout << "Base parts size: " << partsSize << " bytes" << endl;

    splitToSmallFiles(filename, partsSize);

    for (nit = partsNames.begin(); nit != partsNames.end(); ++nit) {
        sortSmallFile((*nit).c_str());
        sortingParts++;
    }

    for (nit = partsNames.begin(); nit != partsNames.end(); ++nit) {
        partsStreams.push_back(new ifstream((*nit).c_str()));
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

                    gfb::unlink_file(partsNames[i].c_str());

                    sortBuffer.erase(sortBuffer.begin() + i);
                    partsNames.erase(partsNames.begin() + i);
                    partsStreams.erase(partsStreams.begin() + i);
                }
            }
        };

        for (i = 1; i < sortBuffer.size(); i++) {
            comparisonsCount++;
            if (sortBuffer[i] != BUF_NULL && sortBuffer[i] < sortBuffer[minIndex]) minIndex = i;
        }

        if (sortBuffer[minIndex] == BUF_NULL) continue;

        result << sortBuffer[minIndex] << endl;
        sortBuffer[minIndex] = BUF_NULL;
        minIndex = 0;
        movedToResult++;
    }
    cout << endl;

    result.close();

    while (!partsStreams.empty()) {
        partsStreams.back()->close();
        delete partsStreams.back();
        partsStreams.pop_back();
    }

    while (!partsNames.empty()) {
        gfb::unlink_file(partsNames.back().c_str());
        partsNames.pop_back();
    }

    float end_time = clock() / CLOCKS_PER_SEC;

    cout << "Spent time: " << (end_time - start_time) << " seconds" << endl;
    cout << "Swaps count: " << swapsCount << endl;
    cout << "Comparisons count: " << comparisonsCount << endl;*/

    return 0;
}