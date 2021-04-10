#ifndef BitMap_hpp
#define BitMap_hpp

#include <string.h>
#include <kernelTypes.h>

#define bitsizeof(x) (sizeof(x) * 8)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder-ctor"

template<typename T>
strict_inline T get(T *_buffer, size_t index, size_t _size){
    if (index >= _size) return UINT8_MAX;

    size_t elemIndex = index / bitsizeof(T);
    return (_buffer[elemIndex] & ((T)1 << (index % bitsizeof(T)))) > 0;
}

template<typename T>
strict_inline T get2(T *_buffer0, T *_buffer1, size_t index, size_t _size){
    if (index >= _size) return UINT8_MAX;

    size_t elemIndex = index / bitsizeof(T);
    return (((_buffer1[elemIndex] & ((T)1 << (index % bitsizeof(T)))) > 0) << 1) | ((_buffer0[elemIndex] & ((T)1 << (index % bitsizeof(T)))) > 0);
}

template<typename T>
strict_inline void set(T *_buffer, size_t index, u8 value, size_t _size){
    if (index >= _size) return;

    size_t elemIndex = index / bitsizeof(T);
    _buffer[elemIndex] &= ~((T)1 << (index % bitsizeof(T)));
    _buffer[elemIndex] |= value == 0 ? 0 : ((T)1 << (index % bitsizeof(T)));
}

template<typename T>
strict_inline void set2(T *_buffer0, T *_buffer1, size_t index, u8 value, size_t _size){
    if (index >= _size) return;

    size_t elemIndex = index / bitsizeof(T);
    auto stage1 = ~((T)1 << (index % bitsizeof(T)));
    auto stage2 = value == 0 ? 0 : ((T)1 << (index % bitsizeof(T)));

    _buffer0[elemIndex] &= stage1;
    _buffer0[elemIndex] |= stage2;

    _buffer1[elemIndex] &= stage1;
    _buffer1[elemIndex] |= stage2;
}

template<typename T>
strict_inline size_t findFree(T *_buffer, size_t _size){
    size_t i = 0;
    auto maxIndex = MAX<size_t>(_size / bitsizeof(T), 1);

    while (i < maxIndex - 1){
        if (_buffer[i] < ~((T)0)){
            T value = _buffer[i];

            for (size_t j = 0; j < bitsizeof(T); j++){
                if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
        }
        i++;
    }

    if (_buffer[i] < ~((T)0)){
        T value = _buffer[i];
        for (size_t j = 0; j < _size % bitsizeof(T); j++){
            if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }
    }

    return UINTMAX_MAX;
}

template<typename T>
class BitMap{
    size_t  _size;
    T       *_buffer;

public:
    strict_inline size_t size() const { return _size; }

public:
    void set(size_t index, u8 value) {
        ::set<T>(_buffer, index, value, _size);
    }

    u8 operator[](size_t index){
        return get<T>(_buffer, index, _size);
    }

    size_t findFree(){
        return ::findFree<T>(_buffer, _size);
    }

    strict_inline void clear(){
        memset(_buffer, 0, _size / bitsizeof(T));
    }

public:
    BitMap() = default;
    strict_inline BitMap(T *buffer, size_t size) : _buffer(buffer), _size(size) {}
};

template<typename T, size_t N>
class BitMapConst{
    T _buffer[N / bitsizeof(T)];

public:
    constexpr strict_inline size_t size() const { return N; }

public:
    void set(size_t index, u8 value) {
        ::set<T>(_buffer, index, value, N);
    }

    u8 get(size_t index){
        return ::get<T>(_buffer, index, N);
    }

    size_t findFree(){
        return ::findFree<T>(_buffer, N);
    }

    strict_inline void clear(){
        memset(_buffer, 0, N / bitsizeof(T));
    }

    strict_inline bool IsEmpty(){
        for (size_t i = 0; i < N / bitsizeof(T); i++)
            if (_buffer[i] != ~((T)0)) return false;

        return true;
    }

public:
    BitMapConst() = default;
};

template<typename T>
class BitMapDouble{
    T *_buffer0;
    T *_buffer1;
    size_t _size;

public:
    constexpr strict_inline size_t size() const { return _size; }

public:
    void set0(size_t index, u8 value) {
        ::set<T>(_buffer0, index, value, _size);
    }

    void set1(size_t index, u8 value) {
        ::set<T>(_buffer1, index, value, _size);
    }

    void set01(size_t index, u8 value) {
        ::set2<T>(_buffer0, _buffer1, index, value, _size);
    }

    u8 get0(size_t index){
       return ::get<T>(_buffer0, index, _size);
    }

    u8 get1(size_t index){
        return ::get<T>(_buffer1, index, _size);
    }

    u8 get01(size_t index){
        return ::get2(_buffer0, _buffer1, index, _size);
    }

    size_t findFree0(){
        return ::findFree<T>(_buffer0, _size);
    }

    size_t findFree1(){
        return ::findFree<T>(_buffer1, _size);
    }

    size_t findFree0Is0And1Is0(){
        size_t i = 0;
        auto maxIndex = MAX<size_t>(_size / bitsizeof(T), 1);

        while (i < maxIndex - 1){
            
            T value0 = _buffer0[i];
            T value1 = _buffer1[i];
            T value  = ~(value0 | value1);

            if (value == ~((T)0)) continue;

            for (size_t j = 0; j < bitsizeof(T); j++){
                if (value == 1) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
            
            i++;
        }

        T value0 = _buffer0[i];
        T value1 = _buffer1[i];
        T value  = (value0 | value1);

        if (value == ~((T)0)) return UINTMAX_MAX;

        for (size_t j = 0; j < _size % bitsizeof(T); j++){
            if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }

        return UINTMAX_MAX;
    }

    strict_inline void clear(){
        memset(_buffer0, 0, _size / bitsizeof(T));
        memset(_buffer1, 0, _size / bitsizeof(T));
    }

    size_t findFree1not0(){
        size_t i = 0;
        auto maxIndex = MAX<size_t>(_size / bitsizeof(T), 1);

        while (i < maxIndex - 1){
            
            T value0 = _buffer0[i];
            T value1 = _buffer1[i];
            T value  = (value0 ^ value1) & value1;

            if (value == 0) continue;

            for (size_t j = 0; j < bitsizeof(T); j++){
                if ((value & 0b1) == 1) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
            
            i++;
        }

        T value0 = _buffer0[i];
        T value1 = _buffer1[i];
        T value  = (value0 ^ value1) & value1;

        if (value == 0) return UINTMAX_MAX;

        for (size_t j = 0; j < _size % bitsizeof(T); j++){
            if ((value & 0b1) == 1) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }

        return UINTMAX_MAX;
    }

public:
    BitMapDouble() = default;
    strict_inline BitMapDouble(T *buffer, size_t size) : _buffer0(buffer), _buffer1(buffer + MAX<size_t>(size / bitsizeof(T), 1)), _size(size) {}
};

template<typename T, size_t N>
class BitMapDoubleConst{
    T _buffer0[N / bitsizeof(T)];
    T _buffer1[N / bitsizeof(T)];

public:
    u8 initialized;
    constexpr strict_inline size_t size() const { return N; }

public:
    void set0(size_t index, u8 value) {
        ::set<T>(_buffer0, index, value, N);
    }

    void set1(size_t index, u8 value) {
        ::set<T>(_buffer0, index, value, N);
    }

    void set01(size_t index, u8 value) {
        ::set2<T>(_buffer0, _buffer1, index, value, N);
    }

    u8 get0(size_t index){
        return ::get<T>(_buffer0, index, N);
    }

    u8 get1(size_t index){
        return ::get<T>(_buffer1, index, N);
    }

    u8 get01(size_t index){
        return ::get2(_buffer0, _buffer1, index, N);
    }

    size_t findFree0(){
        return ::findFree<T>(_buffer0, N);
    }

    size_t findFree1(){
        return ::findFree<T>(_buffer1, N);
    }

    size_t findFree0Is0And1Is0(){
        size_t i = 0;
        auto maxIndex = MAX<size_t>(N / bitsizeof(T), 1);

        while (i < maxIndex - 1){
            
            T value0 = _buffer0[i];
            T value1 = _buffer1[i];
            T value  = ~(value0 | value1);

            if (value == ~((T)0)) continue;

            for (size_t j = 0; j < bitsizeof(T); j++){
                if (value == 1) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
            
            i++;
        }

        T value0 = _buffer0[i];
        T value1 = _buffer1[i];
        T value  = (value0 | value1);

        if (value == ~((T)0)) return UINTMAX_MAX;

        for (size_t j = 0; j < N % bitsizeof(T); j++){
            if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }

        return UINTMAX_MAX;
    }

    size_t findFree1not0(){
        size_t i = 0;
        auto maxIndex = MAX<size_t>(N / bitsizeof(T), 1);

        while (i < maxIndex - 1){
            
            T value0 = _buffer0[i];
            T value1 = _buffer1[i];
            T value  = (value0 ^ value1) & value1;

            if (value == 0) { i++; continue; }

            for (size_t j = 0; j < bitsizeof(T); j++){
                if ((value & 0b1) == 1) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
            
            i++;
        }

        T value0 = _buffer0[i];
        T value1 = _buffer1[i];
        T value  = (value0 ^ value1) & value1;

        if (value == 0) return UINTMAX_MAX;

        for (size_t j = 0; j < N % bitsizeof(T); j++){
            if (value == 1) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }

        return UINTMAX_MAX;
    }

    size_t findFree0not1(){
        size_t i = 0;
        auto maxIndex = MAX<size_t>(N / bitsizeof(T), 1);

        while (i < maxIndex - 1){
            
            T value0 = _buffer0[i];
            T value1 = _buffer1[i];
            T value  = (value0 ^ value1) | value0;

            if (value == ~((T)0)) continue;

            for (size_t j = 0; j < bitsizeof(T); j++){
                if ((value & 0b1) == 0) return i * bitsizeof(T) + j; 
                value = value >> 1;
            }
            
            i++;
        }

        T value0 = _buffer0[i];
        T value1 = _buffer1[i];
        T value  = (value0 ^ value1) & value0;

        if (value == 0) return UINTMAX_MAX;

        for (size_t j = 0; j < N % bitsizeof(T); j++){
            if (value == 1) return i * bitsizeof(T) + j; 
            value = value >> 1;
        }

        return UINTMAX_MAX;
    }

    strict_inline void clear(){
        memset(_buffer0, 0, N / bitsizeof(T));
        memset(_buffer1, 0, N / bitsizeof(T));
    }

    strict_inline bool IsEmpty0(){

        for (size_t i = 0; i < N / bitsizeof(T); i++){
            if (_buffer0[i] != ~((T)0)) return false;
        }

        return true;
    }

    strict_inline bool IsEmpty1(){

        for (size_t i = 0; i < N / bitsizeof(T); i++){
            if (_buffer1[i] != ~((T)0)) return false;
        }

        return true;
    }

public:
    BitMapDoubleConst() = default;
};

#pragma GCC diagnostic pop

#endif /* BitMap_hpp */
