#pragma once

#include "file_descriptor.h"

#include <cstdint>
#include <array>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


class StatCounter {
public:
    enum Mode {
        OneByte,
        Chunk,
        FullFile
    };

    StatCounter(const StatCounter::Mode _mode, const size_t _chunkSize, const std::vector<std::string>& _filenames)
        : mode(_mode), chunkSize(_chunkSize), filenames(_filenames), curFileNum(-1) {
        statistic.fill(0);
    }

    void CountAndPrintStat() {
        uint32_t start = clock();
        CountStat();
        uint32_t end = clock();

        PrintStat(end - start);
    }

    static size_t getFilesize(const char* filename) {
        struct stat st;
        if(stat(filename, &st) != 0) {
            return 0;
        }
        return st.st_size;   
    }

protected:
    bool initNextFile() {
        curFileNum++;
        if (curFileNum == filenames.size()) {
            return false;
        }

        curFileSize = getFilesize(filenames[curFileNum].c_str());
        if (mode == Mode::FullFile) {
            curChunkSize = curFileSize;
        } else {
            curChunkSize = chunkSize;
        }

        specificInitFile(filenames[curFileNum]);

        return true;
    }

    virtual size_t Read(char* buffer, size_t length) = 0;

    virtual void specificInitFile(const std::string& filename) = 0;

private:
    void CountStat() {
        while (initNextFile()) {
            size_t current_size = 0;

            std::vector<char> data(curChunkSize);
            while(current_size < curFileSize) {
                size_t want_to_read = std::min(curChunkSize, curFileSize - current_size);
                size_t readed = Read(data.data(), want_to_read);

                for (int i = 0; i < readed; i++) {
                    statistic[(uint8_t)data[i]]++;
                }

                current_size += readed;
            }
        }
    }

    void PrintStat(uint32_t time) {
        for (int i = 0; i < 256; i++) {
            std::cout << statistic[i] << ' ';
        }
        std::cout << "\n";

        std::cout << "Time: " << ((float)time)/CLOCKS_PER_SEC << " s\n";
    }


protected:
    const StatCounter::Mode mode;
    const size_t chunkSize;

    size_t curFileSize;
    size_t curChunkSize;

private:
    const std::vector<std::string> filenames;
    std::array<uint64_t, 256> statistic;

    int curFileNum;
};