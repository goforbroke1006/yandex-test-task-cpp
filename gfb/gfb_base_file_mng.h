//
// Created by goforbroke on 30.11.17.
//

#ifndef YANDEX_TEST_TASK_CPP_GFB_BASE_FILE_MNG_H
#define YANDEX_TEST_TASK_CPP_GFB_BASE_FILE_MNG_H

#include <fstream>
#include <vector>
#include <algorithm>

namespace gfb {

    std::ifstream inFile;
    std::ofstream outFile;
    std::string line;
    unsigned long linesCount = 0;

    void fileToArray(const char *fn, std::vector<int> content) {
        inFile = std::ifstream(fn);
        for (; getline(inFile, line);) {
            content.push_back(atoi(line.c_str()));
        }
        inFile.close();
    }

    void arrayToFile(std::vector<int> content, const char *fn) {
        outFile = std::ofstream();
        outFile.open(fn, std::ios_base::app);

        while (!content.empty()) {
            outFile << content.back() << std::endl;
            content.pop_back();
        }

        outFile.close();
    }

    unsigned long fileGetLinesCount(const std::string &filePath) {
        inFile = std::ifstream(filePath);
        linesCount = (unsigned long) std::count(std::istreambuf_iterator<char>(inFile),
                                                std::istreambuf_iterator<char>(), '\n');
        inFile.close();
        return (linesCount);
    }

}

#endif //YANDEX_TEST_TASK_CPP_GFB_BASE_FILE_MNG_H
