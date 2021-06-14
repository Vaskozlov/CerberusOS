#ifndef vector_hpp
#define vector_hpp

#include <initializer_list>
#include <cerberus/move.hpp>
#include <cerberus/cerberusc++.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder-ctor"

namespace cerb{

    template<typename _Tp>
    class vector{
        _Tp     *_data;
        size_t  _size;
        size_t  _capicity;

    private:
        void reallocate(){
            auto __oldData = _data;
            _data = cerb::move(static_cast<_Tp*>(operator new(_capicity * 2 * sizeof(_Tp))), __oldData, _size);
            _capicity *= 2;
            operator delete(__oldData);
        }

    public:
        void push_back(_Tp &&_value){
            if (_capicity == _size) reallocate();
            new (_data + (_size++)) _Tp(static_cast<_Tp&&>(_value));
        }

        void push_back(_Tp &_value){
            if (_capicity == _size) reallocate();
            *(_data + (_size++)) = _value;
        }

        template<typename... _Args>
        void emplace_back(_Args&&... _args){
            if (_capicity == _size) reallocate();
            new (_data + _size) _Tp(_args...);
            _size++;
        }

        void pop_back(){
            if (_size == 0) return;
            destroy(_data[--_size]);
        }

    public:
        inline auto *data() const { return _data; }
        inline auto size() const { return _size; }
        inline auto capicity() const { return _capicity; }

    public:
        inline _Tp &at(size_t index){ return _data[index]; }
        inline _Tp &operator[](size_t index) { return _data[index]; }
        inline const  _Tp &operator[](size_t index) const { return _data[index]; }

        inline _Tp &front() { return this->at(0); }
        inline _Tp &back()  { return this->at(_size - 1); }

    public:
        void reverse(){
            cerb::reverse(_data, _size);
        }

        void clear(){
            cerb::destroy(_data, _size);
            _size = 0;
        }

    public:
        friend bool operator==(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            if (_lhs.size() != _rhs.size()) return false;

            for (size_t __iterator = 0; __iterator < _lhs.size(); __iterator++)
                if (_lhs.at(__iterator) != _rhs.at(__iterator)) return false;

            return true;
        }

        inline friend bool operator!=(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            return !(_lhs == _rhs);
        }

        friend bool operator>(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            for (size_t i = 0; i < cerb::MIN(_lhs.size(), _rhs.size()); i++){
                if (_lhs.at(i) > _rhs[i]) return true;
                else if (_lhs.at(i) == _rhs.at(i)) continue;
                else return false;
            }
            return _lhs.size() > _rhs.size();
        }

        friend bool operator<(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            for (size_t i = 0; i < cerb::MIN(_lhs.size(), _rhs.size()); i++){
                if (_lhs.at(i) < _rhs[i]) return true;
                else if (_lhs.at(i) == _rhs.at(i)) continue;
                else return false;
            }
            return _lhs.size() < _rhs.size();
        }

        inline friend bool operator>=(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            return _lhs > _rhs || _lhs == _rhs;
        }

        inline friend bool operator<=(const cerb::vector<_Tp> &_lhs, const cerb::vector<_Tp> &_rhs){
            return _lhs < _rhs || _lhs == _rhs;
        }

    public:
        class iterator{
            _Tp *_elem;

        public:
            iterator &operator++(){
                this->_elem++;
                return *this;
            }

            iterator &operator--(){
                this->_elem--;
                return *this;
            }

            iterator operator++(int){
                this->_elem++;
                return *this;
            }

            iterator operator--(int){
                this->_elem--;
                return *this;
            }

            iterator &operator-=(size_t change){
                this->_elem -= change;
                return *this;
            }

            iterator &operator+=(size_t change){
                this->_elem += change;
                return *this;
            }

            _Tp &operator*(){
                return *_elem;
            }

        public:
            friend inline bool operator!=(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem != _rhs._elem;
            }

            friend inline bool operator>(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem > _rhs._elem;
            }

            friend inline bool operator<(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem < _rhs._elem;
            }

            friend inline bool operator==(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem == _rhs._elem;
            }

            friend inline bool operator>=(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem >= _rhs._elem;
            }

            friend inline bool operator<=(const iterator &_lhs, const iterator &_rhs){
                return _lhs._elem <= _rhs._elem;
            }

        public:
            iterator() = default;
            iterator(iterator &other) : _elem(other._elem) {}
            iterator(iterator &&other) : _elem(other._elem) { other._elem = nullptr; }
            iterator(_Tp* elem) : _elem(elem) {}
        };

        auto begin() const { return iterator(_data); }
        auto end() const { return iterator(_data + _size); }

    public:

        vector<_Tp> &operator=(vector<_Tp> other){
            cerb::swap(_data, other._data);
            cerb::swap(_size, other._size);
            cerb::swap(_capicity, other._capicity);
        }

        vector<_Tp> &operator=(const vector<_Tp> &other){
            if (this == &other) return *this;

            clear();

            if (capicity() < other.size()){
                operator delete(_data);
                _capicity = other.capicity();
                _data = static_cast<_Tp*>(operator new(other.capicity() * sizeof(_Tp)));
            }

            _size = other.size();
            cerb::copy(_data, other._data, other.size());

            return *this;
        }

        vector<_Tp> &operator==(vector<_Tp> &&other) noexcept{
            if (this == &other) return *this;
            this->~vector<_Tp>();

            _size = other.size();
            _capicity = other.capicity();
            _data = other.data();

            other._capicity = 0;
            other._data = nullptr;
            other._size = 0;

            return *this;
        }
    
    public:

        void resize(size_t newSize){
            if (newSize <= size())
                cerb::destroy(_data, newSize, _size);
            else{
                auto _newData = static_cast<_Tp*>(operator new(newSize * sizeof(_Tp)));
                cerb::move(_newData, _data, _size);
                
                for (size_t __iterator = _size; __iterator < newSize; __iterator++)
                    new (_newData + __iterator) _Tp();

                operator delete(_data);
                _data = _newData;
            }

            _size = newSize;
        }

        void reserve(size_t reservation){
            if (reservation <= capicity()) return;

            auto _newData = static_cast<_Tp*>(operator new(reservation * sizeof(_Tp)));
            cerb::move(_newData, _data, size());
            
            _capicity = reservation;
            operator delete(_data);
            _data = _newData;
        }

    public:

        vector(std::initializer_list<_Tp> elems) : _size(0), _capicity(cerb::MAX<size_t>(4, elems.size())){
            _data = static_cast<_Tp*>(operator new(_capicity * sizeof(_Tp)));
            for (auto elem: elems) push_back(cerb::move(elem));
        }

        vector(vector<_Tp> &other) : _size(other._size), _capicity(other._capicity){
            _data = static_cast<_Tp*>(operator new(_capicity * sizeof(_Tp)));
            cerb::copy(_data, other._data, _size);
        }

        vector(vector<_Tp> &&other) : _size(other._size), _capicity(other._capicity){
            _data = other._data;
            other._data = nullptr;
            other._size = 0;
            other._capicity = 0;
        }

    public:
        vector() : _size(0), _capicity(4), _data(static_cast<_Tp*>(operator new(4 * sizeof(_Tp)))) {}
        vector(size_t reservation) : _size(0), _capicity(reservation), _data(static_cast<_Tp*>(operator new(reservation * sizeof(_Tp)))) {}
        ~vector() { this->clear(); operator delete(_data); }
    };
}

#pragma GCC diagnostic pop
#endif /* vector_hpp */
