#include "low_read_stat.h"

LowReadStat::LowReadStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames)
    : descriptor(), StatCounter(_mode, _chunkSize, _filenames) {};

size_t LowReadStat::Read(char* buffer, size_t length) {
    if (descriptor.getDescriptor() == -1) {
        throw std::runtime_error("no file inited");
    }

    ssize_t readed = read(descriptor.getDescriptor(), buffer, length);
    if (readed == -1) {
        switch (errno) {
            case EINTR:
                return 0;
            default:
                throw std::runtime_error("Something wrong with file");
        }
    } else if (readed == 0) {
        throw std::runtime_error("File trunkated!");
    }

    return readed;
}

void LowReadStat::specificInitFile(const std::string& filename) {
    descriptor.newDescriptor(filename.c_str());
}


