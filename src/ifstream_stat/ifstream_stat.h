#pragma once

#include "stat_counter.h"

class IfstreamStat : public StatCounter {
public:
    IfstreamStat(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames);
    ~IfstreamStat();

private:
    size_t Read(char* buffer, size_t length) override;
    void specificInitFile(const std::string& filename) override;

private:
    std::ifstream fileStream;
    bool fileInited;
};