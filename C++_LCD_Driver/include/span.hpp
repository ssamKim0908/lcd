#pragma once   

template <class T>
class span
{
private:
    T* ptr;
    size_t len;
public:
    span(T* ptr, size_t len) : ptr(ptr), len(len) {};

    template <class Container>
    span(Container& container) : ptr(container.data()), len(container.size()) {};

    //Todo : 이니셜라이저 리스트 생성자 필요

    T* data() const { return ptr; }
    size_t size() const { return len; }
};