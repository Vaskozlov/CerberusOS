#ifndef BitMap_hpp
#define BitMap_hpp

#include <kernel.h>

#define bitsizeof(x) (sizeof(x) * 8)

template<typename T>
class BitMap{
    size_t  _size;
    T       *_buffer;

public:
    inline size_t size() const { return _size; }

public:
    void set(size_t index, u8 value) {
        size_t elemIndex = index / bitsizeof(T);
        _buffer[elemIndex] &= ~((T)1 << (index % bitsizeof(T)));
        _buffer[elemIndex] |= value == 0 ? 0 : ((T)1 << (index % bitsizeof(T)));
    }

    u8 operator[](size_t index){
        size_t elemIndex = index / bitsizeof(T);
        return (_buffer[elemIndex] & ((T)1 << (index % bitsizeof(T)))) > 0;
    }

    size_t findFree(size_t startIndex){
        
        for (size_t i = startIndex / bitsizeof(T); i < _size / bitsizeof(T); i++){

            if (_buffer[i] < ~((T)0)){
                T value = _buffer[i];

                for (size_t j = 0; j < bitsizeof(T); j++){
                    if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
                    value = value >> 1;
                }
            }
        }

        for (size_t i = 0; i < startIndex / bitsizeof(T) + 1; i++){

            if (_buffer[i] < ~((T)0)){
                T value = _buffer[i];

                for (size_t j = 0; j < bitsizeof(T); j++){
                    if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
                    value = value >> 1;
                }
            }
        }

        return -1;
    }

public:
    BitMap() = default;
    inline BitMap(T *buffer, size_t size) : _buffer(buffer), _size(size) {}
};

template<class T>
class BITMap{
    size_t  size_;
    T       *buffer_;

public:
    inline size_t size() const { return size_; }
    
    void set(size_t index, u8 value){
        size_t byteIndex = index / sizeof(T);
        buffer_[byteIndex] = ~(1UL << (index % (sizeof(T) * 8)));
        
        this->buffer_[byteIndex] |= value == 1 ? (1 << (index % (sizeof(T) * 8))) : 0;
    }

public:
    u8 operator[](size_t index){
        T byte = this->buffer_[index / (sizeof(T) * 8)];
        return (byte & (1 << (index % (sizeof(T) * 8)))) > 0;
    }

    size_t getFree(size_t start){
        start -= start % sizeof(T);
    }

public:
    BITMap() = default;
    inline BITMap(T *buffer, size_t size) : buffer_(buffer), size_(size) {}
};

#endif /* BitMap_hpp */
