/*
 * main.cpp
 *
 *  Created on: 13 Jul 2018
 *      Author: pait
 */

#include "contracts.h"

#include <iostream>

void contract_violation_handler(contract_violation v)
{
    std::cout << "contract violation in " << v.file_ << ":" << v.line_ << " "
              << v.function_ << ", " << v.expression_ << std::endl;
}

void func()
{
    EXPECT_IMPL(1 < 2);
    std::cout << "func();" << std::endl;
    ASSERT(2 > 3);
    std::cout << "func2();" << std::endl;
    ENSURE(silly, 2 < 1);
}

void throw_func()
{
    ENSURE(always_false, false);
    throw 0;
}

constexpr int const_func(int a)
{
    EXPECT_IMPL(a < 10);

    return ENSURE_RETURN(result > 2, 2 * a);
}

struct A
{
    bool invariant() { return false; }
    void f()
    {
        EXPECT_IMPL(1 > 4);
        ENSURE(silly, 1 > 3);
        ENSURE_INVARIANT();
    }
    void g()
    {
        EXPECT_IMPL(1 > 4);
        ENSURE(silly, 1 > 3);
        ENSURE_INVARIANT();
        throw 1;
    }
};

int main()
{
    constexpr auto b = const_func(9);
    auto           c = const_func(1);
    func();
    try
    {
        throw_func();
    }
    catch(...)
    {
    }
    std::cout << const_func(5) << " " << b << std::endl;
    A a;
    a.f();
    try
    {
        a.g();
    }
    catch(...)
    {
    }

    return 0;
}
