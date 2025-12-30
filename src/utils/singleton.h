#pragma once

template <typename T>
class Singleton
{
public:
    ~Singleton() = default;
    Singleton() = default;
    
    static T& get()
    {
        static T instance;
        return instance;
    }

protected:

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};