
template <class T>
class span
{
private:
    T* data;
    size_t size;
public:
    span(T* data, size_t size) : data(data), size(size) {};

    template <class Container>
    span(Container& container) : data(container.data()), size(container.size()) {};

    T* data() const { return data; }
    size_t size() const { return size; }
}