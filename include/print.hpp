#ifndef PRINT_HPP
#define PRINT_HPP

std::ostream&
print_one(std::ostream& os);

template <class A0, class ...Args>
std::ostream&
print_one(std::ostream& os, const A0& a0, const Args& ...args);

template <class ...Args>
std::ostream&
print(std::ostream& os, const Args& ...args);

template <class ...Args>
std::ostream&
print(const Args& ...args);

#endif
