#ifndef cerbMove_hpp
#define cerbMove_hpp

#include <cerberus/cerberusc++.hpp>

namespace cerb {

    template<typename _Tp>
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept {
        return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);
    }

    template<typename _Tp>
    _Tp *move(_Tp *_dest, _Tp *_src, size_t _size){
        for (size_t __iterator = 0; __iterator < _size; __iterator++)
            new(_dest + __iterator) _Tp(static_cast<_Tp&&>(_src[__iterator]));
        
        return _dest;
    }

    template<typename _Tp>
    _Tp *copy(_Tp *_dest, _Tp *_src, size_t _size){
        for (size_t __iterator = 0; __iterator < _size; __iterator++)
            new (_dest + __iterator) _Tp(static_cast<_Tp&>(_src[__iterator]));
        
        return _dest;
    }

    template<typename _Tp>
    inline void swap(_Tp &_lvalue, _Tp &_rvalue){
        _Tp __tmp   = cerb::move(_lvalue);
        _lvalue     = cerb::move(_rvalue);
        _rvalue     = cerb::move(__tmp);
    }

    template<typename _Tp>
    always_inline void destroy(_Tp &_value){
        (&_value)->~_Tp();
    }

    template<typename _Tp>
    inline void destroy(_Tp &_iterator_begin, _Tp &iterator_end){
        for (; _iterator_begin != iterator_end; ++_iterator_begin)
            cerb::destroy(*_iterator_begin);
    }

    template<typename _Tp>
    inline void destroy(_Tp &&_iterator_begin, _Tp &&_iterator_end){
        for (; _iterator_begin != _iterator_end; ++_iterator_begin)
            cerb::destroy(*_iterator_begin);
    }

    template<typename S, typename _Tp>
    inline void destroy(S *_data, _Tp _times){
        for (size_t __iterator = 0; __iterator < _times; ++__iterator)
            cerb::destroy(_data[__iterator]);
    }

    template<typename S, typename _Tp>
    inline void destroy(S *_data, _Tp _first_index, _Tp _last_index){
        for (; _first_index < _last_index; ++_first_index)
            destroy(_data[_first_index]);
    }

    template<typename S, typename _Tp>
    inline void reverse(S *_data, _Tp _size){
        for (_Tp __iterator = 0; __iterator < (_size >> 1); __iterator++)
            swap(_data[__iterator], _data[_size - __iterator - 1]);
    }

    template<typename S, typename _Tp>
    inline void reverse(S *_data, _Tp _first_index, _Tp _last_index){
        _Tp __size = _last_index - _first_index;

        for (_Tp __iterator = _first_index; __iterator < (__size >> 1); __iterator++)
            swap(_data[__iterator], _data[__size - __iterator - 1]);
    }

    template<typename _Tp>
    void reverse(_Tp &_iterator_begin, _Tp &_iterator_end){
       
        while (_iterator_begin != _iterator_end){
            swap(*_iterator_begin, *_iterator_end);
            
            _iterator_begin++;
            if (_iterator_begin == _iterator_end) break;

            _iterator_end--;    
        }
    }
}

#endif /* cerbMove_hpp */
