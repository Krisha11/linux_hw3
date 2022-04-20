#pragma once

#include <unistd.h>

class FileDescriptor {
public:
    FileDescriptor();
    ~FileDescriptor();
    
    void newDescriptor(const char* fn);
    int getDescriptor() const;

private:
    int descr;
};
