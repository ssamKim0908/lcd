#pragma once
#include <cstddef>
#include <type_traits>

namespace util
{

template <class T>
class Span
{
    T*     ptr_;
    size_t len_;

public:
    Span(T* ptr, size_t len)  : ptr_(ptr), len_(len) {}

    template <size_t N>
    Span(T (&arr)[N])  : ptr_(arr), len_(N) {}

    template <class Container>
    Span(Container& c)  : ptr_(c.data()), len_(c.size()) {}

    T*     data()  const  { return ptr_; }
    size_t size()  const  { return len_; }
    bool   empty() const  { return len_ == 0; }

    T* begin() const  { return ptr_; }
    T* end()   const  { return ptr_ + len_; }

    T& operator[](size_t i) const  { return ptr_[i]; }

    ~Span() = default;
};

template <class Container>
Span(Container& c) -> Span<typename Container::value_type>;

template <class Container>
Span(const Container& c) -> Span<const typename Container::value_type>;

template <class T>
Span<const std::byte> as_bytes(Span<T> s) 
{
    static_assert(std::is_trivially_copyable<T>::value,
                  "as_bytes requires a trivially copyable type");
    return Span<const std::byte>(
        reinterpret_cast<const std::byte*>(s.data()), s.size() * sizeof(T));
}

template <class T>
Span<std::byte> as_writable_bytes(Span<T> s) 
{
    static_assert(std::is_trivially_copyable<T>::value,
                  "as_writable_bytes requires a trivially copyable type");
    static_assert(!std::is_const<T>::value,
                  "as_writable_bytes requires a non-const object");
    return Span<std::byte>(
        reinterpret_cast<std::byte*>(s.data()), s.size() * sizeof(T));
}

} // namespace util