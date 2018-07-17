/*
 * main.cpp
 *
 *  Created on: 13 Jul 2018
 *      Author: pait
 */

#define CONTRACTS_AUDIT

#include "contracts.h"

#include <iostream>

void contract_violation_handler(const contract_violation_info &v)
{
    std::cout << "contract violation [" << v.type_ << "] in "
              << v.location_.file_name() << ":" << v.location_.line() << ", "
              << v.func_ << ", " << v.expression_ << std::endl;
}

void func()
{
    EXPECT(1 < 2);
    EXPECT_AUDIT(1 < 0);
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
    EXPECT(a < 10);

    return 2 * a;
}

struct A
{
    void invariant() { ASSERT(false); }
    void f()
    {
        EXPECT(1 > 4);
        ENSURE(silly, 1 > 3);
        ENSURE_INVARIANT();
    }
    void g()
    {
        EXPECT(1 > 4);
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
