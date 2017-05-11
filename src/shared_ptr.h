#ifndef __SHARED_PTR_H__
#define __SHARED_PTR_H__

#include <functional>

template<typename T>
class SharedPtr final {
public:
    SharedPtr() = default;

    explicit SharedPtr(std::nullptr_t np) {}

    explicit SharedPtr(T *ptr)
        : ptr_(ptr)
        , counter_(new long(1))
    {}

    template<typename D>
    SharedPtr(T *ptr, D deleter)
        : ptr_(ptr)
        , counter_(new long(1))
        , deleter_(deleter)
    {}

    SharedPtr(SharedPtr const &other)
        : ptr_(other.ptr_)
        , counter_(other.counter_)
        , deleter_(other.deleter_)
    {
        if(counter_) {
            ++*counter_;
        }
    }

    SharedPtr(SharedPtr &&other) noexcept
        : ptr_(other.ptr_)
        , counter_(other.counter_)
        , deleter_(other.deleter_)
    {
        other.ptr_ = nullptr;
        other.counter_ = nullptr;
    }

    ~SharedPtr() {
        Dispose();
    }

    SharedPtr &operator=(SharedPtr const &other) {
        if(this != &other) {
            Dispose();
            ptr_ = other.ptr_;
            counter_ = other.counter_;
            deleter_ = other.deleter_;
            if(counter_) {
                ++*counter_;
            }
        }
        return *this;
    }

    SharedPtr &operator=(SharedPtr &&other) {
        if(this != &other) {
            Dispose();
            ptr_ = other.ptr_;
            counter_ = other.counter_;
            deleter_ = other.deleter_;
            other.ptr_ = nullptr;
            other.counter_ = nullptr;
        }
        return *this;
    }

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    operator bool() const { return ptr_; }

    void reset(T *ptr = nullptr) {
        Dispose();
        if(ptr) {
            ptr_ = ptr;
            counter_ = new long(1);
        }
    }

    template<typename D>
    void reset(T *ptr, D deleter) {
        reset(ptr);
        deleter_ = deleter;
    }

    long use_count() const {
        if(counter_) {
            return *counter_;
        }
        return 0;
    }
private:
    T *ptr_ = nullptr;
    long *counter_ = nullptr;
    std::function<void(T*)> deleter_ = DefaultDeleter;

    void Dispose() {
        if(counter_) {
            if(*counter_ == 1) {
                deleter_(ptr_);
                delete counter_;
            } else {
                --*counter_;
            }
            ptr_ = nullptr;
            counter_ = nullptr;
        }
    }

    static void DefaultDeleter(T *ptr) {
        delete ptr;
    }
};

// TODO implement make_shared

#endif