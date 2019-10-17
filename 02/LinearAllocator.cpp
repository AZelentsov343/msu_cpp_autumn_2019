#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) {
    buf = malloc(maxSize);
    if (!buf) {
        throw Exception("Error: can't initialize: Allocator is too big");
    }
    buf_end = buf + maxSize;
}

LinearAllocator::~LinearAllocator() {
    free(buf);
}

char* LinearAllocator::alloc(size_t size) {
    char *ret = buf_cur;
    buf_cur += size;
    if (buf_cur >= buf_end) {
        return nullptr;
    }
    return ret;
}

void LinearAllocator::reset() const {
    buf_cur = buf;
}

