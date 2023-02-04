#ifndef __test
#define __test

#include "main.h"
#include <utility>

class A
{
public:
    explicit A(int a = 10) : a{a} { std::cout << "ctor!\n"; }
    ~A() {std::cout << "dtor!\n";}
    A(const A& other) : a{other.a} { std::cout << "copy ctor!\n"; }
    A(A&& other) noexcept : a{other.a} { std::cout << "move ctor!\n"; }
    A& operator=(const A& other) { a = other.a; std::cout << "copy asn!\n"; return *this; }
    A& operator=(A&& other) { a = other.a; std::cout << "move asn!\n"; return *this; }
    
    friend std::ostream& operator<<(std::ostream& os, const A& dat)
    {
        return os << dat.a;
    }
    
    A& operator+=(const A& rhs)
    {
        a += rhs.a;
        return *this;
    }

private:
    int a;
};

A&& operator+(A lhs, const A& rhs);

#endif //__test