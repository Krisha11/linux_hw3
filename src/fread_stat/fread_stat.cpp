#include "fread_stat.h"

FreadStat::FreadStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames)
    : StatCounter(_mode, _chunkSize, _filenames), fileInited(false), file(NULL) {};

FreadStat::~FreadStat() {
    if (fileInited) {
        fclose(file);
    }
}

size_t FreadStat::Read(char* buffer, size_t length) {
    size_t readed = fread(buffer, sizeof(char), length, file);

    return readed;
}

void FreadStat::specificInitFile(const std::string& filename) {
    if (fileInited) {
        fclose(file);
    }

    fileInited = true;
    file = fopen(filename.c_str(), "r");
}



