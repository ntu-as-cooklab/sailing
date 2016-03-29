#include <iostream>
#include <mutex>

#include "print.hpp"

std::ostream&
print_one(std::ostream& os)
{
    return os;
}

template <class A0, class ...Args>
std::ostream&
print_one(std::ostream& os, const A0& a0, const Args& ...args)
{
    os << a0;
    return print_one(os, args...);
}

template <class ...Args>
std::ostream&
print(std::ostream& os, const Args& ...args)
{
    return print_one(os, args...);
}

template <class ...Args>
std::ostream&
print(const Args& ...args)
{
    static std::mutex m;
    std::lock_guard<std::mutex> _(m);
    return print(std::cout, args...);
}
