#ifndef GD3DTD_HELPERS
#define GD3DTD_HELPERS

template<typename T>
class unique_node_ptr
{
    T *ptr = nullptr;
    bool should_d;

public:
    unique_node_ptr() : ptr(nullptr), should_d(true) {};
    explicit unique_node_ptr(std::nullptr_t) : ptr(nullptr), should_d(true) {};
    explicit unique_node_ptr(const T* ptr) : ptr(ptr), should_d(true) {};
    explicit unique_node_ptr(const T *ptr, const bool should_delete) : ptr(ptr), should_d(should_delete) {};

    ~unique_node_ptr()
    {
       reset();
    }

    T* operator->() const { return ptr; }
    T& operator*()  const { return *ptr; }
    T* get() const { return ptr; }

    explicit operator bool() { return ptr;}

    unique_node_ptr& operator=(std::nullptr_t)
    {
        reset();
        ptr = nullptr;
        return *this;
    }
    unique_node_ptr& operator=(T *other)
    {
        reset();
        ptr = other;
        return *this;
    }
    bool operator==(std::nullptr_t) const { return ptr == nullptr; }
    bool operator!=(std::nullptr_t) const { return ptr != nullptr; }
    bool operator==(T* other) const { return ptr == other; }
    bool operator!=(T* other) const { return ptr == other; }


    T* release()
    {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }

    void reset()
    {
        if (ptr != nullptr && should_d)
            ptr->queue_free();
        ptr = nullptr;
    }

    void set_non_permanency(const bool perm) { should_d = perm; }

    //Avoid creation of these methods by compiler
    unique_node_ptr(unique_node_ptr const&) = delete;
    unique_node_ptr& operator=(unique_node_ptr const&) = delete;

private:

};
#endif // !