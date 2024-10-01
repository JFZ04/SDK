#ifndef CCVector_HPP
#define CCVector_HPP

#include <cstdlib>

template <typename T>
class CCVector
{
public:
    typedef T value_type;
    typedef T* iterator;
    CCVector() : _start(nullptr), _finish(nullptr), _end(nullptr)
    {
        _buffer[ID] = 0;
    }
    ~CCVector() = default;
    void push_back(const value_type &x);
    T pop_back();
    value_type operator[] (int id) const
    {
        return _buffer[id];
    }
    void clear()
    {
        _buffer = (value_type*)malloc(sizeof(value_type) * 2);
        ID = 0;
        _buffer[ID] = 0;
    }
    unsigned int size()
    {
        return ID;
    }
    bool empty()
    {
        return ID == 0;
    }
    void reserve(unsigned int size)
    {
        _buffer = (value_type*)realloc(_buffer, sizeof(value_type) * size);
    }
    bool operator==(const value_type &x)
    {
        return _buffer[ID] == x;
    }
    void erase(int index,int len)
    {
        int a = ID - (len - index);
        value_type* b = (value_type*)malloc(sizeof(value_type) * a);
        for (int i = 0; i < index; ++i)
        {
            b[i] = _buffer[i];
        }
        for (int i = index; i < a; ++i)
        {
            b[i] = _buffer[i+len];
        }
        ID = ID - len;
        _buffer = b;
    }
    void revise(int index,value_type x)
    {
        _buffer[index] = x;
    }
    void insert(int index, value_type x)
    {
        push_back(_buffer[ID-1]);
        for(int i = index; i < ID; i++)
        {
            _buffer[i + 1] = _buffer[i];
        }
        _buffer[index] = x;
    }


private:
    iterator _start;
    iterator _finish;
    iterator _end;
    value_type* _buffer = (value_type*)malloc(sizeof(value_type) * 2);
    unsigned int ID = 0;

};

template<typename T>
T CCVector<T>::pop_back()
{
    return _buffer[--ID];
}


template<typename T>
inline void CCVector<T>::push_back(const value_type &x)
{
    _buffer[ID++] = x;
    _buffer = (value_type*)realloc(_buffer, sizeof(value_type) * (ID + 1));
    _buffer[ID] = 0;
}


#endif
