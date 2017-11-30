//
// Created by goforbroke on 30.11.17.
//

#ifndef YANDEX_TEST_TASK_CPP_GFB_FILE_H
#define YANDEX_TEST_TASK_CPP_GFB_FILE_H

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

namespace gfb {
    std::ifstream in;
    unsigned long file_size = 0;

    const unsigned long get_file_size(const char *fn) {
        in = std::ifstream(fn, std::ifstream::ate | std::ifstream::binary);
        file_size = (unsigned long) in.tellg();
        in.close();
        return file_size;
    }

    void unlink_file(const char *filename) {
        in = std::ifstream(filename);
        if (!in.good()) return;

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
            std::cerr << "Error " << filename << " file deleting!" << std::endl;
    }
}

#endif //YANDEX_TEST_TASK_CPP_GFB_FILE_H
