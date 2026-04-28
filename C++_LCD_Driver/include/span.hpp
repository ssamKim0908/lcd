#pragma once
#include <cstddef>
#include <type_traits>

template <class T>
class Span
{
    T*     ptr_;
    size_t len_;

    template <class C>
    using elem_t = std::remove_pointer_t<decltype(std::declval<C&>().data())>;

public:
    constexpr Span(T* ptr, size_t len) noexcept : ptr_(ptr), len_(len) {}

    template <class Container,
              class E = elem_t<Container>,
              std::enable_if_t<
                  std::is_same<std::remove_cv_t<E>, std::remove_cv_t<T>>::value &&
                  (std::is_const<T>::value || !std::is_const<E>::value),
                  int> = 0>
    constexpr Span(Container& c) noexcept
        : ptr_(c.data()), len_(c.size()) {}

    template <class Container,
              class E = elem_t<Container>,
              std::enable_if_t<
                  std::is_same<std::remove_cv_t<T>, std::byte>::value &&
                  !std::is_same<std::remove_cv_t<E>, std::byte>::value &&
                  std::is_trivially_copyable<std::remove_cv_t<E>>::value &&
                  (std::is_const<T>::value || !std::is_const<E>::value),
                  int> = 0>
    Span(Container& c) noexcept
        : ptr_(reinterpret_cast<T*>(c.data())),
          len_(c.size() * sizeof(E)) {}

    constexpr T*     data()  const noexcept { return ptr_; }
    constexpr size_t size()  const noexcept { return len_; }
    constexpr bool   empty() const noexcept { return len_ == 0; }

    constexpr T* begin() const noexcept { return ptr_; }
    constexpr T* end()   const noexcept { return ptr_ + len_; }

    constexpr T& operator[](size_t i) const noexcept { return ptr_[i]; }
    ~Span() = default;
};
