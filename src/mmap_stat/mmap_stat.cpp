#include "mmap_stat.h"

#include <sys/mman.h>

MmapStat::MmapStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames)
    : descriptor(), StatCounter(_mode, _chunkSize, _filenames), nextFileStartAddr(0), read(0) {
};

size_t MmapStat::Read(char* buffer, size_t length) {
    std::copy(castedData + read, castedData + read + length, buffer);
    read += length;
    return length;
}

void MmapStat::specificInitFile(const std::string& filename) {
    descriptor.newDescriptor(filename.c_str());
    const void *fileData = mmap(const_cast<void*>(nextFileStartAddr), curFileSize, PROT_READ, MAP_PRIVATE, descriptor.getDescriptor(), 0);

    if (fileData == MAP_FAILED) {
        throw std::runtime_error("problems with mmap");
    }

    read = 0;
    castedData = reinterpret_cast<const char*>(fileData);
    nextFileStartAddr = castedData + curFileSize;
}


