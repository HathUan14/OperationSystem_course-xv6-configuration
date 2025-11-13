#ifndef SYSINFO_H
#define SYSINFO_H

#include "types.h"
struct sysinfo {
    uint64 freemem; // Dung lượng bộ nhớ trống (bytes)
    uint64 nproc; //Số tiến trình hoạt động
};

#endif