#include "low_read_stat.h"
#include "ifstream_stat.h"
#include "fread_stat.h"
#include "mmap_stat.h"
#include "stat_counter.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <unordered_set>
#include <unordered_map>
#include <functional>

const std::unordered_map<std::string, StatCounter::Mode> possibleModes = {
    {"OneByte", StatCounter::Mode::OneByte},
    {"Chunk", StatCounter::Mode::Chunk},
    {"Full", StatCounter::Mode::FullFile}
};

const std::unordered_map<std::string, std::function<void(const StatCounter::Mode, const size_t, const std::vector<std::string>&)>> statCounters = { 
    {"Low", [](const StatCounter::Mode mode, const size_t chunkSize, const std::vector<std::string>& filenames){
        LowReadStat(mode, chunkSize, filenames).CountAndPrintStat();
    }},
    {"Ifstream", [](const StatCounter::Mode mode, const size_t chunkSize, const std::vector<std::string>& filenames){
        IfstreamStat(mode, chunkSize, filenames).CountAndPrintStat();
    }},
    {"Fread", [](const StatCounter::Mode mode, const size_t chunkSize, const std::vector<std::string>& filenames){
        FreadStat(mode, chunkSize, filenames).CountAndPrintStat();
    }},
    {"Mmap", [](const StatCounter::Mode mode, const size_t chunkSize, const std::vector<std::string>& filenames){
        MmapStat(mode, chunkSize, filenames).CountAndPrintStat();
    }}
};

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: stat <Mode: OneByte/Chunk/Full> <Chunk Size: chunk size if Chunk mode 0 else> <Read Mode : Low/Ifstream/Mmap > ... filenames ...";
        return 0;
    }

    if (!possibleModes.contains(argv[1])) {
        std::cerr << "No such mode\n";
        return 0;
    }
    StatCounter::Mode mode = possibleModes.at(argv[1]);

    size_t chunkSize;
    sscanf(argv[2], "%zu", &chunkSize);
    if (mode == StatCounter::Mode::OneByte) {
        chunkSize = 1;
    }

    std::string readMode = argv[3];
    if (!statCounters.contains(readMode)) {
        std::cerr << "No such read mode\n";
        return 0;
    }

    std::vector<std::string> filenames;
    for (int i = 4; i < argc; i++) {
        filenames.push_back(argv[i]);
    }

    statCounters.at(readMode)(mode, chunkSize, filenames);

    return 0;
}
