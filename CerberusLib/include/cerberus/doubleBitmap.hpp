#ifndef doubleBitmap_hpp
#define doubleBitmap_hpp

#include <cerberus/bitmap.hpp>
#include <iostream>

namespace cerb{

    template<typename _Tp>
    class DoubleBitmapBase: public BitmapBase<_Tp>{
    protected:
        static void DOUBLE_SET(_Tp *buffer1, _Tp *buffer2, size_t index, u8 value){
            auto arrayIndex = index / cerb::sizeofbits<_Tp>();
            auto elemIndex  = index % cerb::sizeofbits<_Tp>();

            buffer1[arrayIndex] &= ~((_Tp)1 << elemIndex);
            buffer2[arrayIndex] &= ~((_Tp)1 << elemIndex);
            
            buffer1[arrayIndex] |= (value << elemIndex);
            buffer2[arrayIndex] |= (value << elemIndex);
        }

        static size_t FIRST_IS_0_AND_SECOND_IS_0(_Tp *buffer1, _Tp *buffer2, size_t limit){
            size_t i = 0;
            auto maxIndex = cerb::MAX<size_t>(limit / cerb::sizeofbits<_Tp>(), 1);
        
            for (; i < maxIndex - 1; i++){
                _Tp value = buffer1[i] | buffer2[i];
                
                if (value < ~((_Tp)0)){
                    for (unsigned j = 0; j < cerb::sizeofbits<_Tp>(); j++){
                        if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                        value >>= 1;
                    }
                }
            }

            _Tp value = buffer1[i] | buffer2[i];
            
            if (value < ~((_Tp)0)){
                for (size_t j = 0; j < limit % cerb::sizeofbits<_Tp>(); j++){
                    if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                    value >>= 1;
                }
            }

            return UINTMAX_MAX;
        }

        static size_t FIRST_IS_1_AND_SECOND_IS_0(_Tp *buffer1, _Tp *buffer2, size_t limit){
            size_t i = 0;
            auto maxIndex = cerb::MAX<size_t>(limit / cerb::sizeofbits<_Tp>(), 1);

            for (; i < maxIndex - 1; i++){
                _Tp value = (~buffer1[i]) | buffer2[i];
                
                if (value < ~((_Tp)0)){
                    for (unsigned j = 0; j < cerb::sizeofbits<_Tp>(); j++){
                        if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                        value >>= 1;
                    }
                }
            }

            _Tp value = (~buffer1[i]) | buffer2[i];
            
            if (value < ~((_Tp)0)){
                for (size_t j = 0; j < limit % cerb::sizeofbits<_Tp>(); j++){
                    if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                    value >>= 1;
                }
            }

            return UINTMAX_MAX;
        }

        static size_t FIRST_IS_0_AND_SECOND_IS_1(_Tp *buffer1, _Tp *buffer2, size_t limit){
            size_t i = 0;
            auto maxIndex = cerb::MAX<size_t>(limit / cerb::sizeofbits<_Tp>(), 1);

            for (; i < maxIndex - 1; i++){
                _Tp value = buffer1[i] | (~buffer2[i]);
                
                if (value < ~((_Tp)0)){
                    for (unsigned j = 0; j < cerb::sizeofbits<_Tp>(); j++){
                        if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                        value >>= 1;
                    }
                }
            }

            _Tp value = buffer1[i] | (~buffer2[i]);
            
            if (value < ~((_Tp)0)){
                for (size_t j = 0; j < limit % cerb::sizeofbits<_Tp>(); j++){
                    if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                    value >>= 1;
                }
            }

            return UINTMAX_MAX;
        }
    };

    template<typename _Tp, size_t _size>
    class DoubleBitmapConst: protected DoubleBitmapBase<_Tp>{
        typedef DoubleBitmapConst<_Tp, _size> CurType;
        _Tp _data1[_size / cerb::sizeofbits<_Tp>() + 1] = {0};
        _Tp _data2[_size / cerb::sizeofbits<_Tp>() + 1] = {0};

    public:
        const _Tp *data1() const { return (_Tp*)((char*)this + offsetof(CurType, _data1)); }
        const _Tp *data2() const { return (_Tp*)((char*)this + offsetof(CurType, _data2)); }
        constexpr size_t size() const { return _size; }

    public:
        always_inline void set1(size_t index, u8 value){
            if (index >= _size) return;
            this->SET((_Tp*)data1(), index, value);
        }

        always_inline void set2(size_t index, u8 value){
            if (index >= _size) return;
            this->SET((_Tp*)data2(), index, value);
        }

        always_inline void set12(size_t index, u8 value){
            if (index >= _size) return;
            this->DOUBLE_SET((_Tp*)data1(), (_Tp*)data2(), index, value);
        }

        always_inline u8 at1(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data1(), index);
        }
        always_inline u8 at2(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data2(), index);
        }

        always_inline size_t findFree1() const {
            return this->FINDFREE((_Tp*)data1(), _size);
        }

        always_inline size_t findFree2() const {
            return this->FINDFREE((_Tp*)data2(), _size);
        }

        always_inline size_t findFree1And2() const {
            return this->FIRST_IS_0_AND_SECOND_IS_0((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline size_t findFirstFreeAndSecindSet() const {
            return this->FIRST_IS_0_AND_SECOND_IS_1((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline size_t findFirstSetAndSecindFree() const {
            return this->FIRST_IS_1_AND_SECOND_IS_0((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline bool isEmpty1() const {
            return this->IS_EMPTY((_Tp*)data1(), _size);
        }

        always_inline bool isEmpty2() const {
            return this->IS_EMPTY((_Tp*)data2(), _size);
        }

        always_inline bool isEmpty() const {
            return this->isEmpty1() && this->isEmpty2();
        }

        always_inline void clear1(){
            this->CLEAR((_Tp*)data1(), _size);
        }

        always_inline void clear2(){
            this->CLEAR((_Tp*)data2(), _size);
        }

        always_inline void clear(){
            this->clear1();
            this->clear2();
        }

    public:
        DoubleBitmapConst() = default;
        ~DoubleBitmapConst() = default;
    };

    
    template<typename _Tp>
    class DoubleBitmapFree: protected DoubleBitmapBase<_Tp>{
        typedef DoubleBitmapFree<_Tp> CurType;
        _Tp *_data1;
        _Tp *_data2;
        size_t _size;

    public:
        const _Tp *data1() const { return _data1; }
        const _Tp *data2() const { return _data2; }
        size_t size() const { return _size; }

    public:
        always_inline void set1(size_t index, u8 value){
            if (index >= _size) return;
            this->SET((_Tp*)data1(), index, value);
        }

        always_inline void set2(size_t index, u8 value){
            if (index >= _size) return;
            this->SET((_Tp*)data2(), index, value);
        }

        always_inline void set12(size_t index, u8 value){
            if (index >= _size) return;
            this->DOUBLE_SET((_Tp*)data1(), (_Tp*)data2(), index, value);
        }

        always_inline u8 at1(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data1(), index);
        }
        always_inline u8 at2(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data2(), index);
        }

        always_inline size_t findFree1() const {
            return this->FINDFREE((_Tp*)data1(), _size);
        }

        always_inline size_t findFree2() const {
            return this->FINDFREE((_Tp*)data2(), _size);
        }

        always_inline size_t findFree1And2() const {
            return this->FIRST_IS_0_AND_SECOND_IS_0((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline size_t findFirstFreeAndSecindSet() const {
            return this->FIRST_IS_0_AND_SECOND_IS_1((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline size_t findFirstSetAndSecindFree() const {
            return this->FIRST_IS_1_AND_SECOND_IS_0((_Tp*)data1(), (_Tp*)data2(), _size);
        }

        always_inline bool isEmpty1() const {
            return this->IS_EMPTY((_Tp*)data1(), _size);
        }

        always_inline bool isEmpty2() const {
            return this->IS_EMPTY((_Tp*)data2(), _size);
        }

        always_inline bool isEmpty() const {
            return this->isEmpty1() && this->isEmpty2();
        }

        always_inline void clear1(){
            this->CLEAR((_Tp*)data1(), _size);
        }

        always_inline void clear2(){
            this->CLEAR((_Tp*)data2(), _size);
        }

        always_inline void clear(){
            this->clear1();
            this->clear2();
        }

    DoubleBitmapFree &operator=(DoubleBitmapFree other) = delete;
    DoubleBitmapFree &operator=(DoubleBitmapFree &other) = delete;

    DoubleBitmapFree &operator=(DoubleBitmapFree &&other){
            _data1 = (_Tp*)other.data1();
            _data2 = (_Tp*)other.data2();
            _size = other.size();

            other._data1 = nullptr;
            other._data2 = nullptr;
            other._size  = 0;

            return *this;
        }

    public:
        DoubleBitmapFree(DoubleBitmapFree &other) = delete;
        DoubleBitmapFree(DoubleBitmapFree &&other){
            _data1 = (_Tp*)other.data1();
            _data2 = (_Tp*)other.data2();
            _size = other.size();

            other._data1 = nullptr;
            other._data2 = nullptr;
            other._size  = 0;
        }

    public:
        DoubleBitmapFree() = default;
        ~DoubleBitmapFree() = default;
        DoubleBitmapFree(_Tp *buffer1, _Tp *buffer2, size_t elems) : _data1(buffer1), _data2(buffer2), _size(elems) {}
    };
}

#endif /* doubleBitmap_hpp */
