#include "ifstream_stat.h"

IfstreamStat::IfstreamStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames)
    : StatCounter(_mode, _chunkSize, _filenames), fileInited(false) {};

IfstreamStat::~IfstreamStat() {
    if (fileInited) {
        fileStream.close();
    }
}

size_t IfstreamStat::Read(char* buffer, size_t length) {
    fileStream.read(buffer, length);

    if (!fileStream) {
        throw std::runtime_error("File trunkated!");

    }

    return length;
}

void IfstreamStat::specificInitFile(const std::string& filename) {
    if (fileInited) {
        fileStream.close();
    }

    fileInited = true;
    fileStream.open(filename);
}



