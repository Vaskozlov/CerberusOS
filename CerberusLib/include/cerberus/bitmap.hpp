#ifndef bitmap_hpp
#define bitmap_hpp

#include <cerberus/move.hpp>
#include <cerberus/cerberusc++.hpp>

#ifndef offsetof
#define offsetof(t, d) __builtin_offsetof(t, d)
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder-ctor"

namespace cerb{

    template<typename _Tp>
    class BitmapBase{
    protected:
        static void SET(_Tp *buffer, size_t index, u8 value){
            auto arrayIndex = index / cerb::sizeofbits<_Tp>();
            auto elemIndex  = index % cerb::sizeofbits<_Tp>();

            buffer[arrayIndex] &= ~((_Tp)1 << elemIndex);
            buffer[arrayIndex] |= (value << elemIndex);
        }

        static u8 AT(_Tp *buffer, size_t index) {
            auto arrayIndex = index / cerb::sizeofbits<_Tp>();
            auto elemIndex  = index % cerb::sizeofbits<_Tp>();

            return (buffer[arrayIndex] & ((_Tp)1 << elemIndex)) != 0;
        }

        static size_t FINDFREE(_Tp *buffer, size_t limit){
            size_t i = 0;
            auto maxIndex = cerb::MAX<size_t>(limit / cerb::sizeofbits<_Tp>(), 1);
            
            for (; i < maxIndex - 1; i++){
                if (buffer[i] < ~((_Tp)0)){
                    _Tp value = buffer[i];

                    for (unsigned j = 0; j < cerb::sizeofbits<_Tp>(); j++){
                        if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                        value >>= 1;
                    }
                }
            }
            
            if (buffer[i] < ~((_Tp)0)){
                _Tp value = buffer[i];
                
                for (size_t j = 0; j < limit % cerb::sizeofbits<_Tp>(); j++){
                    if ((value & (_Tp)0b1) == 0) return i * cerb::sizeofbits<_Tp>() + j;
                    value >>= 1;
                }
            }
            return UINTMAX_MAX;
        }

        static void CLEAR(_Tp *buffer, size_t limit){
            for (size_t i = 0; i < cerb::MAX<size_t>(limit / cerb::sizeofbits<_Tp>(), 1); i++)
                buffer[i] = static_cast<_Tp>(0);
        }

        static bool ISEMPTY(const _Tp *buffer, size_t limit){
            for (size_t i = 0; i < limit / cerb::sizeofbits<_Tp>(); i++)
                if (buffer[i] > (_Tp)0) return false;
            return true;
        }

    };

    template<typename _Tp>
    class BitmapFree: public BitmapBase<_Tp>{
        size_t  _size;
        _Tp     *_data;
        static_assert(std::is_unsigned<_Tp>::value, "Bitmap can hold only unsigned values");
    
    public:
        always_inline size_t size() const { return _size; }
        always_inline const _Tp   *data() const { return _data; }

    public:
        always_inline void set(size_t index, u8 value){
            if (index >= _size) return;
            this->SET(_data, index, value);
        }

        always_inline u8 at(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT(_data, index);
        }

        always_inline u8 operator[](size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT(_data, index);
        }

        always_inline size_t findFree() const {
            return this->FINDFREE(_data, _size);
        }

        always_inline bool isEmpty() const {
            return this->IS_EMPTY(_data, _size);
        }

        always_inline void clear(){
            this->CLEAR((_Tp*)data(), _size);
        }

    public:
        BitmapFree &operator=(BitmapFree<_Tp> other)        = delete;
        BitmapFree &operator=(const BitmapFree<_Tp> &other) = delete;
        
        BitmapFree &operator=(BitmapFree<_Tp> &&other){
            _data = other._data;
            _size = other._size;

            other._data = nullptr;
            other._size = 0;
            
            return *this;
        }

    public:
        BitmapFree(BitmapFree<_Tp> &other) = delete;
        BitmapFree(BitmapFree<_Tp> &&other) : _data(other._data), _size(other._size){
            other._data = nullptr;
            other._size = 0;
            return *this;
        }

    public:
        BitmapFree() = default;
        ~BitmapFree() = default;
        BitmapFree(_Tp *buffer, size_t numberOfelems) : _data(buffer), _size(numberOfelems) {}
    };

    template<typename _Tp, size_t _size>
    class BitmapConst: public BitmapBase<_Tp>{
        typedef BitmapConst<_Tp, _size> CurType;
        _Tp _data[_size / cerb::sizeofbits<_Tp>() + 1] = {0};
        static_assert(std::is_unsigned<_Tp>::value, "Bitmap can hold only unsigned values");
    
    public:
        always_inline size_t size() const { return _size; }
        always_inline const _Tp   *data() const { return (_Tp*)((char*)this + offsetof(CurType, _data)); }

    public:
        always_inline void set(size_t index, u8 value){
            if (index >= _size) return;
            this->SET((_Tp*)data(), index, value);
        }

        always_inline u8 at(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data(), index);
        }

        always_inline u8 operator[](size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT((_Tp*)data(), index);
        }

        always_inline size_t findFree() const {
            return this->FINDFREE((_Tp*)data(), _size);
        }

        always_inline bool isEmpty() const {
            return this->IS_EMPTY((_Tp*)data(), _size);
        }

        always_inline void clear(){
            this->CLEAR((_Tp*)data(), _size);
        }

    public:
        BitmapConst() = default;
    };

    template<typename _Tp>
    class Bitmap: public BitmapBase<_Tp>{
        size_t  _size;
        _Tp     *_data;
        static_assert(std::is_unsigned<_Tp>::value, "Bitmap can hold only unsigned values");
    
    public:
        always_inline size_t size() const { return _size; }
        always_inline const _Tp   *data() const { return _data; }

    public:
        always_inline void set(size_t index, u8 value){
            if (index >= _size) return;
            this->SET(_data, index, value);
        }

        always_inline u8 at(size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT(_data, index);
        }

        always_inline u8 operator[](size_t index) const {
            if (index >= _size) return UINT8_MAX;
            return this->AT(_data, index);
        }

        always_inline size_t findFree() const {
            return this->FINDFREE(_data, _size);
        }

        always_inline bool isEmpty() const {
            return this->IS_EMPTY((_Tp*)data(), _size);
        }

        always_inline void clear(){
            this->CLEAR((_Tp*)data(), _size);
        }

    public:
        void clearMem() { _size = 0; operator delete(_data); }

    public:
        Bitmap &operator=(Bitmap<_Tp> other){
            cerb::swap(_data, other._data);
            cerb::swap(_size, other._size);
            return *this;
        }

        Bitmap &operator=(const Bitmap<_Tp> &other){
            clearMem();
            _size = other.size();

            auto numberOfelems = other.size() / cerb::sizeofbits<_Tp>() + 1;
            _data =  static_cast<_Tp*>(operator new(sizeof(_Tp) * numberOfelems));
            
            for (size_t j = 0; j < numberOfelems; j++) _data[j] = other._data[j];
            return *this;
        }

        Bitmap &operator=(Bitmap<_Tp> &&other){
            clearMem();
            _data = other._data;
            _size = other.size();
            
            other._data = nullptr;
            other._size = 0;
            return *this;
        }

    public:
        Bitmap(Bitmap<_Tp> &other) : _size(other._size) {
            auto numberOfelems = other.size() / cerb::sizeofbits<_Tp>() + 1;
            _data =  static_cast<_Tp*>(operator new(sizeof(_Tp) * numberOfelems));
            
            for (size_t j = 0; j < numberOfelems; j++) _data[j] = other._data[j];
        }

        Bitmap(Bitmap<_Tp> &&other) : _data(other._data), _size(other._size){
            other._data = nullptr;
            other._size = 0;
            return *this;
        }

    public:
        Bitmap() = default;
        Bitmap(size_t numberOfelems) : _size(numberOfelems) {
            _data = static_cast<_Tp*>(operator new(sizeof(_Tp) * (numberOfelems / cerb::sizeofbits<_Tp>() + 1)));
        }

        ~Bitmap(){
            clearMem();
        }
    };
}

#pragma GCC diagnostic pop
#endif /* bitmap_hpp */
