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

void do_expect(int x) { EXPECT(x < bad_value); }
void do_ensure(int x) { ENSURE(a, x < bad_value); }

SCENARIO("Normal functions")
{
    GIVEN("A normal handler")
    {
        registered_handler = &printing;
        WHEN("Postcondition fails")
        {
            ostream_capture capture(std::cout);
            do_expect(bad_value);
            REQUIRE(capture.str() == "printing");
        }
        WHEN("Precondition fails")
        {
            ostream_capture capture(std::cout);
            do_ensure(bad_value);
            REQUIRE(capture.str() == "printing");
        }
    }
    GIVEN("A throwing handler")
    {
        registered_handler = &throwing;
        WHEN("Precondition holds") { REQUIRE_NOTHROW(do_expect(good_value)); }
        WHEN("Precondition fails")
        {
            REQUIRE_THROWS_AS(do_expect(bad_value),
                              const contract_violation_exception &);
        }
        WHEN("Postcondition holds") { REQUIRE_NOTHROW(do_ensure(good_value)); }
        WHEN("Postcondition fails")
        {
            REQUIRE_THROWS_AS(do_ensure(bad_value), std::exception &);
        }
    }
}
