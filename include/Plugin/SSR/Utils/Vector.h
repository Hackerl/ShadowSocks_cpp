//
// Created by hackerl on 11/4/18.
//

#ifndef SSRPLUGIN_VECTOR_H
#define SSRPLUGIN_VECTOR_H

#include <vector>
#include <string>
#include <sys/types.h>

template <class T>
inline void VectorAppend(std::vector<T> & v1, std::vector<T> v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template <class T>
inline void VectorAppend(std::vector<T> & v1, std::vector<T> v2, size_t Size)
{
    v1.insert(v1.end(), v2.begin(), v2.begin() + Size);
}

template <class T>
inline void VectorAppend(std::vector<T> & v1, T * v2, size_t Size)
{
    v1.insert(v1.end(), v2, v2 + Size);
}

template <class T>
inline void VectorAppendStr(std::vector<T> & v1, std::string v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template <class T, class T_>
inline void VectorAppendData(std::vector<T> & v1, T_ v2)
{
    v1.insert(v1.end(), (T *)&v2, (T *)&v2 + sizeof(T_));
}

template <class T>
inline void VectorPrepend(std::vector<T> & v1, std::vector<T> v2)
{
    v1.insert(v1.begin(), v2.begin(), v2.end());
}

template <class T>
inline void VectorPrepend(std::vector<T> & v1, std::vector<T> v2, size_t Size)
{
    v1.insert(v1.begin(), v2.begin(), v2.begin() + Size);
}

template <class T>
inline void VectorPrepend(std::vector<T> & v1, T * v2, size_t Size)
{
    v1.insert(v1.begin(), v2, v2 + Size);
}

template <class T>
inline void VectorPrependStr(std::vector<T> & v1, std::string v2)
{
    v1.insert(v1.begin(), v2.begin(), v2.end());
}

template <class T, class T_>
inline void VectorPrependData(std::vector<T> & v1, T_ v2)
{
    v1.insert(v1.begin(), (T *)&v2, (T *)&v2 + sizeof(T_));
}

#endif //SSRPLUGIN_VECTOR_H
