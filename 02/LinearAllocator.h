#ifndef INC_02_LINEARALLOCATOR_H
#define INC_02_LINEARALLOCATOR_H

#include <cstdlib>

class LinearAllocator {
public:
    LinearAllocator(size_t maxSize);

    ~LinearAllocator();

    char *alloc(size_t size) const;

    void reset() const;

private:
    char *buf;
    char *buf_end;
    mutable char *buf_cur;
};


#endif //INC_02_LINEARALLOCATOR_H
