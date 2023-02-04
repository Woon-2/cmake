#include "../inc/test.h"

A&& operator+(A lhs, const A& rhs)
{
    return std::move(lhs += rhs);
}