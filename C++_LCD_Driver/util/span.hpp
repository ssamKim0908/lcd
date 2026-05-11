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

    template <class C>
    using elem_t = std::remove_pointer_t<decltype(std::declval<C&>().data())>;

public:
    constexpr Span(T* ptr, size_t len) noexcept : ptr_(ptr), len_(len) {}

    template <size_t N>
    constexpr Span(T (&arr)[N]) noexcept : ptr_(arr), len_(N) {}

    template <class Container>
    constexpr Span(Container& c) noexcept : ptr_(c.data()), len_(c.size()) {}

    constexpr T*     data()  const noexcept { return ptr_; }
    constexpr size_t size()  const noexcept { return len_; }
    constexpr bool   empty() const noexcept { return len_ == 0; }

    constexpr T* begin() const noexcept { return ptr_; }
    constexpr T* end()   const noexcept { return ptr_ + len_; }

    constexpr T& operator[](size_t i) const noexcept { return ptr_[i]; }

    ~Span() = default;
};

template <class T>
Span<const std::byte> as_bytes(const T& v) noexcept
{
    static_assert(std::is_trivially_copyable<T>::value,
                  "as_bytes requires a trivially copyable type");
    return Span<const std::byte>(
        reinterpret_cast<const std::byte*>(&v), sizeof(T));
}

template <class T>
Span<std::byte> as_writable_bytes(T& v) noexcept
{
    static_assert(std::is_trivially_copyable<T>::value,
                  "as_writable_bytes requires a trivially copyable type");
    static_assert(!std::is_const<T>::value,
                  "as_writable_bytes requires a non-const object");
    return Span<std::byte>(
        reinterpret_cast<std::byte*>(&v), sizeof(T));
}

} // namespace util