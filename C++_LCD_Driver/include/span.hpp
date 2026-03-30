
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

    T* data() const { return ptr; }
    size_t size() const { return len; }
};