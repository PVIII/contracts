/*
 * main.cpp
 *
 *  Created on: 13 Jul 2018
 *      Author: pait
 */

#define CONTRACTS_AUDIT

#include "contracts.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <streambuf>

#include "ios.h"

class contract_violation_exception : public std::exception
{
};
class other_exception : public std::exception
{
};

void throwing() { throw contract_violation_exception(); }
void printing() { std::cout << "printing"; }

static void (*registered_handler)() = nullptr;

void contract_violation_handler(contract_violation_info const &)
{
    if(registered_handler != nullptr)
    {
        registered_handler();
    }
}

constexpr int bad_value  = 1;
constexpr int good_value = 0;

void do_expect(int x) { EXPECT(x == good_value); }
void do_ensure(int x) { ENSURE(a, x == good_value); }
void do_expect_and_throw(int x)
{
    EXPECT(x == good_value);
    throw other_exception();
}
void do_ensure_and_throw(int x)
{
    ENSURE(a, x == good_value);
    throw other_exception();
}
void do_throw_and_ensure(int x)
{
    throw other_exception();
    ENSURE(a, x == good_value);
}

SCENARIO("Pre and postconditions")
{
    registered_handler = &printing;
    GIVEN("Normal functions")
    {
        WHEN("The precondition fails")
        {
            ostream_capture capture(std::cout);
            do_expect(bad_value);
            REQUIRE(capture.str() == "printing");
        }
        WHEN("The postcondition fails")
        {
            ostream_capture capture(std::cout);
            do_ensure(bad_value);
            REQUIRE(capture.str() == "printing");
        }
    }
    GIVEN("Lambda functions")
    {
        auto do_expect = [](int x) { EXPECT(x == good_value); };
        auto do_ensure = [](int x) { ENSURE(a, x == good_value); };
        WHEN("The precondition fails")
        {
            ostream_capture capture(std::cout);
            do_expect(bad_value);
            REQUIRE(capture.str() == "printing");
        }
        WHEN("The postcondition fails")
        {
            ostream_capture capture(std::cout);
            do_ensure(bad_value);
            REQUIRE(capture.str() == "printing");
        }
    }
    GIVEN("Throwing functions")
    {
        WHEN("The precondition fails before throw")
        {
            ostream_capture capture(std::cout);
            REQUIRE_THROWS_AS(do_expect_and_throw(bad_value),
                              other_exception &);
            REQUIRE(capture.str() == "printing");
        }
        WHEN("The postcondition is declared before throw")
        {
            ostream_capture capture(std::cout);
            REQUIRE_THROWS_AS(do_ensure_and_throw(bad_value),
                              other_exception &);
            THEN("The postcondition is not checked")
            {
                REQUIRE(capture.str() == "");
            }
        }
        WHEN("The postcondition is declared after throw")
        {
            ostream_capture capture(std::cout);
            REQUIRE_THROWS_AS(do_throw_and_ensure(bad_value),
                              other_exception &);
            THEN("The postcondition is not checked")
            {
                REQUIRE(capture.str() == "");
            }
        }
    }
}

SCENARIO("Throwing handler")
{
    registered_handler = &throwing;
    GIVEN("Normal functions")
    {
        WHEN("The precondition fails")
        {
            REQUIRE_THROWS_AS(do_expect(bad_value),
                              contract_violation_exception &);
        }
        WHEN("The postcondition fails")
        {
            REQUIRE_THROWS_AS(do_ensure(bad_value), std::exception &);
        }
    }
}

struct contract_dummy
{
    int  x;
    void invariant() { ASSERT(x == good_value); }
    void do_expect(int y) { EXPECT(y == good_value); }
    void do_ensure(int y) { ENSURE(a, y == good_value); }
    void do_ensure_invariant() { ENSURE_INVARIANT(); }
    void do_ensure_invariant_and_throw()
    {
        ENSURE_INVARIANT();
        throw other_exception();
    }
};

SCENARIO("Member functions")
{
    GIVEN("A throwing handler")
    {
        registered_handler = &throwing;

        GIVEN("A good instance")
        {
            contract_dummy dummy{good_value};

            WHEN("The precondition holds")
            {
                REQUIRE_NOTHROW(dummy.do_expect(good_value));
            }
            WHEN("The precondition fails")
            {
                REQUIRE_THROWS_AS(dummy.do_ensure(bad_value),
                                  contract_violation_exception &);
            }
            WHEN("The postcondition fails")
            {
                REQUIRE_THROWS_AS(dummy.do_expect(bad_value),
                                  contract_violation_exception &);
            }
            WHEN("The invariant is asserted")
            {
                THEN("The invariant holds")
                {
                    REQUIRE_NOTHROW(dummy.do_ensure_invariant());
                }
            }
            GIVEN("A throwing method")
            {
                WHEN("The invariant is asserted")
                {
                    THEN("The invariant is checked and holds")
                    {
                        REQUIRE_THROWS_AS(dummy.do_ensure_invariant_and_throw(),
                                          other_exception &);
                    }
                }
            }
        }
        GIVEN("A bad instance")
        {
            contract_dummy dummy{bad_value};

            WHEN("The invariant is asserted")
            {
                THEN("The invariant fails")
                {
                    REQUIRE_THROWS_AS(dummy.do_ensure_invariant(),
                                      contract_violation_exception &);
                }
            }
        }
    }
    GIVEN("A printing handler")
    {
        registered_handler = &printing;

        GIVEN("A bad instance")
        {
            contract_dummy dummy{bad_value};

            GIVEN("A throwing method")
            {
                WHEN("The invariant is asserted")
                {
                    ostream_capture capture(std::cout);
                    REQUIRE_THROWS_AS(dummy.do_ensure_invariant_and_throw(),
                                      other_exception &);
                    THEN("The invariant is checked and fails")
                    {
                        REQUIRE(capture.str() == "printing");
                    }
                }
            }
        }
    }
}

struct ensure_in_destructor
{
    ~ensure_in_destructor() { do_ensure(bad_value); }
};

void do_ensure_during_unwind()
{
    ensure_in_destructor e;
    throw other_exception();
}

SCENARIO("Stack unwinding")
{
    registered_handler = &printing;

    GIVEN("A throwing function with an RAII resource")
    {
        WHEN("The resource has a failing postcondition in its destructor")
        {
            ostream_capture capture(std::cout);
            REQUIRE_THROWS_AS(do_ensure_during_unwind(), other_exception &);
            THEN("The postcondition is checked and fails")
            {
                REQUIRE(capture.str() == "printing");
            }
        }
    }
}

constexpr void const_do_expect(int x) { EXPECT(x == good_value); }
constexpr void const_do_assert(int x) { ASSERT(x == good_value); }

SCENARIO("Constant expressions")
{
    GIVEN("A throwing handler")
    {
        registered_handler = &throwing;

        GIVEN("A precondition in a constexpr function")
        {
            WHEN("The precondition holds")
            {
                THEN("compile") { const_do_expect(good_value); }
            }
        }
        GIVEN("An assertion in a contexpr function")
        {
            WHEN("The assertion holds")
            {
                THEN("compile") { const_do_assert(good_value); }
            }
        }
    }
}
