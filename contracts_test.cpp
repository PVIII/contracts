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
    GIVEN("A normal handler")
    {
        registered_handler = &printing;
        GIVEN("Normal functions")
        {
            WHEN("Precondition fails")
            {
                ostream_capture capture(std::cout);
                do_expect(bad_value);
                REQUIRE(capture.str() == "printing");
            }
            WHEN("Postcondition fails")
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
            WHEN("Precondition fails")
            {
                ostream_capture capture(std::cout);
                do_expect(bad_value);
                REQUIRE(capture.str() == "printing");
            }
            WHEN("Postcondition fails")
            {
                ostream_capture capture(std::cout);
                do_ensure(bad_value);
                REQUIRE(capture.str() == "printing");
            }
        }
        GIVEN("Throwing functions")
        {
            WHEN("Precondition fails before throw")
            {
                ostream_capture capture(std::cout);
                REQUIRE_THROWS_AS(do_expect_and_throw(bad_value),
                                  other_exception &);
                REQUIRE(capture.str() == "printing");
            }
            WHEN("Postcondition declared before throw")
            {
                ostream_capture capture(std::cout);
                REQUIRE_THROWS_AS(do_ensure_and_throw(bad_value),
                                  other_exception &);
                THEN("The postcondition is not checked")
                {
                    REQUIRE(capture.str() == "");
                }
            }
            WHEN("Postcondition declared after throw")
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
    GIVEN("A throwing handler")
    {
        registered_handler = &throwing;
        GIVEN("Normal functions")
        {
            WHEN("Precondition holds")
            {
                REQUIRE_NOTHROW(do_expect(good_value));
            }
            WHEN("Precondition fails")
            {
                REQUIRE_THROWS_AS(do_expect(bad_value),
                                  const contract_violation_exception &);
            }
            WHEN("Postcondition holds")
            {
                REQUIRE_NOTHROW(do_ensure(good_value));
            }
            WHEN("Postcondition fails")
            {
                REQUIRE_THROWS_AS(do_ensure(bad_value), std::exception &);
            }
        }
        GIVEN("Lambda functions")
        {
            auto do_expect = [](int x) { EXPECT(x == good_value); };
            auto do_ensure = [](int x) { ENSURE(a, x == good_value); };
            WHEN("Precondition holds")
            {
                REQUIRE_NOTHROW(do_expect(good_value));
            }
            WHEN("Precondition fails")
            {
                REQUIRE_THROWS_AS(do_expect(bad_value),
                                  const contract_violation_exception &);
            }
            WHEN("Postcondition holds")
            {
                REQUIRE_NOTHROW(do_ensure(good_value));
            }
            WHEN("Postcondition fails")
            {
                REQUIRE_THROWS_AS(do_ensure(bad_value), std::exception &);
            }
        }
    }
}
