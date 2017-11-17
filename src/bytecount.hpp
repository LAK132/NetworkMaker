#include <memory>

#ifndef BYTECOUNT_H
#define BYTECOUNT_H

namespace btc {
    //calls .size() if array type
    template<class T>
    auto count_imp(T& obj, int)
        -> decltype(obj.size(), size_t())
    {
        return obj.size();
    }
    //fallback, calls sizeof()
    template<class T>
    auto count_imp(T& obj, long)
        -> decltype(sizeof(obj), size_t())
    {
        return sizeof(obj);
    }
    //returns the size of the object using either .size() or sizeof()
    template<class T>
    auto bytecount(T& obj) -> decltype(count_imp(obj, 0), size_t()) {
        return count_imp(obj, 0);
    }

    //gets pointer to first element
    template<class T>
    auto srcptr(T& src, int) -> decltype(&(src[0]))
    {
        return &(src[0]);
    }
    //fallback, gets pointer to object
    template<class T>
    auto srcptr(T& src, long) -> decltype(&src)
    {
        return &src;
    }
    //calls .resize() if available
    template<class T, class N>
    auto bytecpy_imp(T& dst, const N& src, int) -> decltype(dst.resize(bytecount(src)), void())
    {
        dst.resize(bytecount(src));
        std::memcpy(&(dst[0]), srcptr(src, 0), bytecount(src));
    }
    //fallback, assume memory has already been allocated
    template<class T, class N>
    auto bytecpy_imp(T& dst, const N& src, long) -> decltype(bytecount(src), void())
    {
        std::memcpy(&dst, srcptr(src, 0), bytecount(src));
    }
    //copies memory from src to dst, works with std::vectors and std::strings
    template<class T, class N>
    auto bytecpy(T& dst, const N& src) -> decltype(bytecpy_imp(dst, src, 0), void()) {
        bytecpy_imp(dst, src, 0);
    }
}

#endif
