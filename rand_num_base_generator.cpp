#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

const auto BYTES_TO_MEGABYTES = (float) 1 / 1024 / 1024;

int get_file_size(const char *fn) {
    std::ifstream in(fn, std::ifstream::ate | std::ifstream::binary);
    return (int) in.tellg();
}

int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    const int fileSizeLimit = atoi(argv[2]);

    int s = 0;

    ofstream myfile;
    myfile.open(filename, std::ios_base::app);
    int i=0;
    while (true) {
        s = get_file_size(filename);

        printf("       \r[%d] Mb\r", (int)(s * BYTES_TO_MEGABYTES));
        fflush(stdout);

        if (s * BYTES_TO_MEGABYTES >= fileSizeLimit) break;
        myfile << (rand()) << endl;
    }
    myfile.close();

    printf("       \r[%d] Mb (Finished)\r", (int)(s * BYTES_TO_MEGABYTES));
    fflush(stdout);

    return 0;
}
