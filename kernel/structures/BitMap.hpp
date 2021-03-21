#ifndef BitMap_hpp
#define BitMap_hpp

#include <kernel.h>

class BitMap{

    size_t  size_;
    u8      *buffer;

public:
    inline size_t size() const { return size_; }

public:
    u8 operator[](size_t index);
    void set(size_t index, u8 value);

public:
    BitMap() = default;
    inline BitMap(u8 *Buffer, size_t Size) : size_(Size), buffer(Buffer) {}
};

#endif /* BitMap_hpp */
