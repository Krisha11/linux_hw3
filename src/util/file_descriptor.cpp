#include "file_descriptor.h"

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

FileDescriptor::FileDescriptor() : descr(-1) {}

FileDescriptor::~FileDescriptor() {
    if (descr != -1) {
        close(descr);
    }
}


void FileDescriptor::newDescriptor(const char* fn) {
    if (descr != -1) {
        close(descr);
    }

    errno = 0;
    descr = open(fn, O_RDONLY);
    if (descr < 0) {
        std::cout << "descr:" << fn << ' ' << descr << "\n";
        throw std::runtime_error(strerror(errno));
    }
}

int FileDescriptor::getDescriptor() const {
    return descr;
}
