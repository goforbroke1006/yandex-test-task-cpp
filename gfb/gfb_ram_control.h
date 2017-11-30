//
// Created by goforbroke on 30.11.17.
//

#ifndef YANDEX_TEST_TASK_CPP_GFB_RAM_CONTROL_H
#define YANDEX_TEST_TASK_CPP_GFB_RAM_CONTROL_H

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

#include "gfb_measure.h"

namespace gfb {

#ifdef __linux__
    FILE *ram_usage_file;
    int ram_usage_result = 0;
    char ram_usage_line[128];

    size_t i;
    const char *p;

    int parseLine(char *line) {
        // This assumes that a digit will be found and the line ends in " Kb".
        i = strlen(line);
        p = line;
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
        ram_usage_file = fopen("/proc/self/status", "r");
        ram_usage_result = 0;

        while (fgets(ram_usage_line, 128, ram_usage_file) != nullptr) {
            if (strncmp(ram_usage_line, "VmSize:", 7) == 0) {
                ram_usage_result = parseLine(ram_usage_line);
                break;
            }
        }
        fclose(ram_usage_file);
//        delete ram_usage_file;
        return (unsigned long) (ram_usage_result * 1024);
#endif
    }

    float RAMUsage = 0;

    void checkRAMUsage(unsigned long RAMLimit) {
        while (true) {
            RAMUsage = get_ram_usage();
            if (RAMUsage > RAMLimit) {
                std::cerr << "ERROR: out of memory limit - "
                          << (RAMUsage * BYTES_TO_MEGABYTES) << " Mb, "
                          << "limit - " << (RAMLimit * BYTES_TO_MEGABYTES)
                          << " Mb"
                          << std::endl;
                std::terminate();
                break;
            }
            usleep(0.25d * SECONDS_TO_MILLISECONDS);
        }
    }

}

#endif //YANDEX_TEST_TASK_CPP_GFB_RAM_CONTROL_H
