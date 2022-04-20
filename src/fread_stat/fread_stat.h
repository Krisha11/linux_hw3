#pragma once

#include "stat_counter.h"

class FreadStat : public StatCounter {
public:
    FreadStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames);
    ~FreadStat();

private:
    size_t Read(char* buffer, size_t length) override;
    void specificInitFile(const std::string& filename) override;

private:
    FILE* file;
    bool fileInited;
};